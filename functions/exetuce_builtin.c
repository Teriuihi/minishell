/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_builtin.c                                        :+:    :+:      */
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

/**
 * Takes an array of arrays (user input)
 * Selects the function to execute based on the first argument (args[0])
 * @param	args	the input given by the user
 *
 * @return	void
 */

int	execute_builtin(t_command *command, int fd_read)
{
	char	*cur_dir;

	cur_dir = get_pwd();
	if (!command->command)
		return (-1);
	else if (!ft_strncmp(command->command, "exit", 4))
		exit (0);
	else if (!ft_strncmp(command->command, "echo", 4))
		ft_echo(command, 1);
	else if (!ft_strncmp(command->command, "cd", 2))
		cd(*command->args);
	else if (!ft_strncmp(command->command, "pwd", 3))
	{
		ft_putstr_fd(cur_dir, 1);
		ft_putstr_fd("\n", 1);
	}
	free(cur_dir);
	fd_read = 0; //TODO ignored for now
	return (0);
}
