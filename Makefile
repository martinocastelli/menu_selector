CC = gcc
CFLAGS = -Wall -c -g
LFLAGS = -Wall
SOURCES = main.c tc/tc.c
OBJECT = main.o tc.o
TAGS = tags tc/tags
EXE = menu.out

$(EXE): $(OBJECT)
	$(CC) -o $@ $(LFLAGS) $^

main.o: main.c tc/tc.h
	$(CC) -o $@ $(CFLAGS) main.c

tc.o: tc/tc.h tc/tc.c
	$(CC) -o $@ $(CFLAGS) tc/tc.c

clean:
	rm -f $(OBJECT) $(TAGS) $(EXE)

all: clean $(OBJECT) $(EXE)

clean_build: all

build_clean: $(OBJECT) $(EXE)
	rm -f $(OBJECT) $(TAGS)
