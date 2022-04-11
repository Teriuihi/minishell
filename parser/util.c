/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/08 17:03:58 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/08 17:03:58 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"

/**
 * Checks if a char is a pipe character
 *
 * @param	c	Char to check
 *
 * @return	boolean indicating if the character is a pipe character or not
 */
t_bool	is_pipe(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/**
 * Sets exit status and format + send a parse error
 *
 * @param	minishell	Data for minishell
 * @param	near		char near which the error happened
 *
 * @return	false
 */
t_bool	parse_error(t_minishell *minishell, char near)
{
	t_string	*message;

	message = init_string("some shell: syntax error near unexpected token `");
	if (message == NULL)
		return (set_exit_status(minishell, 1,
				"some shell: Out of memory.", false));
	message = append_char(message, near);
	if (message == NULL)
		return (set_exit_status(minishell, 1,
				"some shell: Out of memory.", false));
	message = append_char(message, '\'');
	if (message == NULL)
		return (set_exit_status(minishell, 1,
				"some shell: Out of memory.", false));
	set_exit_status(minishell, 258, message->s, false);
	free_string(message);
	return (false);
}
