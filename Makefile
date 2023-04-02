CFLAGS = -lsqlite3 -Iinclude
obj = obj/main.o
run: build
	cd bin && ./main
build: dirs $(obj)
	g++ $(CFLAGS) $(obj) -o bin/main
dirs:
	mkdir -p obj bin
obj/%.o: src/%.cpp
	g++ -c $< -o $@
clean:
	rm bin/* || rm obj/*
