/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_cmd.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 15:51:25 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/02 15:51:25 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "../libft/libft.h"
#include "../headers/functions.h"

/**
 * Changes directory and updates pwd on success
 *
 * @param	dir	free-able char pointer
 *
 * @return	-1 for failure in chdir command
 * 	-2 if dir was null
 */
t_bool	cd(t_command *command, t_minishell *minishell)
{
	void	*tmp;
	char	*dir;
	char	*tmp_dir;
	t_bool	result;

	if (command->args_len != 2)
	{
		ft_printf("Invalid command, not enough args\n");
		return (false);
	}
	dir = command->args[1];
	if (dir == NULL)
		return (false);
	else if (*dir == '/')
		dir = ft_strdup(dir);
	else
	{
		tmp_dir = ft_strjoin("/", dir);
		if (!tmp_dir)
		{
			ft_printf("Out of memory\n");
			return (false);
		}
		dir = ft_strjoin(minishell->cur_wd, tmp_dir);
		free(tmp_dir);
	}
	if (dir == NULL)
		return (false);
	tmp = opendir(dir);
	if (!tmp)
		return (false);
	chdir(dir);
	result = set_pwd(getcwd(NULL, 0), minishell);
	free(tmp);
	return (result);
}
