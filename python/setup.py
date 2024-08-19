#!/usr/bin/env python

from setuptools import setup, find_packages

setup(
    name="fastmsg",
    version="1.0.1",
    description="Set of minimalistic cross-language serialization libraries for C++ / Go / Python",
    packages=find_packages(),
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
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
