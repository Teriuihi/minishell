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
t_bool	read_input_write(t_cmd_data *cmd_data, int old_pid[2], int cur_pid[2],
						t_minishell *minishell)
{
	char		*input;

	g_signal.heredoc = true;
	if (close_pipes(&old_pid[0], &old_pid[1]) == false)
	{
		ft_printf(1, "close routine failed in readinputwrite\n");
		return (set_exit_status(minishell, 1, NULL, false));
	}
	if (pipe(old_pid) == -1)
		return (set_exit_status(minishell, 1, NULL, false));
	input = readline("heredoc> ");
	if (signal_check(input, minishell) == false)
		return (set_exit_status(minishell, 1, NULL, false));
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		ft_putstr_fd(ft_strjoin(input, "\n"), old_pid[1]);
		input = readline("heredoc> ");
		if (signal_check(input, minishell) == false)
			return (set_exit_status(minishell, 1, NULL, false));
	}
	g_signal.heredoc = false;
	if (close_pipes(&old_pid[1], NULL) == false)
		return (set_exit_status(minishell, 1, NULL, false));
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
	char	*message;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		return (set_exit_status(minishell, 1, NULL, false));
	if (chdir(path) == -1)
		return (set_exit_status(minishell, 1, NULL, false));
	fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		message = ft_strdup("no such file or directory");
		return (set_exit_status(minishell, 1, message, true));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (set_exit_status(minishell, 1, NULL, false));
	return (true);
}

t_bool	append_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
	char	*path;
	char	*message;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		return (set_exit_status(minishell, 1, NULL, false));
	if (chdir(path) == -1)
		return (set_exit_status(minishell, 1, NULL, false));
	fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
	{
		message = ft_strdup("no such file or directory");
		return (set_exit_status(minishell, 1, message, true));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (set_exit_status(minishell, 1, NULL, false));
	return (true);
}

/**
 * Redirect the contents of a file to a pipe
 *
 * @param	command		Current command
 * @param	minishell	Data for minishell
 */
t_bool	redirect_file(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
						t_minishell *minishell)
{
	char	buffer[1000];
	char	*path;
	char	*message;
	int		fd;
	int		len;

	path = get_pwd(minishell);
	if (path == NULL)
		return (set_exit_status(minishell, 1, NULL, false));
	if (chdir(path) == -1)
		return (set_exit_status(minishell, 1, NULL, false));
	if (close_pipes(&old_pid[0], &old_pid[1]) == false)
	{
		ft_printf(1, "close routine failed in redirectfile\n");
		return (set_exit_status(minishell, 1, NULL, false));
	}
	if (pipe(old_pid) == -1)
		return (set_exit_status(minishell, 1, NULL, false));
	fd = open(cmd_data->input.file, O_RDONLY);
	if (fd < 0)
	{
		message = ft_strjoin("some shell: ", ft_strjoin(cmd_data->input.file, ": No such file or directory\n"));
		return (set_exit_status(minishell, 1, message, true));
	}
	len = 1000;
	while (len == 1000)
	{
		ft_bzero(buffer, sizeof(char) * 1000);
		len = read(fd, buffer, 1000);
		if (len == -1)
			return (set_exit_status(minishell, 1, NULL, false));
		if (write(old_pid[1], buffer, len) == -1)
			return (set_exit_status(minishell, 1, NULL, false));
	}
	if (close_pipes(&old_pid[1], NULL) == false)
	{
		ft_printf(1, "tried to close pipe inside redirect file\n");
		return (set_exit_status(minishell, 1, NULL, false));
	}
	return (true);
}

t_bool	close_pipes(int *pid1, int *pid2)
{
	if (pid1 != NULL)
	{
		if (*pid1 != -1)
		{
			if (close(*pid1) == -1)
			{
				ft_printf(2, "closing err pid2\n");
				*pid1 = -1;
			}
			else
			{
				*pid1 = -1;
			}
		}
	}
	if (pid2 != NULL)
	{
		if (*pid2 != -1)
		{
			if (close(*pid2) == -1)
			{
				ft_printf(2, "closing err pid2\n");
				*pid2 = -1;
			}
			else
			{
				*pid2 = -1;
			}
		}
	}
	return (true);
}
