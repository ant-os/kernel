OSNAME = AntOS
GIT_VERSION = "$(shell git describe --always --tags --long)"
COMPILE_DATETIME := "$(shell date -u +'%x %T')"

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
DEPLOYDIR = ../deploy
LDS = kernel.ld
CC = gcc
LD = ld
ASMC = nasm
STDLIB = antc.a
ANTC_PATH := /home/joscha/projects/AntC/
DRV_MiniFS = /home/joscha/projects/antDriver/MiniFS/bin/MiniFS.drv

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -D__GIT_VERSION__=\"$(GIT_VERSION)\" -D__COMPILE_DATETIME__=\"$(COMPILE_DATETIME)\" -fno-exceptions -isystem $(ANTC_PATH)/include/ -include "ant/stdbase.h"
ASMFLAGS = 
INTFLAGS = -mno-red-zone -mgeneral-regs-only -ffreestanding -fshort-wchar
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib
EMUFLAGS = -soundhw pcspk -machine q35 -drive file=$(BUILDDIR)/$(OSNAME).img -m 1G -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none -name "$(OSNAME)" $(ARGS)
ifdef NOGRAPHIC
ifdef DEBUG
EMUFLAGS += -s -S
endif
EMUFLAGS += -nographic
endif
ifdef SERIAL
EMUFLAGS += -serial $(SERIAL)
#EMUFLAGS += -serial tcp::1234,server,nowait
endif
ifdef NETWORK
EMUFLAGS += -net $(NETWORK)
endif

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
IMAGEDIR = ../images
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

ROOT := ..

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)    
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)


TEXT_START = \033[36;1m
TEXT_END = \033[0m

.SILENT:

kernel: $(OBJS) link

$(OBJDIR)/interrupts/interrupts.o : $(SRCDIR)/interrupts/interrupts.cpp
	@ echo "$(TEXT_START)COMPILING$(TEXT_END)" $^ 
	@ mkdir -p $(@D)
	@ $(CC) -L $(SRCDIR) $(INTFLAGS) -D__FILENAME__="\"$(notdir $(basename $^))\"" -c  $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo "$(TEXT_START)COMPILING$(TEXT_END)" $^ 
	@ mkdir -p $(@D)
	@ $(CC) -L $(SRCDIR) $(CFLAGS) -D__FILENAME__="\"$(notdir $(basename $^))\"" -c $^ -o $@

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@ echo "$(TEXT_START)COMPILING$(TEXT_END)" $^ 
	@ mkdir -p $(@D)
	@ $(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@

link:
	@ echo "$(TEXT_START)LINKING$(TEXT_END)" $(OBJS)
	$(LD) -L$(ANTC_PATH)/bin -l:$(STDLIB) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)
setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

buildimg:
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mformat -v "AntOS$(GIT_VERSION)" -i $(BUILDDIR)/$(OSNAME).img ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/DRIVERS
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-light16.psf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(ROOT)/README.MD ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(DRV_MiniFS) ::

run:	
	@echo -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" 
	qemu-system-x86_64 $(EMUFLAGS)

deploy: clean kernel
	@echo Creating ISO Structure...
	@mkdir $(DEPLOYDIR) -p
	@mkdir $(DEPLOYDIR)/EFI -p
	@mkdir $(DEPLOYDIR)/EFI/BOOT -p
	@echo Copying Files...
	cp $(BOOTEFI) $(DEPLOYDIR)/EFI/BOOT/BOOTX64.EFI
	cp $(BUILDDIR)/kernel.elf  $(DEPLOYDIR)/kernel.elf
	cp $(BUILDDIR)/zap-light16.psf $(DEPLOYDIR)/zap-light16.psf
	cp startup.nsh $(DEPLOYDIR)/startup.nsh
	@echo Creating ISO...
	mkisofs -o $(IMAGEDIR)/AntOS-$(GIT_VERSION).iso $(DEPLOYDIR)
	@echo Cleaning up...
	@rm -rf $(DEPLOYDIR)

clean:
	@rm $(OBJDIR) -r -f