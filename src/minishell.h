/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:45 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/11 21:15:20 by amohdi           ###   ########.fr       */
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
// # include <linux/limits.h>

/*==========================MACROS============================*/

# define PURPLE "\033[0;35m"
# define GREEN "\033[36m"



/*===========================enum=========================*/

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


/*=======================STRUCTS============================*/


struct s_token {
    TOKEN       type;
    t_token     *r_token;
    t_token     *l_token;
    t_cmd       *cmd;
} ;

struct s_redir {
    REDIR_MODE mode;
    char *file_name;
    t_redir *next; // later
};

struct s_cmd
{
    // int         pid;
    char        *cmd;
    char        **args;
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
void        handle_quotes(t_token **token, char ** line);
void        handle_redirection(t_token **token, char **line);
void        handle_input(t_token **token, char **line);
void        handle_output(t_token **token, char **line);
void        handle_operator(t_token **token, char **line);

/*=====================LEXER_UTILS=====================*/
void        skip_spaces(char **line);
char	    **split(char *str, char *charset);
char        **add_arg(char **args, char *arg);
void        add_redirection(t_redir **redir, REDIR_MODE mode, char *file_name);
t_cmd       *new_cmd(void);
void    	parenthesis_lexer(t_token **root, char **line);


// duplicating cmd token
char **ft_dup_args(char **args);
t_redir *ft_dup_redir(t_redir *redir);

/*=====================SYNTAX_EVALUATOR==================*/
t_bool synatx_evaluator(t_token *root);


/*=====================UTILS========================*/
int         strs_len(char **args);
int         ft_strcmp(const char *s1, const char *s2);
t_bool      is_space(char c);
t_bool      is_quote(char c);
t_bool      is_special_char(char c);
t_bool      is_redirection_char(char c);
t_bool      is_an_operator(char *line);
void        special_trim(char **line);


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
int         echo(char **arg);
int         unset(t_env **env, char **cmd);
void        ft_env(t_env *env);
int         cd(char **arg, t_exec *exec);


/*=======================EXECUTION=======================*/
// void        execute_commands(t_token *token);
// void        open_pipe_and_redirect(void);
// void        execute_command(t_token *token);
// void        execute_manager(t_token *head);
// int execute(t_token *token, char **envp);
// int execute_command(t_token *token, char **envp);
// void execute_command(t_token *token, char **envp);
char	**find_path(char **envp);
void	free_2d(char **arr);
char	*get_cmd(char *cmd, char **envp);
void     execute_or(t_token *node, char **envp);
void     execute_and(t_token *node, char **envp);
void     execute(t_token *token, char **envp);
int     stat(int code, int flag);
char *expand_env_variable(char *input, t_env *env);
void handle_input_redirection(t_redir *input);
void handle_output_redirection(t_redir *output);
void handle_redirections(t_cmd *cmd);



//=====================DEBUG=====================//
void    display_cmd(t_token *token);


/*=====================MINISHELL=====================*/
void        minishell_loop(char **env);

/*=====================CLEANUP=====================*/
void        free_strs(char **strs);
void        ft_error(char *error_message, int exit_code);

#endif
