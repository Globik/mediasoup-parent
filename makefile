all: main.c
	gcc -O3 -o main main.c `pkg-config --cflags --libs glib-2.0 gio-unix-2.0 gio-2.0`
c: child.c
	gcc -o child child.c `pkg-config --cflags --libs glib-2.0 gio-unix-2.0 gio-2.0`
m: m.c
	gcc -o m m.c `pkg-config --cflags --libs glib-2.0 gio-unix-2.0 gio-2.0`
d: d.c
	gcc -o d d.c `pkg-config --cflags --libs glib-2.0 gio-unix-2.0 gio-2.0`
