/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:55:58 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/28 00:52:05 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nb_len(long n)
{
	int	len;

	len = 0;
	if (n <= 0)
	{
		len++;
		n = -n;
	}
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*ft_convert(char *s, int len, long n)
{
	int	sign;

	sign = 0;
	if (n < 0)
	{
		n = -n;
		sign = 1;
	}
	s[len--] = '\0';
	while (len >= 0)
	{
		s[len--] = (char)((n % 10) + 48);
		n /= 10;
	}
	if (sign)
		s[0] = '-';
	return (s);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*nbr;

	len = ft_nb_len((long)n);
	nbr = (char *)malloc(sizeof(char) * (len + 1));
	if (!nbr)
		return (NULL);
	nbr = ft_convert(nbr, len, ((long)n));
	return (nbr);
}
