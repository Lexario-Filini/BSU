import os

try:
    from db_sqlite import SQLiteRepository
except ImportError:
    SQLiteRepository = None

try:
    from database_mongo import MongoRepository
except ImportError:
    MongoRepository = None


def get_repository():
    db_type = os.getenv("DB_TYPE", "sqlite")

    if db_type == "mongo" and MongoRepository is not None:
        return MongoRepository()

    if SQLiteRepository is not None:
        return SQLiteRepository()

    raise ImportError("No valid repository found. Please check your DB_TYPE and repository files.")
