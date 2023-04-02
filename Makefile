CFLAGS = -lsqlite3 -Iinclude
obj = obj/main.o obj/controller.o obj/model.o
run: build
	./bin/main
build: dirs $(obj)
	g++ $(CFLAGS) $(obj) -o bin/main
dirs:
	mkdir -p obj bin
obj/%.o: src/%.cpp
	g++ $(CFLAGS) -c $< -o $@
clean:
	rm bin/* || rm obj/*
