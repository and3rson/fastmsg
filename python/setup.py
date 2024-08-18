#!/usr/bin/env python

from setuptools import setup, find_packages

setup(
    name="fastmsg",
    version="0.1.0",
    description="Set of minimalistic cross-language serialization libraries for C++ / Go / Python",
    packages=find_packages(),
    install_requires=[
        # 'six',
    ],
    extras_require={
        "dev": [
            "black",
            "isort",
            "pytest",
            "pytest-cov",
            "radon",
            "xenon",
        ],
    },
    python_requires=">=3.8",
)
