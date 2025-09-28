COMP = g++ main.cpp src/glad.c -o a.elf -lglfw -lfreetype -I/usr/include/freetype2/ -I/usr/include/stb/

a.elf: main.cpp
	$(COMP)

.PHONY = ct c

ct: a.elf
	./a.elf
	rm a.elf

c:
	rm a.elf
