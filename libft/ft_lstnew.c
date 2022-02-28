/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstnew.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:58:54 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:58:54 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Creates a new t_list entry containing the given content
 *
 * @param	content	The content to put in the new t_list entry
 *
 * @return	A new t_list entry with the given content
 */
t_list	*ft_lstnew(void *content)
{
	t_list	*start;

	start = ft_calloc(1, sizeof(*start));
	if (!start)
		return (NULL);
	start->content = content;
	return (start);
}
