from typing import List, Optional, Dict, Any
from sqlalchemy import create_engine, Column, Integer, String, Text
from sqlalchemy.orm import sessionmaker, declarative_base

engine = create_engine("sqlite:///tasks.db", connect_args={"check_same_thread": False})
SessionLocal = sessionmaker(bind=engine)
Base = declarative_base()

class TaskModel(Base):
    __tablename__ = "tasks"
    id = Column(Integer, primary_key=True, index=True, autoincrement=True)
    title = Column(String(255), nullable=False)
    description = Column(Text, nullable=True)
    status = Column(String(32), nullable=False)

Base.metadata.create_all(bind=engine)

class SQLiteRepository:
    def list_tasks(self) -> List[Dict[str, Any]]:
        db = SessionLocal()
        items = db.query(TaskModel).all()
        db.close()
        return [{"id": i.id, "title": i.title, "description": i.description, "status": i.status} for i in items]

    def create_task(self, data: Dict[str, Any]) -> Dict[str, Any]:
        db = SessionLocal()
        item = TaskModel(title=data["title"], description=data.get("description"), status=data["status"])
        db.add(item)
        db.commit()
        db.refresh(item)
        result = {"id": item.id, "title": item.title, "description": item.description, "status": item.status}
        db.close()
        return result

    def get_task(self, task_id: int) -> Optional[Dict[str, Any]]:
        db = SessionLocal()
        item = db.query(TaskModel).filter(TaskModel.id == task_id).first()
        db.close()
        if not item:
            return None
        return {"id": item.id, "title": item.title, "description": item.description, "status": item.status}

    def update_task(self, task_id: int, data: Dict[str, Any]) -> Optional[Dict[str, Any]]:
        db = SessionLocal()
        item = db.query(TaskModel).filter(TaskModel.id == task_id).first()
        if not item:
            db.close()
            return None
        item.title = data["title"]
        item.description = data.get("description")
        item.status = data["status"]
        db.commit()
        db.refresh(item)
        result = {"id": item.id, "title": item.title, "description": item.description, "status": item.status}
        db.close()
        return result

    def patch_task(self, task_id: int, data: Dict[str, Any]) -> Optional[Dict[str, Any]]:
        db = SessionLocal()
        item = db.query(TaskModel).filter(TaskModel.id == task_id).first()
        if not item:
            db.close()
            return None
        if "title" in data and data["title"] is not None:
            item.title = data["title"]
        if "description" in data:
            item.description = data["description"]
        if "status" in data and data["status"] is not None:
            item.status = data["status"]
        db.commit()
        db.refresh(item)
        result = {"id": item.id, "title": item.title, "description": item.description, "status": item.status}
        db.close()
        return result

    def delete_task(self, task_id: int) -> bool:
        db = SessionLocal()
        item = db.query(TaskModel).filter(TaskModel.id == task_id).first()
        if not item:
            db.close()
            return False
        db.delete(item)
        db.commit()
        db.close()
        return True
