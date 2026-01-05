import os
from fastapi import FastAPI, HTTPException
from typing import List
from models import TaskCreate, TaskUpdate, TaskPatch, TaskOut
from repository import TaskRepository
from db_sqlite import SQLiteRepository
from db_mongo import MongoRepository
from cache import TTLCache

app = FastAPI(title="To-Do List API", version="1.0.0")

db_type = os.getenv("DB_TYPE", "sqlite").lower()
repo: TaskRepository = SQLiteRepository() if db_type == "sqlite" else MongoRepository()

cache = TTLCache(ttl_seconds=30, max_items=1000)

@app.get("/tasks", response_model=List[TaskOut])
def list_tasks():
    cached = cache.get("tasks_all")
    if cached is not None:
        return cached
    items = repo.list_tasks()
    cache.set("tasks_all", items)
    return items

@app.post("/tasks", response_model=TaskOut, status_code=201)
def create_task(task: TaskCreate):
    created = repo.create_task(task.dict())
    cache.invalidate("tasks_all")
    cache.invalidate(f"task_{created['id']}")
    return created

@app.get("/tasks/{task_id}", response_model=TaskOut)
def get_task(task_id: int):
    cached = cache.get(f"task_{task_id}")
    if cached is not None:
        return cached
    item = repo.get_task(task_id)
    if not item:
        raise HTTPException(status_code=404, detail="Task not found")
    cache.set(f"task_{task_id}", item)
    return item

@app.put("/tasks/{task_id}", response_model=TaskOut)
def update_task(task_id: int, task: TaskUpdate):
    updated = repo.update_task(task_id, task.dict())
    if not updated:
        raise HTTPException(status_code=404, detail="Task not found")
    cache.invalidate("tasks_all")
    cache.invalidate(f"task_{task_id}")
    return updated

@app.patch("/tasks/{task_id}", response_model=TaskOut)
def patch_task(task_id: int, task: TaskPatch):
    updated = repo.patch_task(task_id, {k: v for k, v in task.dict().items() if v is not None or k == "description"})
    if not updated:
        raise HTTPException(status_code=404, detail="Task not found")
    cache.invalidate("tasks_all")
    cache.invalidate(f"task_{task_id}")
    return updated

@app.delete("/tasks/{task_id}")
def delete_task(task_id: int):
    ok = repo.delete_task(task_id)
    if not ok:
        raise HTTPException(status_code=404, detail="Task not found")
    cache.invalidate("tasks_all")
    cache.invalidate(f"task_{task_id}")
    return {"deleted": True}
