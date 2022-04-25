/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_echo.c                                             :+:    :+:         */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "ft_echo.h"

static t_bool	is_it_an_edge_case(t_command *command, int fd)
{
	char	**args;

	args = command->args;
	if (command->args_len == 1 && ft_streq(args[0], "echo"))
	{
		write(fd, "\n", 1);
		return (new_set_exit_status(0, NULL));
	}
	if (command->args_len == 2 && ft_streq(args[1], "-n"))
	{
		return (new_set_exit_status(0, NULL));
	}
	return (false);
}

static void	print_leftover(t_command *command, int i, t_bool get_rid_of_space)
{
	char	**args;

	args = command->args;
	while (i != command->args_len)
	{
		if (i != 1)
		{
			if (get_rid_of_space == true)
				get_rid_of_space = false;
			else if (i != command->args_len)
				write(1, " ", 1);
		}
		write(1, args[i], ft_strlen(args[i]));
		i++;
	}
}

static void	skip_n_flags(t_command *command, int *i, t_bool *get_rid_of_space)
{
	int		j;
	int		args_len;
	char	**args;

	args = command->args;
	args_len = command->args_len;
	if (ft_streq(args[*i], "-n") == 1)
	{
		while (*i < args_len - 1 && (ft_strncmp(args[*i], "-n", 2) == 0))
		{
			j = 1;
			while (args[*i][j] == 'n')
				j++;
			if (args[*i][j] == 0)
				(*i)++;
			else
				break ;
		}
		*get_rid_of_space = true;
	}
}

/**
 * Takes an array of arrays (user input), checks if the first
 *	argument is the -n flag.
 * Prints the arguments with a single whitespace char between.
 *	Prints newline char if -n was not specified
 *
 * @param	args	the input given by the user,
 *	excluding "echo" -> input: echo batman -> args = batman
 *
 * @return	true or false depending whether execution was successful
*/
t_bool	ft_echo(t_command *command, int fd)
{
	int		i;
	char	c;
	t_bool	get_rid_of_space;

	i = 1;
	c = '\0';
	get_rid_of_space = false;
	if (is_it_an_edge_case(command, fd) == true)
	{
		return (true);
	}
	skip_n_flags(command, &i, &get_rid_of_space);
	if (i == 1)
	{
		c = '\n';
	}
	print_leftover(command, i, get_rid_of_space);
	write(1, &c, 1);
	return (new_set_exit_status(0, NULL));
}
