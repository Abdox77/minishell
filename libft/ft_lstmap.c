/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:34:35 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 21:06:01 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_root;
	t_list	*new_node;
	void	*cont;

	if (!lst || !f || !del)
		return (NULL);
	new_root = NULL;
	while (lst)
	{
		cont = f(lst->content);
		new_node = ft_lstnew(cont);
		if (!new_node)
		{
			ft_lstclear(&new_root, del);
			return (NULL);
		}
		ft_lstadd_back(&new_root, new_node);
		lst = lst->next;
	}
	return (new_root);
}
