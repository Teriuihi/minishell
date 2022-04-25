/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_pwd.c                                          :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_pwd.h"

t_bool	ft_pwd(char *cur_dir)
{
	if (ft_putstr_fd(cur_dir, 1) == -1)
		return (new_set_exit_status(1, NULL));
	if (ft_putstr_fd("\n", 1) == -1)
		return (new_set_exit_status(1, NULL));
	return (new_set_exit_status(0, NULL));
}

/**
 * Get the working directory
 *
 * @return	the current working directory
 */
char	*get_pwd(t_minishell *minishell)
{
	if (!minishell)
		return (NULL);
	return (minishell->cur_wd);
}

/**
 * Update current working directory in minishell data
 *
 * @param	path		New path to set wd to
 * @param	minishell	Data for minishell
 *
 * @return	True on success, false on failure
 */
t_bool	update_pwd(char *path, t_minishell *minishell)
{
	if (ft_set_env("PWD", path, minishell->env, true) == false)
	{
		free(path);
		return (false);
	}
	free(minishell->cur_wd);
	minishell->cur_wd = ft_strdup(path);
	free(path);
	if (minishell->cur_wd == NULL)
	{
		return (false);
	}
	return (true);
}

/**
 * Set working directory to new path
 *
 * @param	path		New path to set wd to
 * @param	minishell	Data for minishell
 *
 * @return	True on success, false on failure
 */
t_bool	set_pwd(char *path, t_minishell *minishell)
{
	char	*pwd_path;
	char	*tmp;

	pwd_path = get_pwd(minishell);
	if (!path || !*path)
		return (false);
	if (pwd_path == NULL || *path == '/')
	{
		return (update_pwd(path, minishell));
	}
	tmp = ft_calloc(ft_strlen(path) + ft_strlen(pwd_path) + 1, sizeof(char));
	if (!tmp)
		return (false);
	ft_strlcpy(tmp, path, ft_strlen(path));
	ft_strlcpy(tmp + ft_strlen(path), pwd_path, ft_strlen(pwd_path));
	free(path);
	return (update_pwd(tmp, minishell));
}
