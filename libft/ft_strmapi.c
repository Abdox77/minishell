/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:26:29 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 14:09:58 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*r;
	size_t			len;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	r = (char *)malloc(sizeof(char) * (len + 1));
	if (!r)
		return (NULL);
	while (s[i])
	{
		r[i] = f(i, s[i]);
		i++;
	}
	r[i] = '\0';
	return (r);
}
