# -*- coding: utf-8 -*-
import collections


def deep_update(from_dict, to_dict):
    for (key, value) in from_dict.items():
        if (
            key in to_dict.keys()
            and isinstance(to_dict[key], collections.Mapping)
            and isinstance(value, collections.Mapping)
        ):
            deep_update(value, to_dict[key])
        else:
            to_dict[key] = value
