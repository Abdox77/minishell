/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_itoa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 02:26:52 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 02:28:01 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	ft_convert(char *s, int len, long n)
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
}

char	*ft_itoa_no_malloc(int n)
{
	static char	buffer[12];
	int			len;

	len = ft_nb_len((long)n);
	ft_convert(buffer, len, ((long)n));
	return (buffer);
}
