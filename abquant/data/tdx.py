from .base import IDataSource


class Stock(IDataSource):
    db = ""
    type = ""

    def __init__(self, db=DB):
        "docstring"
        self._db = db

    def create(self):
        print("create ... stock")
        self.create_day()
        self.create_min()

    def create_day(self):
        pass

    def create_min(self):
        pass

    def delete(self):
        print("delete ... stock")

    def update(self):
        print("update ... stock")

    def read(self):
        print("read ... stock")
