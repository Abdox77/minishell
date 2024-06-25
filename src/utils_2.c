/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:23:05 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/25 01:02:10 by aabou-ib         ###   ########.fr       */
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
    if (is_redirection_char(c) == TRUE || is_space(c) == TRUE || is_quote(c) == TRUE || c == '|')
        return (TRUE);
    return (FALSE);
}

t_bool      is_redirection_char(char c)
{
    if (c == '>' || c == '<')
        return (TRUE);
    return (FALSE);
}

char *ft_strcpy(char *dest, char *src)
{
	int i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
