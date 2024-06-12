ifndef OUTPUT
.SILENT:
endif

NAME		= woody_woodpacker
ARGS32	=	"samples/return32"
ARGS64	= "samples/return64"

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

all:	$(NAME)

$(NAME): $(OBJ) $(HEAD)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

$(OBJS): %o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf $(OBJ)

fclean:	clean
	@rm -rf $(NAME)

re: fclean all

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

rv:			re v
rvf:		re vf
rg:			re g
rt:			re t
