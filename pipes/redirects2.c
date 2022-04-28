/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects2.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/28 17:14:56 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/28 17:14:56 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "redirects.h"

/**
 * Redirects the output to a location pointed by cmd_data->output.file
 *
 * @param	cmd_data		Current command and it's attributes
 * @param	minishell		Data for minishell
 */
t_bool	redirect_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
	char	*path;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		return (new_set_exit_status(1, NULL));
	if (chdir(path) == -1)
		return (new_set_exit_status(1, NULL));
	fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		return (new_set_exit_status(1, "no such file or directory"));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (new_set_exit_status(1, NULL));
	return (true);
}

t_bool	append_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
	char	*path;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		return (new_set_exit_status(1, NULL));
	if (chdir(path) == -1)
		return (new_set_exit_status(1, NULL));
	fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
	{
		return (new_set_exit_status(1, "no such file or directory"));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (new_set_exit_status(1, NULL));
	return (true);
}
