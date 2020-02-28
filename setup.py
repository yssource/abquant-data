#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""The setup script."""

from setuptools import setup, find_packages

with open("README.org") as readme_file:
    readme = readme_file.read()

with open("HISTORY.rst") as history_file:
    history = history_file.read()

requirements = [
    "Click>=7.0",
]

setup_requirements = [
    "conan",
    "pytest-runner",
]

test_requirements = [
    "pytest",
]

setup(
    author="Jimmy M. Gong",
    author_email="yssource@163.com",
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Natural Language :: English",
        "Programming Language :: Python :: 3.7",
    ],
    description="A&B Quantitive Finance System",
    # py_modules=['abquant'],
    entry_points={"console_scripts": ["abquant=abquant.cli:cli"]},
    install_requires=requirements,
    license="MIT license",
    long_description=readme + "\n\n" + history,
    include_package_data=True,
    keywords="abquant",
    name="abquant",
    packages=find_packages(include=["abquant", "abquant.*"]),
    # package_data={"": ["../../lib/*"]},
    setup_requires=setup_requirements,
    test_suite="tests",
    tests_require=test_requirements,
    url="https://github.com/yssource/abquant",
    version="0.1.0",
    zip_safe=False,
)
