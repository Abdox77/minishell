/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:25:26 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 18:59:21 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	s;

	i = 0;
	p = (unsigned char *)b;
	s = (unsigned char)c;
	while (i < len)
		p[i++] = s;
	return ((void *)p);
}
