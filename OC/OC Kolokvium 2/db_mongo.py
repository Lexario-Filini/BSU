from typing import List, Optional, Dict, Any
from pymongo import MongoClient, ReturnDocument

client = MongoClient("mongodb://localhost:27017")
db = client["todo_api"]
tasks = db["tasks"]
counters = db["counters"]

def get_next_id() -> int:
    doc = counters.find_one_and_update(
        {"_id": "taskid"},
        {"$inc": {"seq": 1}},
        upsert=True,
        return_document=ReturnDocument.AFTER
    )
    if "seq" not in doc:
        counters.update_one({"_id": "taskid"}, {"$set": {"seq": 1}})
        return 1
    return doc["seq"]

class MongoRepository:
    def list_tasks(self) -> List[Dict[str, Any]]:
        items = list(tasks.find({}, {"_id": 0}))
        return items

    def create_task(self, data: Dict[str, Any]) -> Dict[str, Any]:
        new_id = get_next_id()
        doc = {"id": new_id, "title": data["title"], "description": data.get("description"), "status": data["status"]}
        tasks.insert_one(doc)
        return doc

    def get_task(self, task_id: int) -> Optional[Dict[str, Any]]:
        doc = tasks.find_one({"id": task_id}, {"_id": 0})
        return doc

    def update_task(self, task_id: int, data: Dict[str, Any]) -> Optional[Dict[str, Any]]:
        update_doc = {"title": data["title"], "description": data.get("description"), "status": data["status"]}
        doc = tasks.find_one_and_update({"id": task_id}, {"$set": update_doc}, return_document=ReturnDocument.AFTER, projection={"_id": 0})
        return doc

    def patch_task(self, task_id: int, data: Dict[str, Any]) -> Optional[Dict[str, Any]]:
        set_fields = {}
        if "title" in data and data["title"] is not None:
            set_fields["title"] = data["title"]
        if "description" in data:
            set_fields["description"] = data["description"]
        if "status" in data and data["status"] is not None:
            set_fields["status"] = data["status"]
        if not set_fields:
            return self.get_task(task_id)
        doc = tasks.find_one_and_update({"id": task_id}, {"$set": set_fields}, return_document=ReturnDocument.AFTER, projection={"_id": 0})
        return doc

    def delete_task(self, task_id: int) -> bool:
        res = tasks.delete_one({"id": task_id})
        return res.deleted_count == 1
