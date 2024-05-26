NAME	= woody

CFLAGS	= -Wall -Werror -Wextra

SRC		= src/error.c src/main.c src/utils.c 

CC		= gcc

OBJ		= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean:	clean
	@rm -rf $(NAME)

re: clean fclean all

.PHONY: clean fclean re