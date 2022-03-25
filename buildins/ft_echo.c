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
	int		i;
	int		j;
	char	c;
	char	**args;

	//echo can have multiple inputs with -n. eg :-n -n -nnnn
	args = command->args;
	i = 1;
	j = 1;
	c = '\0';
	if (!args || args[0] == NULL || args[1] == NULL)
		return (set_exit_status(minishell, 2));
	if (ft_strncmp(args[i], "-n", 3) == 0)
	{	
		while (i < command->args_len - 1 && (ft_strncmp(args[i], "-n", 1) == 0)) 
		{
			j = 1;
			while (args[i][j] == 'n')
				j++;
			if (args[i][j] == 0)
				i++;
			else
				break ;
		}
	}
	else
		c = '\n';
	while (i != command->args_len + 1)
	{
		if (i != 1)
			write(1, " ", 1);
		write(fd, args[i], ft_strlen(args[i]));
		i++;
	}
	write(fd, &c, 1);
	return (set_exit_status(minishell, 0));
}
