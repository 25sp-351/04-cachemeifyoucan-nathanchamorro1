CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g

SRC = main.c rod_cut.c lru_cache.c rr_cache.c
OBJ = $(SRC:.c=.o)

DEPS = cache.h rod_cut.h

TARGET = rod_cut

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)