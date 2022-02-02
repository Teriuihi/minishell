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

/*
* Takes an array of arrays (user input), checks if the first 
	argument is the -n flag. 
* Prints the arguments with a single whitespace char between. 
	Prints newline char if -n was not specified
*
* @param	args	the input given by the user, 
				excluding "echo" -> input: echo batman -> args = batman
*
* @return	void
*/

void	ft_echo(t_command *command, int fd)
{
	int		i;
	char	c;
	char	**args;

	args = command->args;
	i = 0;
	c = '\0';
	if (!args || args[0] == NULL)
		return ;
	if (ft_strncmp(args[0], "-n", 3) == 0)
		args++;
	else
		c = '\n';
	while (i != command->args_len)
	{
		if (i != 0)
			write(1, " ", 1);
		write(fd, args[i], ft_strlen(args[i]));
		i++;
	}
	write(fd, &c, 1);
}
