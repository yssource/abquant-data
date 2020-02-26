from .base import IDataSource


class Stock(IDataSource):
    db = ""
    type = ""

    def create(self):
        print("create ... stock")

    def delete(self):
        print("delete ... stock")

    def update(self):
        print("update ... stock")

    def read(self):
        print("read ... stock")
