# -*- coding: utf-8 -*-

# class Singleton(type):
#     """
#     Singleton Metaclass.
#     Enforces any object using this metaclass to only create a single instance.
#     - External Usage documentation: U{https://github.com/tylerlaberge/PyPattyrn#singleton-pattern}
#     - External Singleton Pattern documentation: U{https://en.wikipedia.org/wiki/Singleton_pattern}
#     """
#     __instance = None

#     def __call__(cls, *args, **kwargs):
#         """
#         Override the __call__ method to make sure only one instance is created.
#         """
#         if cls.__instance is None:
#             cls.__instance = type.__call__(cls, *args, **kwargs)

#         return cls.__instance


def singleton(cls):
    instances = {}

    def _wrapper(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]

    return _wrapper
