CC=gcc
MPICC=mpicc
CC_FLAGS=-O3 -std=gnu11 -Wall -Wextra -pedantic -lm

.PHONY: all
all: heat_stencil_2D_seq heat_stencil_2D_mpi heat_stencil_2D_mpi_block heat_stencil_1D_mpi heat_stencil_1D_mpi_block


heat_stencil_2D_seq: heat_stencil_2D_seq.c
	@$(CC) $(CC_FLAGS) $< -o heat_stencil_2D_seq

heat_stencil_2D_mpi: heat_stencil_2D_mpi.c
	@$(MPICC) $(CC_FLAGS) $< -o heat_stencil_2D_mpi

heat_stencil_2D_mpi_block: heat_stencil_2D_mpi_block.c
	@$(MPICC) $(CC_FLAGS) $< -o heat_stencil_2D_mpi_block

heat_stencil_1D_mpi_block: heat_stencil_1D_mpi_block.c
	@$(MPICC) $(CC_FLAGS) $< -o heat_stencil_1D_mpi_block

heat_stencil_1D_mpi: heat_stencil_1D_mpi.c
	@$(MPICC) $(CC_FLAGS) $< -o heat_stencil_1D_mpi

.PHONY: clean
clean:
	@rm heat_stencil_2D_seq
	@rm heat_stencil_2D_mpi
	@rm heat_stencil_2D_mpi_block
	@rm heat_stencil_1D_mpi
	@rm heat_stencil_1D_mpi_block

.PHONY: run
run: all
	@echo "Sequential:"
	@./heat_stencil_2D_seq
	@echo
	@echo "Parallel:"
	@mpiexec ./heat_stencil_2D_mpi
	@echo
