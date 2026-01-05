import time
from typing import Any, Optional

class TTLCache:
    def __init__(self, ttl_seconds: int = 30, max_items: int = 1000):
        self.ttl = ttl_seconds
        self.max_items = max_items
        self.store = {}

    def _evict_if_needed(self):
        if len(self.store) <= self.max_items:
            return
        oldest_key = min(self.store.keys(), key=lambda k: self.store[k][1])
        self.store.pop(oldest_key, None)

    def get(self, key: str) -> Optional[Any]:
        item = self.store.get(key)
        if not item:
            return None
        value, ts = item
        if time.time() - ts > self.ttl:
            self.store.pop(key, None)
            return None
        return value

    def set(self, key: str, value: Any):
        self.store[key] = (value, time.time())
        self._evict_if_needed()

    def invalidate(self, key_prefix: str = ""):
        if not key_prefix:
            self.store.clear()
            return
        keys = [k for k in self.store.keys() if k.startswith(key_prefix)]
        for k in keys:
            self.store.pop(k, None)
