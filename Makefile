OBJS = main.cpp
CC = g++

LDFLAGS = -lSDL2main -lSDL2 -lSDL2_ttf

youdied: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o youdied

.PHONY: clean
clean:
	rm -rf *.o

.PHONY: distclean
distclean:
	rm -rf *.o
	rm -rf youdied

.PHONY: install
install:
	cp youdied /usr/local/bin/youdied

.PHONY: uninstall
uninstall:
	rm -rf /usr/local/bin/youdied
