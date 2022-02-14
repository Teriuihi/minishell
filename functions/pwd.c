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

#include "../headers/functions.h"
#include <stdlib.h>

/**
 * Get the pwd, and sets it if path isn't null
 *
 * @param	path
 *
 * @return	the pwd (if it's been updated it will be equal to path)
 */
char	*get_pwd(void)
{
	return (ft_get_env_val("PWD", get_hash_table()));
}

t_bool	update_pwd(char *path, t_minishell *minishell)
{
	ft_set_env("PWD", path, get_hash_table());//what happens if set didnt work?
	free(minishell->cur_wd);
	minishell->cur_wd = ft_strdup(path);
	if (minishell->cur_wd == NULL)
		return (false);
	ft_printf("new wd: %s\n", minishell->cur_wd);
	return (true);
}

t_bool	set_pwd(char *path, t_minishell *minishell)
{
	char	*pwd_path;
	char	*tmp;

	pwd_path = get_pwd();
	if (!path || !*path)
		return (false);
	if (pwd_path == NULL || *path == '/')
	{
		free(pwd_path);
		return (update_pwd(path, minishell));
	}
	tmp = ft_calloc(ft_strlen(path) + ft_strlen(pwd_path) + 1, sizeof(char));
	if (!tmp)
		return (false);
	ft_strlcpy(tmp, path, ft_strlen(path));
	ft_strlcpy(tmp + ft_strlen(path), pwd_path, ft_strlen(pwd_path));
	free(pwd_path);
	free(path);
	return (update_pwd(tmp, minishell));
}
