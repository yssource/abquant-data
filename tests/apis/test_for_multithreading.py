from threading import Thread
from abquant.apis.base import get_price


# FIXME: segmentation fault (core dumped)
def test():
    code = "000001"
    start = "2020-01-01"
    end = "2020-02-01"
    fields = ["open"]
    actual = get_price(code, start, end, fields=fields)
    return actual


if __name__ == "__main__":
    t = Thread(target=test, args=())
    t.start()
    t.join()
