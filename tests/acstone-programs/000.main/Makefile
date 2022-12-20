CC		:= riscv32-unknown-linux-gnu-gcc
OBJDUMP := riscv32-unknown-linux-gnu-objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data

TARGET	:= 000.main
GCC_OPTS = -Wa,-march=rv32imafd -std=gnu99
LINK_OPTS = -nostartfiles -lc -lm
LIB_DIR	:=	-L../../libac_sysc
LIBS	:=	-lc -lac_sysc
HAL		:=	../../rv_hal/get_id.S
SRCS	:=

all:	$(TARGET).c
	$(CC) -c ../../rv_hal/crt.S -Wa,-march=rv32imafd -std=gnu99
	$(CC) $(TARGET).c -o $(TARGET).run $(SRCS) $(HAL) $(LIB_DIR) $(LIBS) -T ../../rv_hal/test.ld $(GCC_OPTS) $(LINK_OPTS)
	$(OBJDUMP) $(TARGET).run > $(TARGET).out

clean:
	rm $(TARGET).run crt.o $(TARGET).out
