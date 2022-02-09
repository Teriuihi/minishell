/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   is_builtin.c                                             :+:    :+:      */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"

/*
 * Takes an array of arrays (user input)
 * Prints the arguments with a single whitespace char between.
 * Prints newline char if -n was not specified
 *
 * @param	args	the input given by the user
 *
 * @return	true if the input is a builtin command, false if not
*/

int	is_builtin(char *command)
{
	int			i;
	const char	*builtins[7] = {"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit"};

	if (command == NULL)
		return (0);
	i = 0;
	while (i < 7)
	{
		if (ft_streq(command, builtins[i]))
			return (1);
		i++;
	}
	return (0);
}
