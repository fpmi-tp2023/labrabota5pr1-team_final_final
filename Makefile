CFLAGS = -lsqlite3 -Iinclude -lssl -lcrypto
TESTFLAGS = -lsqlite3 -lgtest -lgtest_main -lssl -Iinclude -lcrypto
CC = g++
main_obj = obj/main.o
obj = obj/controller.o obj/model.o
test_obj = obj/db_test.o
run: build
	./bin/main
test: build_test
	./bin/test_main
build_test: dirs $(test_obj) $(obj)
	$(CC) $(obj) $(test_obj) -o bin/test_main $(TESTFLAGS)
build: dirs $(obj) $(main_obj)
	$(CC) $(main_obj) $(obj) -o bin/main $(CFLAGS)
dirs:
	mkdir -p obj bin
obj/%.o: test/%.cc
	$(CC) -c $< -o $@ $(TESTFLAGS)
obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
clean:
	rm bin/* obj/*
