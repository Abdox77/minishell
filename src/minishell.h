/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:45 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 18:41:10 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "get_next_line.h"
# include <termios.h>
#include <sys/stat.h>
#ifdef __linux__
    # include <linux/limits.h>
#endif

/*==========================MACROS============================*/

# define PURPLE "\033[0;35m"
# define GREEN "\033[36m"
# define HEREDOC_LOCATION "/var/tmp/"
# define YELLOW "\033[33m"
# define RESET_COLORS "\033[0m"
// # define BUFFER_SIZE 1000
# define R_HEREDOC 0
# define W_HEREDOC 1
# define _HEREDOC_EXPAND_FD 2
# define FNAME "_MINISHELL_HEREDOC__FILE_NAMING_"
// # define BUFFER_SIZE 42
/*===========================enum=========================*/

typedef enum {
    _SAVE,
    _RETRIEVE,
} t_sigops;

typedef enum
{
    _NO_STATE_, 
    SET_SIGS,   
    UNSET_SIGS,
} t_sigstate;

typedef enum{
    PRINT,
    SAVE, 
    NOT_PRINTED,
    RETRIEVE,
    RESET,
    RESET_HEREDOC ,
}   t_error;

typedef enum {
    FAILED,
    PASSED,
} t_eval;

typedef enum {
    FALSE,
    TRUE,
}   t_bool;

typedef enum 
{
    INFILE,
    OUTFILE,
    HEREDOC,
    APPEND ,
    TRUNC,
    STD_OUT
}   REDIR_MODE;

typedef enum
{
    INPUT,
    OUTPUT,
} REDIR_TYPE;

typedef enum
{
    AND,
    OR,
    NONE,
    CMD,
    PARENTHESIS,
    PIPE,
}   TOKEN;

/*=======================TYPEDEFS=========================*/

typedef struct s_token      t_token;
typedef struct s_cmd        t_cmd;
typedef struct s_redir      t_redir;
typedef struct s_env        t_env;
typedef struct s_env_var    t_var;
typedef struct s_exec       t_exec;
typedef struct s_expands    t_expands;
typedef struct s_lvars      t_lvars;
typedef struct s_iter_vars  t_iter_vars;



/*=======================STRUCTS============================*/

struct s_iter_vars{
    int i;
    int len;  
};

struct s_lvars {
    int len;
    int og_len;
};

struct s_expands {
    char        **og_args;
    char        *og_cmd;
    t_redir     *og_redir;
    // t_redir     *og_output;
};

struct s_token {
    TOKEN       type;
    t_token     *r_token;
    t_token     *l_token;
    t_redir     *redir;
    t_redir     *og_redir;
    t_redir     *input;
    // t_redir     *og_input;
    t_cmd       *cmd;
} ;

struct s_redir {
    int         fd;
    int         here_doc_fd[3];
    t_bool      to_be_expanded;
    char        *file_name;
    t_redir     *next; // later
    REDIR_MODE  mode;
};

struct s_cmd
{
    char        *cmd;
    char        **args;
    char        *cmd_to_be_expanded;
    t_expands   *og_tokens; // og stands for original
    t_redir     *redir;
    // t_redir     *output;
};


struct s_exec
{
	t_env	*env;
	char	*tmp;
	char	**envp;
	int		o_in;
	int		o_out;
    int     in;
    int     out;
    char    **to_free;
};


struct s_env
{
	char			*key;
	char			*value;
    // char            *oper;
	t_env	*next;
};

struct s_env_var {
    char *tmp_key;
    char *tmp_val;
    int i;
    int j;
    int rem;
} ;

struct s_exec_var {
    char **tmp_key;
    char **nullo;
    char ** pro;
    int j;
    int rem;
} ;

/*=====================INIT=====================*/
t_token     *init_token(void);
t_token     *set_pipe_head(void);
t_token     *new_token(TOKEN type);
t_redir     *new_cmd_redir (REDIR_MODE mode, char *file_name);
t_token     *new_token(TOKEN type);
void        init_lvars(t_lvars *vars);

/*=====================LEXERS=====================*/

/* OPERATOR */
void	handle_operators(t_token **token, char **line);
void	place_operator_node(t_token **root, t_token **to_put);
t_bool	handle_error_if_not_valid_op(char **line, char operator, int len);
t_bool	is_an_operator(char **line, int len);
t_eval  check_operator_syntax(char **line);

