ifndef OUTPUT
.SILENT:
endif

# Besides usual all, clean, fclean, re, use:
# v  : valgrind
# vf : valgrind full flags
# g  : gdb
# samples : compiles samples

NAME		= woody_woodpacker

RETURN64= "samples/return64"
HELLO64	=	"samples/sample64"

CFLAGS	= -Wall -Werror -Wextra -g -Wfatal-errors

SRC		= src/error.c src/main.c src/utils.c
HEAD	=	Makefile src/woody.h

CC		= gcc

OBJ		= $(SRC:.c=.o)

SHELL	= /bin/sh

VAL		=	valgrind

VALFLAG	=	--tool=memcheck \
			--leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--show-reachable=yes

all:	samples $(NAME)

$(NAME): $(OBJ) $(HEAD)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

$(OBJS): %o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf $(OBJ)

fclean:	clean
	@rm -rf $(NAME)
	@cd samples && ./clean.sh

re: fclean all

.PHONY: samples
samples:
	cd samples && ./tiny.sh

v:			all
	$(VAL) ./$(NAME) $(HELLO64)
vf:			all
	$(VAL) $(VALFLAG) ./$(NAME) $(HELLO64)
g:			all
	gdb -args ./$(NAME) $(HELLO64)

t:			all
	-unit/pass.sh "$(RETURN64)"
s:			all
	-unit/pass.sh "$(HELLO64)"

rv:			re v
rvf:		re vf
rg:			re g
rt:			re t
