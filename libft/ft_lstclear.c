/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:09:19 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 21:06:41 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*curr;

	if (!lst || !(*lst) || !del)
		return ;
	while (*lst)
	{
		del((*lst)->content);
		curr = (*lst);
		(*lst) = (*lst)->next;
		free(curr);
	}
	lst = NULL;
}
