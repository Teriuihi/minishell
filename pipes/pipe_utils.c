/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   functions.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 14:13:16 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/04 14:13:16 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipe_utils.h"

t_bool	close_pipes(int *pid1, int *pid2)
{
	if (pid1 != NULL && *pid1 != -1)
	{
		if (close(*pid1) == -1)
		{
			*pid1 = -1;
			return (new_set_exit_status(1, NULL));
		}
		else
		{
			*pid1 = -1;
		}
	}
	if (pid2 != NULL && *pid2 != -1)
	{
		if (close(*pid2) == -1)
		{
			*pid2 = -1;
			return (new_set_exit_status(1, NULL));
		}
		else
		{
			*pid2 = -1;
		}
	}
	return (true);
}

void	check_is_dir(t_command *command)
{
	int	i;

	i = 0;
	while (command->command[i] == '/')
		i++;
	if (command->command[i] == 0)
		ft_printf(2, "some shell: %s: is a directory\n",
			command->command);
}
