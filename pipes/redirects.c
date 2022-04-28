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

static t_bool	read_input_write_2(t_cmd_data *cmd_data, int old_pid[2],
					char *input, t_string *tmp)
{
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		tmp = append_char_array(tmp, input);
		tmp = append_char_array(tmp, "\n");
		free(input);
		if (tmp == NULL)
			return (false);
		input = readline("heredoc> ");
		if (signal_check(input) == false)
		{
			free(input);
			free_string(tmp);
			return (new_set_exit_status(1, NULL));
		}
	}
	free(input);
	g_signal.heredoc = false;
	if (g_signal.sigint == 0)
		ft_putstr_fd(tmp->s, old_pid[1]);
	free_string(tmp);
	if (close_pipes(&old_pid[1], NULL) == false)
		return (new_set_exit_status(1, NULL));
	return (true);
}

/**
 * Read input using readline until delimiter is found and write it to pipe
 *
 * @param	command		Current command
 * @param	write_pid	PID to write to
 */
t_bool	read_input_write(t_cmd_data *cmd_data, int old_pid[2])
{
	char		*input;
	t_string	*tmp;

	g_signal.sigint = 0;
	g_signal.heredoc = true;
	if (close_pipes(&old_pid[0], &old_pid[1]) == false)
		return (new_set_exit_status(1, NULL));
	if (pipe(old_pid) == -1)
		return (new_set_exit_status(1, NULL));
	input = readline("heredoc> ");
	if (signal_check(input) == false)
	{
		free(input);
		return (new_set_exit_status(1, NULL));
	}
	tmp = init_string(NULL);
	return (read_input_write_2(cmd_data, old_pid, input, tmp));
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
		return (new_set_exit_status(1, NULL));
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
		return (new_set_exit_status(1, NULL));
	if (pipe(old_pid) == -1)
		return (new_set_exit_status(1, NULL));
	fd = open(cmd_data->input.file, O_RDONLY);
	if (fd < 0)
		return (new_set_exit_status(1,
				"some shell: %s: No such file or directory\n",
				cmd_data->input.file));
	return (write_to_file(fd, old_pid));
}
