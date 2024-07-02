NAME	= woody_wood

CFLAGS	= -Wall -Werror -Wextra

SRC		= src/error.c src/main.c src/utils.c 

CC		= gcc

INJECT = src/inject.s

OBJ		= $(SRC:.c=.o) $(INJECT:%.s=%.o)

all: $(NAME)

%.o: %.s
	nasm -f elf64 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) 
	
src/g_decryptor.c:
	nasm -f bin src/decryptor.s -o src/g_decryptor
	cd src && xxd -i -c 8 g_decryptor g_decryptor.c	

$(NAME): $(OBJ) src/g_decryptor.o
	$(CC) $(CFLAGS) $(OBJ) src/g_decryptor.o -o $(NAME)

clean:
	@rm -rf $(OBJ)
	@rm -rf woody

fclean:	clean
	@rm -rf $(NAME)

re: clean fclean all

.PHONY: clean fclean re