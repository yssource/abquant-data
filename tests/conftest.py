#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
    Dummy conftest.py for abquant.

    If you don't know what this is for, just leave it empty.
    Read more about conftest.py under:
    https://pytest.org/latest/plugins.html
"""

import pytest
from pyabquant import PyAbquant


@pytest.fixture(scope="session", autouse=True)
def my_own_session_run_at_beginning(request):
    print("\nIn my_own_session_run_at_beginning()")
    PyAbquant.start()

    def my_own_session_run_at_end():
        print("\nIn my_own_session_run_at_end()")
        PyAbquant.finish()

    request.addfinalizer(my_own_session_run_at_end)
