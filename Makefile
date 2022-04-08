CC = gcc
CFLAGS = -Wall -O3

LIBS = -lm
EXEC = imageprocessing
TEST = test
OBJS = processing.o imageio.o
INCS = processing.h imageio.h

all: $(EXEC) $(TEST)

debug: CFLAGS += -DDEBUG -g
debug: all

profile: PROFILE += -pg
profile: $(EXEC)

$(EXEC): main.o $(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(PROFILE)

$(TEST): unittests.o $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c $(INCS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf main.o unittests.o *.out *dSYM $(OBJS) $(EXEC) $(TEST) *.~ *.mtx *.dSYM
