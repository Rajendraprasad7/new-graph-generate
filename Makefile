CC = g++
CFLAGS = -std=c++17

SRCS = main.cxx  
OBJS = $(SRCS:.cxx=.o)
EXEC = a.out

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cxx
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
