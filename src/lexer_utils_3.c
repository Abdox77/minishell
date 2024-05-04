// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   lexer_utils_3.c                                    :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/05/04 09:27:47 by amohdi            #+#    #+#             */
// /*   Updated: 2024/05/04 14:13:27 by amohdi           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// void get_cmd_between_parenthesis(t_token **token, char **line)
// {
//     ++(*line);
//     *token = handle_command(line);
//     if (**line != ')')
//         printf("Syntax error unexpected token near (");
//     else
//         ++(*line);
// }

// static void get_command(t_token **token, char **line)
// {
//     int len;
//     char *tmp;
//     char quote;

//     len = 0;
//     tmp = *line;
//     special_trim(line);
//     if (**line = '(')
//         get_cmd_between_parenthesis(token, line);
//     else
//     {
//         while(tmp[len] && is_special_char(tmp[len]) == false && is_an_operator(tmp + len) && is_parenthesis(tmp[len]))
//             ++len;
//         if (is_quote(tmp[len]) == true)
//         {
//             *token = new_token(CMD);
//             quote = tmp[len];
//             ++len;
//             while(tmp[len] && tmp[len] != quote)
//                 ++len;
//             if (!tmp[len])
//                 printf("syntax error unclosed quote\n");
//             (*token)->cmd->cmd = special_cmd_quote(line, len, quote);
//         }
//         else if (tmp[len] == ')')
//             printf("Syntax error unexpected token\n"); 
//         else if (len)
//         {
//             *token = new_token(CMD);
//             (*token)->cmd->cmd = ft_substr(*line, 0, len);
//         }
//     }
// }

// static t_bool is_parenthesis(char c)
// {
//     if (c == '(' || c == ')')
//         return true;
//     return false;
// }

// t_token *handle_command(char **line)
// {
//     int len;
//     char quote;
//     t_bool quote_flag;
//     t_token *cmd;

//     cmd == NULL;
//     quote_flag = false;
//     special_trim(line);
//     if(**line && is_an_operator(line) == false && **line != '|')
//     {
//         get_cmd(&cmd, line);
//         while(**line)
//         {
//             len = 0;
//             special_trim(line);
//             while(**line && **line != '|' && is_an_operator(*line) == false && is_parenthesis(**line) == false)
//             {
//                 if (is_quote(**line))
//                 {
//                     quote_flag = true;
//                     break;
//                 }
//                 len++;
//             }
//             if (quote_flag == true && len)
//             {
//                 quote = (*line)[len++];
//                 while((*line)[len] && (*line)[len] != quote)    
//                     ++len;
//                 if (!(*line)[len])
//                 {
//                     printf("Syntax Error unclosed quotes here\n");
//                     // okay , so i can add a flag in the struct of the command that indicates wether an erro happened in one of the commands so no need to allocate for the command , i'll only have to skip the command until i reach a new token
//                 }
//                 if (!cmd->cmd->args)
//                 {
//                     cmd->cmd->args = malloc(sizeof(char *) * 2);
//                     cmd->cmd->args[0] = get_token_with_quotes(line, len, quote);
//                     cmd->cmd->args[1] = NULL;
//                 }
//                 else
//                     cmd->cmd->args = add_arg(cmd->cmd->args, ft_substr(*line, 0, len));
//                 (*line) += len + 1;
//             }
//             else if ((*line)[len] == '(')
//                 printf("Syntax Error unexpected token near '('\n");
//             if (!cmd->cmd->args && len)
//             {
//                 cmd->cmd->args = malloc(sizeof(char *) * 2);
//                 cmd->cmd->args[0] = ft_substr(*line, 0, len);
//                 cmd->cmd->args[1] = NULL;
//                 (*line) += len;
//             }
//             else if (cmd->cmd->args && len)
//             {
//                 cmd->cmd->args = add_arg(cmd->cmd->args, ft_substr(*line, 0, len));
//                 (*line) += len;
//             }
//         }
//     }
    
//     return (cmd);
// }
