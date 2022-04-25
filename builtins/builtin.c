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

#include "builtins.h"

/**
 * Checks whether assignment is in a correct form.
 * @return	true if if export or correct variable assignment is present,
 			false otherwise
*/
static t_bool	is_input_correct(char *command, int *count)
{
	int	i;
	int	equal_found;

	i = 0;
	equal_found = 0;
	while (command[i])
	{
		if (command[i] == '=')
		{
			*count = *count + 1;
			equal_found++;
			if (i == 0)
				return (false);
		}
		i++;
	}
	if (equal_found != 0)
		return (true);
	else
		return (false);
}

/**
 * 		&& var_names_correct(command_t->args) == true)
 * Checks whether export or an expression like "a=b" is present.
 * @return	true if if export or correct variable assignment is present,
 			false otherwise
*/
t_bool	env_variable_found(t_command *command_t)
{
	int		count;
	char	*command;

	command = command_t->command;
	count = 0;
	if (!command)
	{
		return (false);
	}
	if (ft_streq(command, "export") && command_t->args_len != 1)
	{
		command_t->export_found = true;
		return (true);
	}
	if (is_input_correct(command, &count) == false)
		return (false);
	if (count > 0)
		return (true);
	else
		return (false);
}

/**
 * Checks whether export or an expression like "a=b" is present.
 * Checks whether any of the builtin commands are equal to the char *command.
 *
 * @param	command struct, uses the char *command from the struct
 *
 * @return	true if the input is a builtin command, false otherwise
*/
t_bool	is_builtin(t_command *command)
{
	int			i;
	const char	*builtins[6] = {"echo", "cd", "pwd", "export", "unset", "env"};

	i = 0;
	if (!command)
	{
		return (false);
	}
	if (env_variable_found(command) == true)
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
	return (false);
}
