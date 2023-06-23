ANTC_PATH = /home/joscha/projects/AntC/
CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -fno-exceptions -isystem $(ANTC_PATH)/include/

SRCS = $(wildcard *.c)
OBJS = $(addprefix lib/,$(notdir $(SRCS:.c=.o)))

all: $(OBJS)

lib/%.o: %.c
	mkdir -p lib
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf lib
