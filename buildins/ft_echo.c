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

#include "../libft/libft.h"
#include "../headers/functions.h"

static t_bool	is_it_an_edge_case(t_command *command, int fd,
				t_minishell *minishell)
{
	char	**args;

	args = command->args;
	if (command->args_len == 1 && ft_streq(args[0], "echo"))
	{
		write(fd, "\n", 1);
		return (set_exit_status(minishell, 0, NULL, false));
	}
	if (command->args_len == 2 && ft_streq(args[1], "-n"))
	{
		return (set_exit_status(minishell, 0, NULL, false));
	}
	return (false);
}

void	print_leftover(int i, char **args, t_bool get_rid_of_space,
						t_command *command)
{
	while (i != command->args_len) //why argslen + 1?
	{
		if (i != 1)
		{
			if (get_rid_of_space == true)
				get_rid_of_space = false;
			else if (i != command->args_len - 1)
				write(1, " ", 1);
		}
		write(1, args[i], ft_strlen(args[i]));
		i++;
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
t_bool	ft_echo(t_command *command, int fd, t_minishell *minishell)
{
	t_bool	get_rid_of_space;
	int		i;
	int		j;
	char	c;
	char	**args;

	//echo can have multiple inputs with -n. eg :-n -n -nnnn
	args = command->args;
	i = 1;
	get_rid_of_space = false;
	c = '\0';
	if (is_it_an_edge_case(command, fd, minishell) == true)
	{
		return (true);
	}
	//set initial contidions, TODO FUNCTION, //why is the second condition there in the while loop
	if (ft_streq(args[i], "-n") == 1)
	{
		while (i < command->args_len - 1 && (ft_streq(args[i], "-n") == 1) == 1)
		{
			j = 1;
			while (args[i][j] == 'n')
				j++;
			if (args[i][j] == 0)
				i++;
			else
				break ;
		}
		get_rid_of_space = true;
	}
	else
		c = '\n';
	print_leftover(i, args, get_rid_of_space, command);
	write(1, &c, 1);
	return (set_exit_status(minishell, 0, NULL, false));
}
