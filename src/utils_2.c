/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:23:05 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/05 09:05:01 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool      is_space(char c)
{
    if (c == ' ' || c == '\t') // i need to check wether i should add the other whitespaces
        return (TRUE);
    return (FALSE);
}

t_bool      is_quote(char c)
{
    if (c == '\'' || c == '"')
        return (TRUE);
    return (FALSE);
}

t_bool      is_special_char(char c)
{
    if (is_redirection_char(c) || is_space(c) || is_quote(c) || c == '|'
    || c == '$')
        return (TRUE);
    return (FALSE);
}

t_bool      is_redirection_char(char c)
{
    if (c == '>' || c == '<')
        return (TRUE);
    return (FALSE);
}
