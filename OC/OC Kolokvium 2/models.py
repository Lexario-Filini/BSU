from enum import Enum
from pydantic import BaseModel, Field
from typing import Optional

class Status(str, Enum):
    todo = "todo"
    in_progress = "in_progress"
    done = "done"

class TaskCreate(BaseModel):
    title: str = Field(..., min_length=1, max_length=255)
    description: Optional[str] = None
    status: Status = Status.todo

class TaskUpdate(BaseModel):
    title: str = Field(..., min_length=1, max_length=255)
    description: Optional[str] = None
    status: Status

class TaskPatch(BaseModel):
    title: Optional[str] = Field(None, min_length=1, max_length=255)
    description: Optional[str] = None
    status: Optional[Status] = None

class TaskOut(BaseModel):
    id: int
    title: str
    description: Optional[str] = None
    status: Status
