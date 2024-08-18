PYTHON = python3

test:
	$(PYTHON) -m unittest fastmsg.test_message --verbose

.PHONY: build
build:
	rm -rf dist
	$(PYTHON) -m build

.PHONY: upload
upload: build
	twine upload dist/*
