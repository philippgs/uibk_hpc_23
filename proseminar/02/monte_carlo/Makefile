CC=gcc
MPICC=mpicc
CC_FLAGS=-O3 -std=gnu11 -Wall

.PHONY: all
all: pi_seq pi_mpi

pi_seq: pi_seq.c
	@$(CC) $(CC_FLAGS) $< -o pi_seq

pi_mpi: pi_mpi.c
	@$(MPICC) $(CC_FLAGS) $< -o pi_mpi

.PHONY: clean
clean:
	@rm pi_seq
	@rm pi_mpi

.PHONY: run
run: all
	@echo "Sequential:"
	@./pi_seq $(ARGS)
	@echo
	@echo "Parallel:"
	@mpiexec ./pi_mpi $(ARGS)
	@echo
