ifndef OUTPUT
.SILENT:
endif

NAME	= woody_woodpacker
ARGS	=	sample

CFLAGS	= -Wall -Werror -Wextra -g -Wfatal-errors

SRC		= src/error.c src/main.c src/utils.c src/pretty_print.c
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
	$(VAL) ./$(NAME) $(ARGS)
vf:			all
	$(VAL) $(VALFLAG) ./$(NAME) $(ARGS)
g:			all
	gdb -args ./$(NAME) $(ARGS)
t:			all
	-unit/pass.sh
p:			all
	cd playground && ./tiny.sh

rv:			re v
rvf:		re vf
rg:			re g
rt:			re t
