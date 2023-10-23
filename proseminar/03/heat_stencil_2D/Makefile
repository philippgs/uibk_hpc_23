CC=gcc
MPICC=mpicc
CC_FLAGS=-O3 -std=gnu11 -Wall -Wextra -pedantic -lm
BUILD=build/

pre:
	mkdir build

.PHONY: all
all: pre heat_stencil_2D_seq heat_stencil_2D_mpi heat_stencil_2D_mpi_block heat_stencil_1D_mpi heat_stencil_1D_mpi_block


heat_stencil_2D_seq: heat_stencil_2D_seq.c
	@$(CC) $(CC_FLAGS) $< -o $(BUILD)heat_stencil_2D_seq

heat_stencil_2D_mpi: heat_stencil_2D_mpi.c
	@$(MPICC) $(CC_FLAGS) $< -o $(BUILD)heat_stencil_2D_mpi

heat_stencil_2D_mpi_block: heat_stencil_2D_mpi_block.c
	@$(MPICC) $(CC_FLAGS) $< -o $(BUILD)heat_stencil_2D_mpi_block

heat_stencil_1D_mpi_block: heat_stencil_1D_mpi_block.c
	@$(MPICC) $(CC_FLAGS) $< -o $(BUILD)heat_stencil_1D_mpi_block

heat_stencil_1D_mpi: heat_stencil_1D_mpi.c
	@$(MPICC) $(CC_FLAGS) $< -o $(BUILD)heat_stencil_1D_mpi

.PHONY: clean
clean:
	rm -r $(BUILD)

.PHONY: run
run: all
	@echo "Sequential:"
	@./$(BUILD)heat_stencil_2D_seq
	@echo
	@echo "Parallel:"
	@mpiexec ./$(BUILD)heat_stencil_2D_mpi
	@echo
