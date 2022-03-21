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

/**
 * Takes an array of arrays (user input)
 * Prints the arguments with a single whitespace char between.
 * Prints newline char if -n was not specified
 *
 * @param	args	the input given by the user
 *
 * @return	true if the input is a builtin command, false if not
*/

t_bool	env_variable_found(char *command) //check if its not =hellothere
{
	int	i;
	int	count;
	int	equal_found;

	i = 0;
	count = 0;
	equal_found = 0;
	if (!command)
	{
		return (false);
	}
	if (ft_streq(command, "export"))
	{
		return (true);
	}
	while (command[i])
	{
		if (command[i] == '=') //if its two == next to eachother its incorrect input
		{
			count++;
			equal_found++;
			if (i == 0)
			{
				return (false);
			}
			if (command[i + 1] == '=' && equal_found == 1) //check if one has been already found, if yes then this is false
			{
				return (false);
			}
		}
		i++;
	}
	if (count > 0)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

t_bool	is_builtin(t_cmd_data *cmd_data)
{
	t_command	*command;
	int			i;
	const char	*builtins[6] = {"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env"};


	if (!cmd_data)
	{
		return (false);
	}
	command = cmd_data->command; // &cmd_data->command; was before
	if (command == NULL || command->command == NULL)
		return (0);
	i = 0;
	if (env_variable_found(command->command) == true) //what happens if its false but because of incorrect input? hello==myvar
	{
		return (true);
	}
	while (i < 6)
	{
		if (ft_streq(command->command, builtins[i]))
		{
			return (true);
		}
		i++;
	}
	if (cmd_data->output.type == REDIRECT_INPUT || cmd_data->output.type == DELIMITER_INPUT) //prob we dont need this anymore
		return (true);
	return (false);
}
