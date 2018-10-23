#NAME: Marvin Cirignano

.SILENT:

src_files = main.c Termios.c Textline.c Document.c View.c Display.c Constants.c Editor.c

default: main.c 
	gcc -Wall -Wextra -o niv $(src_files)


test: default
	./niv
	rm niv

clean:
	rm niv
