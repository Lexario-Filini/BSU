import logging
import time
from fastapi import FastAPI, Request, HTTPException
from fastapi.responses import PlainTextResponse
from prometheus_client import Counter, Histogram, generate_latest, CONTENT_TYPE_LATEST

from models import TaskCreate, TaskUpdate, TaskPatch, TaskOut
from repository import get_repository
from cache import cache

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
)
logger = logging.getLogger("todo_api")

REQUEST_COUNT = Counter(
    "http_requests_total", "Total HTTP requests", ["method", "endpoint", "http_status"]
)
REQUEST_LATENCY = Histogram(
    "http_request_duration_seconds", "Request latency", ["method", "endpoint"]
)

app = FastAPI(title="To-Do API with Logging & Metrics")

repo = get_repository()

@app.middleware("http")
async def log_requests(request: Request, call_next):
    start_time = time.time()
    logger.info(f"➡️ {request.method} {request.url.path}")
    try:
        response = await call_next(request)
    except Exception:
        logger.exception("Error while processing request")
        raise
    process_time = time.time() - start_time
    REQUEST_LATENCY.labels(request.method, request.url.path).observe(process_time)
    REQUEST_COUNT.labels(request.method, request.url.path, response.status_code).inc()
    logger.info(
        f"⬅️ {request.method} {request.url.path} finished in {process_time:.3f}s "
        f"with status {response.status_code}"
    )
    return response

@app.get("/metrics")
def metrics():
    return PlainTextResponse(generate_latest(), media_type=CONTENT_TYPE_LATEST)

@app.post("/tasks", response_model=TaskOut, status_code=201)
def create_task(task: TaskCreate):
    created = repo.create_task(task.dict())
    cache.invalidate("tasks_all")
    return created

@app.get("/tasks", response_model=list[TaskOut])
def list_tasks():
    cached = cache.get("tasks_all")
    if cached:
        return cached
    tasks = repo.list_tasks()
    cache.set("tasks_all", tasks)
    return tasks

@app.get("/tasks/{task_id}", response_model=TaskOut)
def get_task(task_id: int):
    cached = cache.get(f"task_{task_id}")
    if cached:
        return cached
    task = repo.get_task(task_id)
    if not task:
        raise HTTPException(status_code=404, detail="Task not found")
    cache.set(f"task_{task_id}", task)
    return task

@app.put("/tasks/{task_id}", response_model=TaskOut)
def update_task(task_id: int, task: TaskUpdate):
    updated = repo.update_task(task_id, task.dict())
    if not updated:
        raise HTTPException(status_code=404, detail="Task not found")
    cache.invalidate(f"task_{task_id}")
    cache.invalidate("tasks_all")
    return updated

@app.patch("/tasks/{task_id}", response_model=TaskOut)
def patch_task(task_id: int, task: TaskPatch):
    updated = repo.patch_task(task_id, task.dict(exclude_unset=True))
    if not updated:
        raise HTTPException(status_code=404, detail="Task not found")
    cache.invalidate(f"task_{task_id}")
    cache.invalidate("tasks_all")
    return updated

@app.delete("/tasks/{task_id}", status_code=200)
def delete_task(task_id: int):
    deleted = repo.delete_task(task_id)
    if not deleted:
        raise HTTPException(status_code=404, detail="Task not found")
    cache.invalidate(f"task_{task_id}")
    cache.invalidate("tasks_all")
    return {"detail": "Task deleted"}
