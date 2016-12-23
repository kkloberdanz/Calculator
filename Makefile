CFLAGS = -std=gnu++14
all:
	g++ main.cpp -o calculator $(CFLAGS)

debug:
	g++ main.cpp -o calculator $(CFLAGS) -DDEBUG