/* PARENTHESIS*/
void	place_parenthesis_node(t_token **root, t_token **to_put);
void	handle_parenthesis(t_token **root, char **line);
void	parenthesis_lexer(t_token **token, char **line);

/* PIPE */
void	place_pipe_node(t_token **root, t_token **to_put);
void	handle_pipe(t_token **token, char **line);
void	handle_pipes(t_token **token, char **line);

void	handle_commands(t_token **root, char **line);
void	place_node(t_token **root, t_token **to_put, TOKEN type);
void    lexer(t_token **token, char **line);



/*=====================CMD_PARSER=====================*/
t_token     *handle_command(char **line);
char        *copy_token_in_between_quotes(char **line, int len, int *og_len);
char        **add_arg(char **args, char *arg);
char        **add_cmd_to_args(char *cmd , char **args);
char        *ft_special_join(char *arg, char *buff);
void        get_command(t_token **token, char **line);
char        *get_token_in_between_quotes(char **line, char quote, int *og_len);
void        get_token_wquotes(char **line, char **arg, int *og_len);
void        get_token_wout_quotes(char **line, char **arg, int *og_len);
char        *get_token_with_quotes(char **line, int len, int *og_len);
void        process_line(char **line, t_token **token);
void        process_line_without_quotes(char **line, t_token **token, t_lvars *vars);
void        process_line_with_quotes(char **line, t_token **token, t_lvars *vars);
void        process_simple_cmd(t_token **token, char **line, int len);
void        process_line_when_parenthesis(char **line, t_token **token, int len);

/*=====================CMD_PARSER_UTILS=====================*/

t_cmd       *new_cmd(void);

void        skip_spaces(char **line);
t_bool      arg_len_condition(char **line, int len);
t_bool      process_line_condition(char **line);
t_bool      handle_parenthesis_error(char **line, int len);
t_bool      check_char_validation_for_cmd(char **line, char *tmp, int len);
t_bool      check_for_quotes_condition(char **line , t_lvars *vars);
t_bool      check_for_quotes(char **line, t_lvars *vars);
t_bool      is_parenthesis(char c);
t_bool      is_valid_char_to_be_processed(char c);

/*=====================REDIR_PARSER=====================*/
REDIR_MODE  get_redir_mode(char **line);
t_bool      is_valid_filename(char c);
int         calculate_file_name_len(char **line, t_lvars *vars);
void        add_redirection(t_redir **redir, REDIR_MODE mode, char *file_name);
void        check_for_expansion_in_heredoc(t_token **token , t_bool is_root, REDIR_MODE mode);
void        get_file_name(char **line, char **file_name, t_lvars *vars);
void        handle_redirection(t_token **token, char **line, t_bool is_root);
void        handle_input(t_token **token, char **line, t_bool is_root);
void        handle_output(t_token **token, char **line, t_bool is_root);

/*=====================LEXER_UTILS=====================*/
void        skip_spaces(char **line);
char	    **split(char *str, char *charset);
char        **add_arg(char **args, char *arg);
void        add_redirection(t_redir **redir, REDIR_MODE mode, char *file_name);
void    	parenthesis_lexer(t_token **root, char **line);
char        *get_token_with_quotes(char **line, int len, int *og_len);


char    **ft_dup_args(char **args);
t_redir *ft_dup_redir(t_redir *redir);

/*=====================SIGNALS=====================*/
t_sigstate sig_state(t_sigstate state, t_sigops operation);
void singal_handler(void);
void handle_sig(int sig);


/*=====================SYNTAX_EVALUATOR==================*/
void    evaluate_syntax(t_token *root);


/*=====================HERE_DOC==================*/
char    *expand_in_heredoc(t_exec *exec, char *line);
void    here_doc_helper(int w_heredoc, char *delimiter);
void    here_doc(t_redir *here_doc, t_bool error_flag);
/*=====================HERE_DOC_UTILS==================*/
char    *get_value_in_between_quotes(char *line, char quote);
char    *ft_get_value(t_exec *exec, char *key);
t_bool  ft_check_for_quotes(char *og_delimiter);
t_bool  is_delimiter(char *buffer, char *delimiter);
void    heredoc_to_fd(t_redir **heredoc_redir);
t_bool  is_valid_char_for_expansion(char c);
void    init_iter_vars(t_iter_vars *vars);

/*=====================UTILS========================*/
int         strs_len(char **args);
int         ft_strcmp(const char *s1, const char *s2);
t_bool      is_space(char c);
t_bool      is_quote(char c);
t_bool      is_special_char(char c);
t_bool      is_redirection_char(char c);
t_bool      is_an_operator(char **line, int len);
void        special_trim(char **line);
char	    *ft_tolowerr(char *c);
char        *ft_strcpy(char *dest, char *src);

