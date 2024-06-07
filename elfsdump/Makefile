ifndef OUTPUT
.SILENT:
endif

NAME	=	elfsdump
ARGS	=	../sample
SRCS	=	main.c utils.c
HEAD	=	Makefile elfsdump.h
SHELL	=	/bin/sh
CC		=	gcc
CCFLAGS	=	-Wall -Werror -Wextra -Wfatal-errors -g
OBJS	=	$(SRCS:.c=.o)
VAL		=	valgrind
VALFLAG	=	--tool=memcheck \
			--leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--show-reachable=yes
all:		$(NAME)
$(NAME):	$(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(NAME)
$(OBJS):	%.o : %.c $(HEAD)
	$(CC) $(CCFLAGS) -o $@ -c $<
clean:
	-rm -f $(OBJS)
fclean:		clean
	-rm -f $(NAME)
re:			fclean all
v:			all
	$(VAL) ./$(NAME) $(ARGS)
vf:			all
	$(VAL) $(VALFLAG) ./$(NAME) $(ARGS)
g:			all
	gdb ./$(NAME)
t:			all
	./$(NAME) $(ARGS)
rv:			re v
rvf:		re vf
rg:			re g
rt:			re t
