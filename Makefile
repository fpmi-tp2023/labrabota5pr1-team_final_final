CFLAGS = -lsqlite3 -Iinclude
obj = obj/main.o obj/controller.o obj/model.o
run: build
	./bin/main
build: dirs $(obj)
	g++ $(obj) -o bin/main $(CFLAGS)
dirs:
	mkdir -p obj bin
obj/%.o: src/%.cpp
	g++ -c $< -o $@ $(CFLAGS)
clean:
	rm bin/* || rm obj/*
