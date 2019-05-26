INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: bkpctl

bkpctl: bkpctl.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi bkpctl.c -o bkpctl

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f bkpctl
