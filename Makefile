INCLUDE = -Iinclude
LIB = -lsqlite3 -lssl -lcrypto
LIBTEST = -lgtest -lgtest_main -lgcov
COVERAGE = -fprofile-arcs -ftest-coverage
CC = g++
test_obj = obj/db_test.o obj/tcontroller.o obj/tmodel.o
obj = obj/main.o obj/controller.o obj/model.o
FLAGS = -std=c++17
CPPFLAGS = $(FLAGS) $(INCLUDE)
TESTFLAGS = $(COVERAGE) $(FLAGS) $(INCLUDE)
EXEC = bin/main
TESTEXEC = bin/test_main
.PHONY: dirs clean run build test report build_test
run: dirs $(EXEC)
	./$(EXEC)
build: dirs $(EXEC)
report: test
	lcov -c --directory src --directory obj --output-file coverage.info --no-external
	genhtml coverage.info -o cov
	rm coverage.info
	open cov/index.html
test: dirs $(TESTEXEC)
	./$(TESTEXEC)
build_test: dirs $(TESTEXEC)
$(TESTEXEC): $(test_obj)
	$(CC) $(TESTFLAGS) $(test_obj) -o bin/test_main  $(LIBTEST) $(LIB)
obj/%.o: test/%.cc
	$(CC) $(TESTFLAGS) -c $< -o $@
obj/t%.o: src/%.cpp
	$(CC) $(TESTFLAGS) -c $< -o $@ 
$(EXEC): $(obj)
	$(CC) $(CPPFLAGS) $(obj) -o bin/main $(LIB)
obj/%.o: src/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
dirs:
	-mkdir -p obj bin
clean:
	-rm bin/* 
	-rm obj/*
	-rm -r cov
