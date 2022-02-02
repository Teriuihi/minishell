/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 15:51:30 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/02 15:51:30 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <stdlib.h>

/**
 * Get the pwd, and sets it if path isn't null
 *
 * @param	path
 *
 * @return	the pwd (if it's been updated it will be equal to path)
 */
char	*get_pwd(char *path)
{
	static char	*pwd;

	if (path != NULL)
		pwd = path;
	return (pwd);
}

int	pwd(char *path)
{
	char	*pwd_path;
	char	*tmp;

	pwd_path = get_pwd(NULL);
	if (!path || !*path)
		return (-1);
	if (pwd_path == NULL || *path == '/' || *path == '~')
	{
		free(pwd_path);
		get_pwd(path);
		return (0);
	}
	tmp = ft_calloc(ft_strlen(path) + ft_strlen(pwd_path) + 1, sizeof(char));
	if (!tmp)
		return (-2);
	ft_strlcpy(tmp, path, ft_strlen(path));
	ft_strlcpy(tmp + ft_strlen(path), pwd_path, ft_strlen(pwd_path));
	free(pwd_path);
	free(path);
	get_pwd(tmp);
	return (0);
}