/*=====================ENV=====================*/
char	   **env_to_envp(t_exec *exec);
void	   ft_write(char *str, int fd, int new_line);
void        copy_key(char **env, t_var *var);
void        print_env(t_env *head);
void        append_node(t_env **head, char *key, char *value);
t_env       *parse_env(char **env);
t_env       *create_node(char *key, char *value);

/*=====================BUILTINS=====================*/
int	        pwd(void);
int         ft_echo(char **arg);
int         unset(t_env **env, char **cmd);
void        ft_env(t_env *env);
int         cd(char **arg, t_exec *exec);
int         ft_export(t_env *env, char **args);



/*=======================EXECUTION=======================*/
// void        execute_commands(t_token *token);
// void        open_pipe_and_redirect(void);
// void        execute_command(t_token *token);
// void        execute_manager(t_token *head);
// int execute(t_token *token, char **envp);
// int execute_command(t_token *token, char **envp);
// void execute_command(t_token *token, char **envp);
char	    **find_path(char **envp);
void	    free_2d(char **arr);
char	    *get_cmd(char *cmd, char **envp);
void        execute_or(t_token *node, t_exec *exec);
void        execute_and(t_token *node, t_exec *exec);
int        execute(t_token *token, t_exec *exec);
int         stat_handler(int code, int flag);
char        *expand_env_variable(char *input, t_env *env);
void        handle_input_redirection(t_redir *input, t_env *env);
void        handle_output_redirection(t_redir *output, t_env *env);
void execute_subtree(t_token *root, t_exec *exec);
void        handle_redirections(t_cmd *cmd, t_env *env, t_exec *exec);
// int	        check_builtins(t_token *node, t_exec *exec, char **args);
int	check_builtins(char *cmd, t_cmd *cc, t_exec *exec, char **args);
char **expander(t_token *token, t_exec *exec, char *cmd);
char **initialize_args_if_null(char *cmd, char **args);
void handle_output_redirections(t_redir *output, t_redir *og_output, t_env *env);
// void        expand_variables(t_cmd *cmd, t_env *env_list, int *flag);
// char        *expand_token(const char *token, t_env *env_list);
// char        **expand_args(char **args, t_env *env_list);
// char **process_args(char **args, char **og_args, char *cmd, t_env *env_list);
// char **process_args(char **args, char **og_args, char *cmd, t_env *env_list) ;
// char *expand_string(const char *str, t_env *env_list);
// char *expand_string(const char *str, t_env *env_list, int in_quotes);
// char *expand_arg_if_needed(char *arg, char *og_arg, t_env *env_list);
// char **expand_arg_if_needed(char *arg, char *og_arg, t_env *env_list);
// char **expand_arg_if_needed(char *arg, char *og_arg, t_env *env_list, int in_quotes);
char    **expand_arg_if_needed(char *arg, char *og_arg, t_env *env_list);
// int is_inside_quotes(const char *str, const char *pos)
char    *expand_string(const char *str, t_env *env_list);
char    **process_args(char **args, char **og_args, char *og_cmd, char *cmd, t_env *env_list);
char    **process_cmd_and_args(char *cmd, char **args, t_env *env_list);
int     check_to_expand(char *cmd, t_env *env_list);
void	ft_exit(char **cmd);
char    *ft_strndup(const char *s1, size_t n);
char    **expand_wildcards(char **args);
void    handle_input_redirections(t_redir *input, t_redir *og_input, t_env *env, t_exec *exec);
void reset_fd(int in, int out);
void exec_error(char *cmd, char *cmd_path);
char	*ft_itoa_no_malloc(int n);



//=====================DEBUG=====================//
void    display_cmd(t_token *token);


/*=====================MINISHELL=====================*/
void        minishell_loop(char **env);

/*=====================CLEANUP=====================*/
void        free_strs(char **strs);
void        free_redirections(t_redir *redir);
void        free_expands(t_expands *expands);
void        free_cmd(t_token *root);
void        free_token(t_token *root);
void        cleanup(t_token *root);
t_bool      ft_print_error(char *message, char **line, t_error indicator);
void        ft_error(char *error_message, int exit_code);
void        free_env_list(t_env *head);
void        safe_free(void *ptr);
// void reset_fd(int in, int out);

#endif