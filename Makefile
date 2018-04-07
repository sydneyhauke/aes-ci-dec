CC=gcc

CFLAGS= -O2 -std=c11 -Wall -I$(INCL_FOLDER)/
LDFLAGS=-I$(INCL_FOLDER)/

SRC_FOLDER=src
INCL_FOLDER=include
BUILD_FOLDER=build

SOURCES=$(wildcard $(SRC_FOLDER)/*.c)
OBJS = $(SOURCES:%.c=%.o)

EXE_NAME=aes-cidec

all: $(BUILD_FOLDER)/$(EXE_NAME)

$(BUILD_FOLDER)/$(EXE_NAME): $(OBJS)
	$(CC) -o $(BUILD_FOLDER)/$(EXE_NAME) $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.phony: clean mrproper

clean:
	rm -f $(SRC_FOLDER)/*.o

mrproper : clean
	rm -f $(BUILD_FOLDER)/$(EXE_NAME)
