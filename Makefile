CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
SRC = 	src/minishell.c \
		src/init.c \
		src/lexer.c 	 \
		src/lexer_utils.c \
		src/lexer_utils_2.c \
		src/split.c \
		src/utils.c \
		src/utils_2.c \
		src/here_doc.c \
		src/cleanup.c \
		src/get_env.c \
		src/linked_list_helper.c \
		builtins/env.c \
		builtins/pwd.c \
		builtins/unset.c \
		src/env_to_envp.c \

OBJ = $(SRC:.c=.o)
RM = rm -rf
LIBFT = ./libft/libft.a
LIBFT_DIR = libft
NAME = minishell

all : $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR) 

$(NAME) : $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re

.SECONDARY: $(OBJ) $(LIB)