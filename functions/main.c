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

int	*eval(int *read_pid, t_command *command)
{
	pid_t	cpid;
	int		*pid;

	pid = ft_calloc(2, sizeof(int));
	if (!pid)
		return (NULL);
	chdir(get_pwd(NULL));
//	if (is_builtin(command->command) == true)
//	{
//		if (command->type == NONE)
//			execute_builtin(command, -1);
//		else if (command->type == OUTPUT_TO_COMMAND)
//		{
//			pipe(pid);
//			dup2(pid[1], 1);
//			//TODO start listening on another thread
//			execute_builtin(command, pid[0]);
//			if (read_pid != -1)
//				close(read_pid);
//			close(pid[1]);
//		}
//		if (command->type == OUTPUT_TO_COMMAND)
//			return (pid[0]);
//		else
//			return (-1);
//	}
	cpid = fork();
	if (cpid == 0)
	{
		if (read_pid)
		{
			ft_putnbr_fd(read_pid[0], 1);
			write(1, "-a\n", 3);
			ft_putnbr_fd(read_pid[1], 1);
			write(1, "-b\n", 3);
		}
		if (read_pid != NULL)
			close(read_pid[1]);
		if (command->type == OUTPUT_TO_COMMAND)
		{
			pipe(pid);
			ft_putnbr_fd(pid[0], 1);
			write(1, "-c\n", 3);
			ft_putnbr_fd(pid[1], 1);
			write(1, "-d\n", 3);
			dup2(pid[1], 1); //write
			dup2(pid[0], 0); //read
		}
		if (execve(command->command, command->args, NULL) < 0)
		{
			ft_printf("Command not found\n");
			exit(0);
		}
		if (read_pid != NULL)
			close(read_pid[0]);
	}
	else
		wait(NULL); //its not guaranteed that the child process will execute first
	if (command->type == OUTPUT_TO_COMMAND)
		return (pid);
	return (NULL);
}

int	main(void)
{
	t_signal	*signal_struct;
	char		*input;
	char		**args;
	t_list		**commands;
	t_list		*entry;
	t_command	*command;
	int			*pid;

	pid = NULL;
	signal_struct = init_signal();
	signal(SIGQUIT, sigquit_handler);
	get_pwd(getcwd(NULL, 0)); //TODO check for null
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
			command->command = search_in_path(command->command);
			*command->args = command->command;
			if (command == NULL)
			{
				ft_printf("Error\n");
				return (0);
			}
			pid = eval(pid, command);
			ft_putnbr_fd(pid[0], 1);
			write(1, "-y\n", 3);
			ft_putnbr_fd(pid[1], 1);
			write(1, "-z\n", 3);
			entry = entry->next;
			free(command->args);
			free(command);
		}
		free_input_args(input, args);
		input = readline("some shell>");
	}
	return (0);
}
