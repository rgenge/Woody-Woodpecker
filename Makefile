ifndef OUTPUT
.SILENT:
endif

# Besides usual all, clean, fclean, re, use:
# v  : valgrind
# vf : valgrind full flags
# g  : gdb
# samples : compiles samples

NAME		= woody_woodpacker

ARGS32	=	"samples/return32"
ARGS64	= "samples/return64"
ARGSS64	=	"samples/sample64"
ARGSS32	=	"samples/sample32"
ARGSA32 = "samples/A32"
ARGSB64 = "samples/B64"
ARGSB32 = "samples/B32"
ARGSA64 = "samples/A64"

ARGMAIN	=	"samples/sample"

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
	$(VAL) ./$(NAME) $(ARGS32)
vf:			all
	$(VAL) $(VALFLAG) ./$(NAME) $(ARGS32)
g:			all
	gdb -args ./$(NAME) $(ARGS32)

t:			all
	-unit/pass.sh "$(ARGS32)"
tt:			all
	-unit/pass.sh "$(ARGS64)"
s:			all
	-unit/pass.sh "$(ARGSS32)"
ss:			all
	-unit/pass.sh "$(ARGSS64)"

rv:			re v
rvf:		re vf
rg:			re g
rt:			re t
