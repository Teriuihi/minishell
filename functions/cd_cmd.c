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
int	cd(char *dir)
{
	void	*tmp;
	char	*tmp_dir;
	char	*cur_dir;

	if (dir == NULL)
		return (-2);
	else if (*dir == '/')
		dir = ft_strdup(dir);
	else
	{
		tmp_dir = ft_strjoin("/", dir); //TODO null check
		cur_dir = get_pwd();
		dir = ft_strjoin(cur_dir, tmp_dir); //TODO use env variable
		free(cur_dir);
		free(tmp_dir);
	}
	if (dir == NULL)
		return (-3);
	tmp = opendir(dir);
	if (!tmp)
		return (-1);
	chdir(dir);
	set_pwd(getcwd(NULL, 0));
	free(tmp);
	return (0);
}
