from typing import Optional
import pandas as pd


def get_stock_block() -> Optional[pd.DataFrame]:
    """ths的版块数据

    Returns:
        [type] -- [description]
    """

    url = "http://data.yutiansut.com/ths_block.csv"
    try:
        return pd.read_csv(url).set_index("code", drop=False)
    except:
        return None


def get_stock_list(type_=None) -> Optional[pd.DataFrame]:
    return


def my_name():
    return "THS"
