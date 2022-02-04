/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 14:13:16 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/04 14:13:16 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * Checks if the str is | > or >> should check for < and << later as well
 * Should return a value based on the type of pipe that's needed maybe?
 *
 * @param	str	string to check
 *
 * @return	0 if it's a command separator
 */

#include "../headers/structs.h"
#include "../libft/libft.h"

t_pipe_type	command_separator_type(char *str)
{
	if (ft_strlen(str) == 1)
	{
		if (*str == '|')
			return (OUTPUT_TO_COMMAND);
		if (*str == '>')
			return (REDIRECT_OUTPUT);
		if (*str == '<')
			return (REDIRECT_INPUT);
	}
	else if (ft_strlen(str) == 2)
	{
		if (!ft_strncmp(str, "<<", 2))
			return (DELIMITER_INPUT);
		if (!ft_strncmp(str, ">>", 2))
			return (APPEND_OUTPUT);
	}
	return (NONE);
}
