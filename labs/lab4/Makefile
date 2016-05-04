OBJ	= main.o llmultiplyasm.o
OBJ2	= test1.o llmultiplyasm.o
OBJ3	= test1.o llmultiplyc.o
OBJ4	= main.o llmultiplyasmosc.o
OBJ5	= test1.o llmultiplyasmosc.o
OBJ6	= test1.o llmultiplycosc.o

CFLAGS = -O0

COURSE   = /home/TDDI11/sw
LDSCRIPT = $(COURSE)/lib/link.cmd
BOOTSECT = $(COURSE)/lib/bootload.bin
INCLUDES = -I "$(COURSE)/include" -I "$(COURSE)/include/djgpp" $(EXTRA_INCLUDES)
CC       = /sw/i386-elf-gcc-4.9.0/bin/i386-elf-gcc-4.9.0
CFLAGS  += -Wall -g $(INCLUDES)
LD       = /sw/i386-elf-gcc-4.9.0/bin/i386-elf-ld
LDFLAGS  = -T$(LDSCRIPT) -static -ustart -Map link.map
AS       = nasm
ASFLAGS  = -f elf32

embedded.bin: $(OBJ)
	$(LD) $(OBJ) $(LIBS) $(LDFLAGS)

embedded2.bin: $(OBJ2)
	$(LD) $(OBJ2) $(LIBS) $(LDFLAGS)

embedded3.bin: $(OBJ3)
	$(LD) $(OBJ3) $(LIBS) $(LDFLAGS)

embedded4.bin: $(OBJ4)
	$(LD) $(OBJ4) $(LIBS) $(LDFLAGS)

embedded5.bin: $(OBJ5)
	$(LD) $(OBJ5) $(LIBS) $(LDFLAGS)

embedded6.bin: $(OBJ6)
	$(LD) $(OBJ6) $(LIBS) $(LDFLAGS)

%.o: %.asm
	$(AS) $(ASFLAGS) $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	\rm -f embedded.bin *~ *.o link.map
