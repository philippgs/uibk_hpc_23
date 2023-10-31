CC=mpicc
CC_FLAGS=-std=gnu11

.PHONY: all
all: example_1 example_2

example_1: example_1.c
	@$(CC) $(CC_FLAGS) $< -o example_1

example_2: example_2.c
	@$(CC) $(CC_FLAGS) $< -o example_2

.PHONY: clean
clean:
	@rm example_1 example_2
