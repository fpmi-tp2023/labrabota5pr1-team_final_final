CFLAGS = -lsqlite3 -Iinclude
obj = obj/main.o
run: build
	cd bin && ./main
build: dirs $(obj)
	gcc $(CFLAGS) $(obj) -o bin/main
dirs:
	mkdir -p obj bin
obj/%.o: src/%.c
	gcc -c $< -o $@
clean:
	rm bin/* || rm obj/*
