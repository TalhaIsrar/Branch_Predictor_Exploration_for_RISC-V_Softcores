# Convert .elf file to .dump for easy debugging
core.dump: core.elf
	riscv32-unknown-elf-objdump -D ../core.elf > ../core.dump

# Use GCC to generate .elf file
core.elf: link.ld start.s main.c libslre.c
	riscv32-unknown-elf-gcc -O2 -march=rv32im -mabi=ilp32 -nostartfiles -T link.ld start.s main.c libslre.c beebsc.c -lc -lgcc -lm -o ../core.elf