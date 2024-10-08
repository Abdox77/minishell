CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 
SRC = 	src/minishell.c \
		src/init.c \
		src/lexer.c 	 \
		src/parenthesis_lexer.c 	 \
		src/pipe_lexer.c 	 \
		src/operator_lexer.c 	 \
		src/parse_cmd.c \
		src/parse_cmd_2.c \
		src/parse_cmd_3.c \
		src/parse_cmd_4.c \
		src/parse_cmd_utils.c \
		src/parse_cmd_utils_2.c \
		src/redir_parser.c \
		src/redir_parser_utils.c \
		src/redir_parser_utils_2.c \
		src/utils_3.c \
		src/split.c \
		src/utils.c \
		src/utils_4.c \
		src/utils_2.c \
		src/here_doc.c \
		src/expand_in_heredoc.c\
		src/here_doc_utils.c\
		src/here_doc_utils_2.c\
		src/cleanup.c \
		src/cleanup_2.c \
		src/get_env.c \
		src/get_env_help.c \
		src/signals.c \
		src/signals_2.c \
		src/linked_list_helper.c \
		builtins/env.c \
		builtins/pwd.c \
		builtins/unset.c \
		builtins/exit.c \
		src/env_to_envp.c \
		builtins/cd.c \
		src/execution.c \
		src/execution_utils.c \
		src/status.c \
		src/redirection.c \
		src/redirection_helper.c \
		src/syntax_evaluation.c \
		builtins/check_builtin.c \
		builtins/echo.c \
		src/expander.c \
		builtins/export.c \
		builtins/export_utils.c \
		builtins/print_export.c \
		src/execution_itoa.c \
		src/expand_string.c \
		src/expand_variable.c \
		src/expand_helper.c \
		src/expand_length.c \
		src/expand_utils.c \
		src/check_expand.c \
		src/execute_command.c \
		src/execute_command_utils.c \
		src/execute_command_utils_2.c \
		src/execute_subtree.c \
		src/check_expand_utils.c \
		src/execute_pipe.c \
		src/append_var_value.c \
		src/wildcards.c \
		src/wildcards_helper.c \
		src/wildcards_helper_2.c \
		gnl/gnl.c \
		gnl/gnl_utils.c \

OBJ = $(SRC:.c=.o)
RM = rm -rf
LIBFT = ./libft/libft.a
LIBFT_DIR = libft
NAME = minishell

ifeq ($(DEBUG), 1)
	CFLAGS += -fsanitize=address
endif

all : $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR) 

$(NAME) : $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)
	$(RM) $(OBJ)

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re

.SECONDARY: $(OBJ) $(LIB)
