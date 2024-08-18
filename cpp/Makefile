SRC = $(wildcard *.cpp) $(wildcard tests/*.cpp)
DEPS = $(wildcard *.h)
TARGET = run_tests

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	g++ -I. $(SRC) -o run_tests

compiledb:
	compiledb make
