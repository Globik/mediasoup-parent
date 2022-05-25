all: main.c
	gcc -O3 -o main main.c `pkg-config --cflags --libs glib-2.0 gio-unix-2.0 gio-2.0`
c: child.c
	gcc -o child child.c `pkg-config --cflags --libs glib-2.0 gio-unix-2.0 gio-2.0`
m: m.c
	gcc -o m m.c `pkg-config --cflags --libs glib-2.0 gio-unix-2.0 gio-2.0`
d: d.c
	gcc -o d d.c `pkg-config --cflags --libs glib-2.0 gio-unix-2.0 gio-2.0`
t: t.c
	gcc -o t t.c
f: f.c
	gcc -o f f.c
u: u.c
	gcc -o u u.c `pkg-config --cflags --libs libuv`
l: child2.c
	gcc -o l child2.c `pkg-config --cflags --libs libuv`
fi: fuck.c
	gcc -o fi fuck.c `pkg-config --cflags --libs libuv`
b: buf.c
	gcc -o b buf.c `pkg-config --cflags --libs libuv`
re: re.c
	gcc -o re re.c 
hex: hex.c
	gcc -o hex hex.c
