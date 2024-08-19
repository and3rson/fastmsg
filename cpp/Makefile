SRC = $(wildcard *.cpp) $(wildcard tests/*.cpp)
DEPS = $(wildcard *.h)
TARGET = run_tests

test: $(TARGET)
	./$(TARGET)
	cppcheck . \
		-i .ccls-cache \
		--enable=performance,style \
		--check-level=exhaustive \
		--error-exitcode=1

$(TARGET): $(SRC) $(HEADERS)
	g++ -I. $(SRC) -o run_tests

compiledb:
	compiledb make
