/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:45 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/17 19:36:52 by amohdi           ###   ########.fr       */
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
# include <linux/limits.h>

/*==========================MACROS============================*/

# define PURPLE "\033[0;35m"
# define GREEN "\033[36m"
# define HEREDOC_LOCATION "/var/tmp/"
# define BUFFER_SIZE 1000
# define W_HEREDOC 1
# define R_HEREDOC 0
/*===========================enum=========================*/

typedef enum{
    PRINT,
    SAVE, 
    NOT_PRINTED,
    RETRIEVE,
    RESET,
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
    PIPE     
}   TOKEN;

/*=======================TYPEDEFS=========================*/

typedef struct s_token      t_token;
typedef struct s_cmd        t_cmd;
typedef struct s_redir      t_redir;
typedef struct s_env        t_env;
typedef struct s_env_var    t_var;
typedef struct s_exec       t_exec;
typedef struct s_expands    t_expands;



/*=======================STRUCTS============================*/

struct s_expands {
    char        **og_args;
    char        *og_cmd;
    t_redir     *og_input;
    t_redir     *og_output;
};

struct s_token {
    TOKEN       type;
    t_token     *r_token;
    t_token     *l_token;
    t_redir     *output;
    t_redir     *og_output;
    t_cmd       *cmd;
} ;

struct s_redir {
    int         fd;
    int         here_doc_fd[2];
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
    t_redir     *input;
    t_redir     *output;
};


struct s_exec
{
	t_env	*env;
	char	*tmp;
	char	**envp;
	int		o_in;
	int		o_out;
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


/*=====================INIT=====================*/
t_token     *init_token(void);
t_token     *set_pipe_head(void);
t_token     *new_token(TOKEN type);
t_redir     *new_cmd_redir (REDIR_MODE mode, char *file_name);
t_token     *new_token(TOKEN type);

/*=====================LEXER=====================*/
void        lexer(t_token **token, char **line);
t_token     *handle_command(char **line);
void        handle_redirection(t_token **token, char **line, t_bool is_root);
void        handle_input(t_token **token, char **line);
void        handle_output(t_token **token, char **line, t_bool is_root);

/*=====================LEXER_UTILS=====================*/
t_cmd       *new_cmd(void);
void        skip_spaces(char **line);
char	    **split(char *str, char *charset);
char        **add_arg(char **args, char *arg);
void        add_redirection(t_redir **redir, REDIR_MODE mode, char *file_name);
void    	parenthesis_lexer(t_token **root, char **line);
char        *get_token_with_quotes(char **line, int len, int *og_len);


// duplicating cmd token
char    **ft_dup_args(char **args);
t_redir *ft_dup_redir(t_redir *redir);

/*=====================SYNTAX_EVALUATOR==================*/
void    evaluate_syntax(t_token *root);
void    here_doc(t_redir **here_doc);
/*=====================HERE_DOC==================*/
void    expand_heredoc_to_infiles(t_token **root);


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
void        execute(t_token *token, t_exec *exec);
int         stat(int code, int flag);
char        *expand_env_variable(char *input, t_env *env);
void        handle_input_redirection(t_redir *input);
void        handle_output_redirection(t_redir *output);
void        handle_redirections(t_cmd *cmd);
int	        check_builtins(t_token *node, t_exec *exec, char **args);
void        expand_variables(t_cmd *cmd, t_env *env_list, int *flag);
char        *expand_token(const char *token, t_env *env_list);
char        **expand_args(char **args, t_env *env_list);




//=====================DEBUG=====================//
void    display_cmd(t_token *token);


/*=====================MINISHELL=====================*/
void        minishell_loop(char **env);

/*=====================CLEANUP=====================*/
void        free_strs(char **strs);
t_bool      ft_print_error(char *message, char **line, t_error indicator);
void        ft_error(char *error_message, int exit_code);

#endif