# -*- coding: utf-8 -*-
"""
pass
"""


import pprint
import collections
import six


class Singleton(type):
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(
                Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]


class AbqAttrDict(object):

    def __init__(self, d=None):
        self.__dict__ = d if d is not None else dict()

        for k, v in list(six.iteritems(self.__dict__)):
            if isinstance(v, dict):
                self.__dict__[k] = AbqAttrDict(v)

    def __repr__(self):
        return pprint.pformat(self.__dict__)

    def __iter__(self):
        return self.__dict__.__iter__()

    def update(self, other):
        AbqAttrDict._update_dict_recursive(self, other)

    def items(self):
        return six.iteritems(self.__dict__)

    iteritems = items

    def keys(self):
        return self.__dict__.keys()

    @staticmethod
    def _update_dict_recursive(target, other):
        if isinstance(other, AbqAttrDict):
            other = other.__dict__
        if isinstance(target, AbqAttrDict):
            target = target.__dict__

        for k, v in six.iteritems(other):
            if isinstance(v, collections.Mapping):
                r = AbqAttrDict._update_dict_recursive(target.get(k, {}), v)
                target[k] = r
            else:
                target[k] = other[k]
        return target

    def convert_to_dict(self):
        result_dict = {}
        for k, v in list(six.iteritems(self.__dict__)):
            if isinstance(v, AbqAttrDict):
                v = v.convert_to_dict()
            result_dict[k] = v
        return result_dict
