/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.c                                       :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor       #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "redirects.h"

/**
 * Read input using readline until delimiter is found and write it to pipe
 *
 * @param	command		Current command
 * @param	write_pid	PID to write to
 */
t_bool	read_input_write(t_cmd_data *cmd_data, int old_pid[2],
						t_minishell *minishell)
{
	char		*input;

	g_signal.heredoc = true;
	if (close_pipes(&old_pid[0], &old_pid[1]) == false)
		return (new_set_exit_status(1, "close failed readinputwrite\n"));
	if (pipe(old_pid) == -1)
		return (new_set_exit_status(1, NULL));
	input = readline("heredoc> ");
	if (signal_check(input, minishell) == false)
		return (new_set_exit_status(1, NULL));
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		ft_putstr_fd(ft_strjoin(input, "\n"), old_pid[1]);
		input = readline("heredoc> ");
		if (signal_check(input, minishell) == false)
			return (new_set_exit_status(1, NULL));
	}
	g_signal.heredoc = false;
	if (close_pipes(&old_pid[1], NULL) == false)
		return (new_set_exit_status(1, NULL));
	return (true);
}

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

static t_bool	write_to_file(int fd, int *old_pid)
{
	char	buffer[1000];
	int		len;

	len = 1000;
	while (len == 1000)
	{
		ft_bzero(buffer, sizeof(char) * 1000);
		len = read(fd, buffer, 1000);
		if (len == -1)
			return (new_set_exit_status(1, NULL));
		if (write(old_pid[1], buffer, len) == -1)
			return (new_set_exit_status(1, NULL));
	}
	if (close_pipes(&old_pid[1], NULL) == false)
		return (new_set_exit_status(1, "close pipe inside redirect fail\n"));
	return (true);
}

/**
 * Redirect the contents of a file to a pipe
 *
 * @param	command		Current command
 * @param	minishell	Data for minishell
 */
t_bool	redirect_file(t_cmd_data *cmd_data, int *old_pid,
						t_minishell *minishell)
{
	char	*path;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		return (new_set_exit_status(1, NULL));
	if (chdir(path) == -1)
		return (new_set_exit_status(1, NULL));
	if (close_pipes(&old_pid[0], &old_pid[1]) == false)
		return (new_set_exit_status(1, "close failed in redirectfile\n"));
	if (pipe(old_pid) == -1)
		return (new_set_exit_status(1, NULL));
	fd = open(cmd_data->input.file, O_RDONLY);
	if (fd < 0)
		return (new_set_exit_status(1,
				"some shell: %s: No such file or directory\n",
				cmd_data->input.file));
	return (write_to_file(fd, old_pid));
}

t_bool	close_pipes(int *pid1, int *pid2)
{
	if (pid1 != NULL && *pid1 != -1)
	{
		if (close(*pid1) == -1)
		{
			*pid1 = -1;
			return (new_set_exit_status(1, "close error pid1\n"));
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
			return (new_set_exit_status(1, "close error pid2\n"));
		}
		else
		{
			*pid2 = -1;
		}
	}
	return (true);
}
