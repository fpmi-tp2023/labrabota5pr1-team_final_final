INCLUDE = -Iinclude
LIB = -lsqlite3 -lssl -lcrypto
LIBTEST = -lgtest -lgtest_main -lgcov
COVERAGE = -fprofile-arcs -ftest-coverage
CC = g++
test_obj = obj/db_test.o obj/tcontroller.o obj/tmodel.o
obj = obj/main.o obj/controller.o obj/model.o
CPPFLAGS = $(INCLUDE) $(LIB)
TESTFLAGS = $(INCLUDE) $(LIB) $(LIBTEST)
EXEC = bin/main
TESTEXEC = bin/test_main
.PHONY: dirs clean run build test report build_test
run: dirs $(EXEC)
	./$(EXEC)
build: dirs $(EXEC)
report: test
	lcov -c --directory obj --output-file coverage.info
	genhtml coverage.info -o cov
	rm coverage.info
	firefox cov/index.html
test: dirs $(TESTEXEC)
	./$(TESTEXEC)
build_test: dirs $(TESTEXEC)
$(TESTEXEC): $(test_obj)
	$(CC) $(test_obj) -o bin/test_main  $(TESTFLAGS)
obj/%.o: test/%.cc
	$(CC) -c $< -o $@ $(TESTFLAGS)
obj/t%.o: src/%.cpp
	$(CC) $(COVERAGE) -c $< -o $@ $(INCLUDE)
$(EXEC): $(obj)
	$(CC) $(obj) -o bin/main $(CPPFLAGS)
obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(INCLUDE)
dirs:
	-mkdir -p obj bin
clean:
	-rm bin/* 
	-rm obj/*
	-rm -r cov
