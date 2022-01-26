/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstlast.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:58:56 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:58:56 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * Go to the end of the t_list and return that entry
 *
 * @param	lst	The t_list to iterate through
 *
 * @return	The last entry of the given t_list
 */
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (lst);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
