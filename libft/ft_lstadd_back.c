/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstadd_back.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:02 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:02 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Adds a new entry to the back of the t_list
 *
 * @param	lst	The pointer to the start of the t_list to add the entry to
 * @param	new	The new entry to add to the t_list
 */
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		new->prev = ft_lstlast(*lst);
		new->prev->next = new;
	}
}
