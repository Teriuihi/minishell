/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   add_command.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/01 19:09:11 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/01 19:09:11 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../string/string.h"
#include "../libft/libft.h"
#include "../headers/arguments.h"

/**
 * Free's a t_arg
 *
 * @param	ptr	Pointer to t_arg that needs to be freed
 */
static void	free_t_arg(void *ptr)
{
	t_arg	*arg;

	arg = ptr;
	free_string(arg->arg);
	free(arg);
}

/**
 * Adds a string to the argument list
 *
 * @param	head	Head of the argument list
 * @param	string	String to add
 * @param	literal	A boolean indicating if the argument is literal or not
 *
 * @return	a boolean indicating success/failure
 */
static t_bool	add_to_list(t_list **head, t_string *string, t_bool literal)
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

/**
 * Add to list with null checks and free's
 *
 * @param	head	Head of the list to add the arguments to
 * @param	string	String to add
 * @param	literal	If the string is literal or not
 *
 * @return	NULL on failure new empty string on success
 */
t_string	*safe_add_to_list(t_list **head, t_string *string, t_bool literal)
{
	if (add_to_list(head, string, literal) == false)
	{
		free_string(string);
		ft_lstclear(head, free_t_arg);
		return (NULL);
	}
	return (init_string(NULL));
}
