/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:45 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/14 21:03:53 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

// macros
#define PURPLE "\033[0;35m"
#define GREEN "\033[36m"

typedef enum {
    false,
    true
}   t_bool;

typedef enum {
    INFILE,
    OUTFILE,
    HEREDOC,
    APPEND ,
    STD_OUT
}   REDIR_MODE;

// typedef enum
// {
//     SINGLE,
//     DOUBLE
// } t_quote;

typedef enum
{
    NONE,
    CMD,
    PIPE 
    
    // here the madeup types to fix later    
}   TOKEN;

typedef struct s_token t_token;
typedef struct s_cmd   t_cmd;
typedef struct s_redir t_redir;

struct s_token {
    TOKEN       type;
    char        *value;
    t_token     *r_token;
    t_token     *l_token;
    t_cmd       *cmd;
} ;

typedef struct s_vars {
    int i;
    int j;   
} t_vars;


struct s_redir {
    REDIR_MODE type;
    char *file;
    // t_redir *next; // later
};

struct s_cmd
{
    // int         pid;
    char        *cmd;
    char        **args;
    t_redir     redir;
};

//=====================INIT=====================
t_token     *init_token(void);
t_token     *set_pipe_head(void);
t_token     *new_token(TOKEN type);

//=====================LEXER=====================
void lexer(t_token *token, char **line);
void handle_redirection(t_token *token, char **line, REDIR_MODE type);

//=====================LEXER_UTILS=====================
t_bool      is_space(char c);
t_bool      is_quote(char c);
t_bool      is_special_char(char c);
t_bool      is_redirection_char(char c);
char	    **split(char *str, char *charset);
char        **add_arg(char **args, char *arg);
t_cmd       *new_cmd(void);
t_token     *new_token(TOKEN type);
void        handle_quotes(t_token *token, char ** line);
void        handle_command(t_token *token, char **line);

//=====================UTILS========================
int         strs_len(char **args);



//=====================MINISHELL=====================
void    minishell_loop(void);


//=====================CLEANUP=====================
void    free_strs(char **strs);
void    ft_error(char *error_message, int exit_code);


#endif