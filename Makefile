CFLAGS = -std=gnu++14 -Os
all:
	g++ main.cpp -o calculator $(CFLAGS)

debug:
	g++ main.cpp -o calculator $(CFLAGS) -DDEBUG

clean:
	rm -f calculator
