/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstdelentry.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:00 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:00 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	ft_lstdelentry(void *content)
{
	free(content);
}
