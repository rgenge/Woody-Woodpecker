ifndef OUTPUT
.SILENT:
endif

# Besides usual all, clean, fclean, re, use:
# v  : valgrind
# vf : valgrind full flags
# g  : gdb
# samples : compiles samples

NAME		= woody_woodpacker

RETURN42= "./samples/return42"
HELLO64	=	"./samples/sample"
COMPLEX64 = "./samples/sample_complex"

CFLAGS	= -Wall -Werror -Wextra -Wfatal-errors

SRC		= src/error.c src/main.c src/utils.c
HEAD	=	Makefile src/woody.h

CC		= gcc -g -fsanitize=address

OBJ		= $(SRC:.c=.o)

SHELL	= /bin/sh

VAL		=	valgrind

VALFLAG	=	--tool=memcheck \
			--leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--show-reachable=yes

all:	samples blob $(NAME)

$(NAME): $(OBJ) $(HEAD)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

$(OBJS): %o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean: clean_woody_bin
	@rm -rf $(OBJ)
	@rm -rf src/WOODYcode.o

fclean:	clean
	@rm -rf $(NAME)
	@cd samples && ./clean.sh
	@rm -rf WOODY_blob.bin

clean_woody_bin:
	@rm -rf woody

re: fclean all

.PHONY: samples
samples:
	cd samples && ./tiny.sh

blob:
	cd src && ./make_WOODY_blob.sh

v:			re
	$(VAL) ./$(NAME) $(HELLO64)
vf:			re
	$(VAL) $(VALFLAG) ./$(NAME) $(HELLO64)
g:			re
	gdb -args ./$(NAME) $(HELLO64)

t:			re
	-unit/pass.sh "$(RETURN42)"
s:			re
	-unit/pass.sh "$(HELLO64)"
c:			re
	-unit/pass.sh "$(COMPLEX64)"
l:			re
	-unit/pass.sh "/bin/ls"
