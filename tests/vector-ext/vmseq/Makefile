CC		:=	riscv32-unknown-elf-gcc
AS		:=	riscv32-unknown-elf-as
OBJDUMP :=  riscv32-unknown-elf-objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data

TARGET	:= vmseq
GCC_OPTS = -march=rv32imafdv
LINK_OPTS = -nostartfiles -lc -lm
LIB_DIR	:=	-L ../../libac_sysc
LIBS	:=	-lc -lac_sysc
HAL		:=	../../rv_hal/get_id.S
TMP		:= $(TARGET).o
SRCS	:=

all:	$(TARGET).c
	$(CC) -c ../../rv_hal/crt.S -march=rv32imafdv
	$(AS) -march=rv32imafdv -o $(TARGET).o $(TARGET).s
	$(CC) $(TARGET).c -o $(TARGET).run $(SRCS) $(HAL) $(LIB_DIR) $(LIBS) -T ../../rv_hal/test.ld $(GCC_OPTS) $(LINK_OPTS) $(TMP)
	$(OBJDUMP) $(TARGET).run > $(TARGET).out

clean:
	rm $(TARGET).run crt.o $(TARGET).out $(TARGET).o
