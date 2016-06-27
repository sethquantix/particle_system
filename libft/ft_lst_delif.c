/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_delif.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <cchaumar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/08 15:06:38 by cchaumar          #+#    #+#             */
/*   Updated: 2016/05/23 18:12:35 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_delif(t_list **root, t_list *elem)
{
	t_list	*temp;
	t_list	*node;

	if (!root)
		return ;
	node = *root;
	temp = *root;
	while (node != elem)
	{
		temp = (node != *root) ? node : temp;
		node = node->next;
	}
	if (temp == elem)
	{
		temp = (*root)->next;
		node = *root;
		*root = temp;
	}
	else
		temp->next = node->next;
	if (node && node->content)
		ft_memdel((void **)&(node->content));
	if (node)
		ft_memdel((void **)&node);
}
