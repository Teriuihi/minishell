/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:22:29 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:22:29 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"
#include <readline/history.h>
#include <sys/stat.h>
#include <unistd.h>

extern char	**environ;

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	i;

	dst_len = ft_strlen(dst) >= dstsize ? dstsize : ft_strlen(dst);
	i = 0;
	if (dstsize == dst_len || dstsize == 0)
		return (dstsize + ft_strlen(src));
	if (ft_strlen(src) >= dstsize - dst_len)
	{
		while (i < dstsize - dst_len - 1)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	else
	{
		while (src[i] && i < dstsize - dst_len)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	dst[dst_len + i] = '\0';
	return (dst_len + ft_strlen(src));
}

void	free_input_args(char *input, char **args)
{
	char	**tmp;

	tmp = args;
	free(input);
	while (*args)
	{
		free(*args);
		args++;
	}
	free(tmp);
}

int	eval(int read_pid, t_command *command)
{
	pid_t	cpid;
	int		pid[2];
	char	**tmp; //TODO remove

	write(1, "-3\n", 3);
	if (is_builtin(command->command) == true)
	{
		if (command->type == NONE)
			execute_builtin(command, -1);
		else if (command->type == OUTPUT_TO_COMMAND)
		{
			pipe(pid);
			dup2(pid[1], 1);
			//TODO start listening on another thread
			execute_builtin(command, pid[0]);
			if (read_pid != -1)
				close(read_pid);
			close(pid[1]);
		}
		if (command->type == OUTPUT_TO_COMMAND)
			return (pid[0]);
		else
			return (-1);
	}
	write(1, "-2\n", 3);
	if (command->type == OUTPUT_TO_COMMAND)
	{
		pipe(pid);
		dup2(1, pid[1]);
		//TODO start listening on another thread
	}
	write(1, "-1\n", 3);
	cpid = fork();
	write(1, "0\n", 2);
	if (cpid == 0)
	{
		write(1, "1\n", 2);
		if (read_pid != -1)
			dup2(0, read_pid);
		write(1, "2\n+", 3);
		ft_putnbr_fd(command->type, 1);
		write(1, "\n", 1);
		write(1, command->command, ft_strlen(command->command));
		write(1, "\n", 1);
		tmp = command->args;
		while (tmp)
		{
			write(1, *tmp, ft_strlen(*tmp));
			write(1, "\n", 1);
			tmp++;
		}
		if (execve(command->command, command->args, NULL) < 0)
		{
			write(1, "3\n", 2);
			ft_printf("Command not found\n");
			exit(0);
		}
		else if (command->type == 1)
		{
			write(1, "4\n", 2);
			if (read_pid != -1)
				close(read_pid);
			close(pid[1]);
		}
	}
	else
	{
		write(1, "5\n", 2);
		wait(NULL); //its not guaranteed that the child process will execute first.
	}
	write(1, "6\n", 2);
	if (command->type == OUTPUT_TO_COMMAND)
		return (pid[0]);
	return (-1);
}

int	main(void)
{
	t_signal	*signal_struct;
	char		*input;
	char		**args;
	int			i;
	t_list		**commands;
	t_list		*entry;
	t_command	*command;
	int			read_pid;

	read_pid = -1;
	signal_struct = init_signal();
	signal(SIGQUIT, sigquit_handler);
	get_pwd(getcwd(NULL, 0)); //TODO check for null
	i = 0;
	input = readline("some shell>");
	while (input)
	{
		add_history(input);
		args = get_args(input);
		if (args == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		commands = find_commands(args);
		if (commands == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		entry = *commands;
		while (entry)
		{
			command = entry->content;
			search_in_path(command->command); //What does this do?
			eval(read_pid, command);
			entry = entry->next;
			free(command->args);
			free(command);
		}
		free_input_args(input, args);
		input = readline("some shell>");
	}
	return (0);
}
