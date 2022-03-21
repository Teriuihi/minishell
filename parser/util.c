/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 18:53:59 by sappunn       #+#    #+#                 */
/*   Updated: 2022/03/04 18:53:59 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/bool.h"
#include "../string/string.h"
#include "../headers/arguments.h"
#include "../libft/libft.h"

t_bool	add_to_list(t_list **head, t_string *string, t_bool literal)
{
	t_list	*entry;
	t_arg	*arg;

	if (!head || !string)
		return (false);
	arg = ft_calloc(1, sizeof(t_arg));
	if (!arg)
		return (false);
	arg->arg = string;
	arg->literal = literal;
	entry = ft_lstnew(arg);
	if (!entry)
	{
		free(arg);
		return (false);
	}
	ft_lstadd_back(head, entry);
	return (true);
}

void	free_t_arg(void *ptr)
{
	t_arg	*arg;

	arg = ptr;
	free_string(arg->arg);
}