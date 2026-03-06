# Default benchmark (can be overridden from root)
BENCH ?= cubic

# CPU frequency (for scaling)
CPU_MHZ ?= 2

# Collect benchmark source files automatically
BENCH_SRC := $(wildcard $(BENCH)/*.c)

# Generate disassembly
core.dump: core.elf
	riscv32-unknown-elf-objdump -D ../core.elf > ../core.dump

# Build ELF
core.elf:
	riscv32-unknown-elf-gcc \
	-O2 \
	-march=rv32im \
	-mabi=ilp32 \
	-nostartfiles \
	-T link.ld \
	-I. \
	-DCPU_MHZ=$(CPU_MHZ) \
	-fno-common \
	-fno-builtin \
	-fno-tree-vectorize \
	start.s \
	main.c \
	beebsc.c \
	$(BENCH_SRC) \
	-lc -lgcc -lm \
	-o ../core.elf