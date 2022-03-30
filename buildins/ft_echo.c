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
	if (!args || args[0] == NULL)
	{
		return (set_exit_status(minishell, 2, NULL));
	}
	//another functions checking for edge case, put the two functions below into that
	if (command->args_len == 1 && ft_streq(args[0], "echo"))
	{
		write(fd, "\n", 1);
		return (set_exit_status(minishell, 0, NULL));
	}
	//what happens if its only echo -n
	if (command->args_len == 2 && ft_streq(args[1], "-n"))
	{
		return (set_exit_status(minishell, 0, NULL));
	}
	if (ft_strncmp(args[i], "-n", 3) == 0) //replace w streq, used to be ft_strcmp, -n, 3, 
	{	
		//ft_strncmp(args[i], "-n", 1) == 0 ? ft_printf(1, "YES\n") : ft_printf(1, "NO\n");
		while (i < command->args_len - 1 && (ft_strncmp(args[i], "-n", 2) == 0)) 
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
	while (i != command->args_len) //why argslen + 1?
	{
		if (i != 1)
		{
			if (get_rid_of_space == true)
				get_rid_of_space = false;
			else if (i != command->args_len - 1)
				write(1, " ", 1);
		}
		write(fd, args[i], ft_strlen(args[i]));
		i++;
	}
	write(fd, &c, 1);
	return (set_exit_status(minishell, 0, NULL));
}

/*
if args[1] == ~
	print $home
	
	if (ft_strncmp(args[1], "~", 1) //what is there is another char after?
	{
	check next char, if it
	if (ft_strlen(args[1]) > 1)
	{

	}
}
*/