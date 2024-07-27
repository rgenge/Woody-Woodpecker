ifndef OUTPUT
#.SILENT:
endif

NAME = woody_woodpacker

RETURN42 = "./samples/return42"
HELLO64 = "./samples/sample"
COMPLEX64 = "./samples/sample_complex"

SRC = src/error.c src/main.c src/utils.c
HEAD = src/woody.h src/buzz_buzzard.h
OBJ = $(SRC:.c=.o)

ASM_SRC = src/buzz_buzzard.s
ASM_OBJ = src/buzz_buzzard.o
ASM_BIN = src/buzz_buzzard.bin
ASM_HDR = src/buzz_buzzard.h

CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wfatal-errors
SHELL = /bin/sh

VAL = valgrind --quiet
VALFLAG = --tool=memcheck \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --show-reachable=yes

all: $(NAME)

$(NAME): $(OBJ) $(ASM_HDR)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) -c $< -o $@

$(ASM_OBJ): $(ASM_SRC)
	nasm -f elf64 $(ASM_SRC) -o $(ASM_OBJ)

$(ASM_BIN): $(ASM_OBJ)
	objcopy -j .text -O binary $(ASM_OBJ) $(ASM_BIN)

$(ASM_HDR): $(ASM_BIN)
	xxd -i $(ASM_BIN) > $(ASM_HDR)

clean:
	@rm -f $(OBJ)
	@rm -f $(ASM_OBJ)
	@rm -f $(ASM_BIN)
	@rm -f $(ASM_HDR)

fclean: clean
	@rm -f $(NAME)
	@rm -rf ./src/buzz_buzzard.bin
	@rm -rf ./src/buzz_buzzard.h
	@rm -rf ./src/buzz
	@rm -rf ./.pdf
	@rm -rf ./a ./b
	@cd samples && ./clean.sh
	@rm -f woody


re: fclean all

.PHONY: all clean fclean re samples blob

samples:
	cd samples && ./make_samples.sh

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
