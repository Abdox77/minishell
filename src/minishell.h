/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:45 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 13:46:30 by aabou-ib         ###   ########.fr       */
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





/*===========================enum=========================*/

typedef enum {
    false,
    true
}   t_bool;

typedef enum {
    INFILE,
    OUTFILE,
    HEREDOC,
    APPEND ,
    TRUNC,
    STD_OUT
}   REDIR_MODE;

typedef enum
{
    NONE,
    CMD,
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
    t_redir     *redir;
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

/*=====================LEXER=====================*/
void lexer(t_token *token, char **line);


/*=====================LEXER_UTILS=====================*/
char	    **split(char *str, char *charset);
char        **add_arg(char **args, char *arg);
void        handle_quotes(t_token *token, char ** line);
void        handle_command(t_token *token, char **line);
t_cmd       *new_cmd(void);
t_bool      is_space(char c);
t_bool      is_quote(char c);
t_bool      is_special_char(char c);
t_bool      is_redirection_char(char c);
t_token     *new_token(TOKEN type);


void        handle_redirection(t_token *token, char **line, REDIR_MODE type);
void        handle_input(t_token *token, char **line);
void        handle_output(t_token *token, char **line);
void        add_redirection(t_token *token, REDIR_MODE mode, char *file_name);

/*=====================UTILS========================*/
int         strs_len(char **args);



/*=====================MINISHELL=====================*/
void        minishell_loop(void);


/*=====================CLEANUP=====================*/
void        free_strs(char **strs);
void        ft_error(char *error_message, int exit_code);

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
int         unset(t_env **env, char **cmd);
void        envv(t_env *env);
void        free_env_node(t_env **env, char *arg);
int         n_exists(char *arg);
int         echo(char **arg);


#endif