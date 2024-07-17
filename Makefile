ifndef OUTPUT
.SILENT:
endif

NAME = woody_woodpacker

RETURN42	= "./samples/return42"
HELLO64		=	"./samples/sample"
COMPLEX64	= "./samples/sample_complex"

SRC		= src/error.c src/main.c src/utils.c
HEAD	=	Makefile src/woody.h src/buzz_buzzard.h
OBJ		= $(SRC:.c=.o)

CC			= gcc
CFLAGS	= -Wall -Werror -Wextra -Wfatal-errors
SHELL	= /bin/sh

VAL			=	valgrind --quiet
VALFLAG	=	--tool=memcheck \
			--leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--show-reachable=yes

all:	samples blob $(NAME)

$(NAME): $(OBJ) $(HEAD)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJS): %o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean: clean_woody_bin
	@rm -rf $(OBJ)
	@rm -rf src/buzz_buzzard.o

fclean:	clean
	@rm -rf $(NAME)
	@rm -rf ./src/buzz_buzzard.bin
	@rm -rf ./src/buzz_buzzard.h
	@rm -rf ./src/buzz
	@rm -rf ./.pdf
	@rm -rf ./a ./b
	@cd samples && ./clean.sh

clean_woody_bin:
	@rm -rf woody

re: fclean all

.PHONY: samples
samples:
	cd samples && ./make_samples.sh

blob:
	nasm -f elf64 src/buzz_buzzard.s -o src/buzz_buzzard.o
	objcopy -j.text -O binary src/buzz_buzzard.o src/buzz_buzzard.bin
	xxd -i src/buzz_buzzard.bin > src/buzz_buzzard.h

v:			s
	echo "$(NAME) $(HELLO64)"
	$(VAL) $(VALFLAG) ./$(NAME) $(HELLO64)
	echo "./woody"
	$(VAL) $(VALFLAG) ./woody
g:			s
	gdb -args ./$(NAME) $(HELLO64)

t:			re
	-unit/pass.sh "$(RETURN42)"
s:			re
	-unit/pass.sh "$(HELLO64)"
c:			re
	-unit/pass.sh "$(COMPLEX64)"
l:			re
	-unit/pass.sh "/bin/ls" ".."

dt:			t
	-unit/a_b.sh "$(RETURN42)" ./woody
ds:			s
	-unit/a_b.sh "$(HELLO64)" ./woody
dc:			c
	-unit/a_b.sh "$(COMPLEX64)" ./woody
dl:			l
	-unit/a_b.sh "/bin/ls" ./woody

see:		s
	-./woody
	readelf ./woody -h
	xxd ./woody | grep -A32 1000\:
gg:			s
	gdb -q -x ./unit/gdb.script ./woody
