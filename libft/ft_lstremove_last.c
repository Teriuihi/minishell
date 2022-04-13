/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstremove_last.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/30 20:19:58 by sappunn       #+#    #+#                 */
/*   Updated: 2022/03/30 20:19:58 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstremove_last(t_list **head)
{
	t_list	*entry;

	entry = ft_lstlast(*head);
	ft_lstdelentry(entry->content);
	if (entry->prev != NULL)
		entry->prev->next = NULL;
	else
		*head = NULL;
	free(entry);
}
