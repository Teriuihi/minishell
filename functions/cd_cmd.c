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

	if (dir == NULL)
		return (-2);
	else if (*dir == '/')
		dir = ft_strdup(dir);
	else
	{
		tmp_dir = ft_strjoin("/", dir); //TODO null check
		dir = ft_strjoin(get_pwd(NULL), tmp_dir); //TODO use env variable
		free(tmp_dir);
	}
	if (dir == NULL)
		return (-3);
	//TODO path should be in env variable
	tmp = opendir(dir);
	if (!tmp)
		return (-1);
	pwd(dir);
	free(tmp);
	return (0);
}
