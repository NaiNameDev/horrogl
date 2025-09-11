COMP = g++ main.cpp src/glad.c -o a.elf -lglfw

a.elf: main.cpp
	$(COMP)

.PHONY = ct c

ct: a.elf
	./a.elf
	rm a.elf

c:
	rm a.elf
