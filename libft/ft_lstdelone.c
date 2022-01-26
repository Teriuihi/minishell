/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstdelone.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:58:59 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:58:59 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Deletes and free's a t_list
 *
 * @param	lst	the t_list to delete
 * @param	del	The function to apply to the content of the t_list to delete it
 */
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst)
		return ;
	del(lst->content);
	free(lst);
}
