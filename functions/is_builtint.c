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

//export myvar=hello 
static	t_bool env_variable_found(char *command, t_data *data) //check if its not =hellothere
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
	//check for export?
	if (ft_streq(command, "export"))
	{
		data->export_flag = 1;
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

t_bool	is_builtin(char *command, t_data *data)
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
	if (env_variable_found(command, data) == true) //what happens if its false but because of incorrect input? hello==myvar
	{
		//if its true, can we not just add immediately to hashtable?
		return (true);
	}
	while (i < 7)
	{
		if (ft_streq(command, builtins[i]))
			return (true);
		i++;
	}
	return (false);
}
