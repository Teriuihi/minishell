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

void	eval(t_command *command_old, t_command *command)
{
	pid_t	cpid;
	int		pid[2];

	if (is_builtin(command->command) == true)
	{
		if (command->type == INVALID)
			execute_builtin(command, -1);
		else if (command->type == OUTPUT_TO_COMMAND)
		{
			pipe(pid);
			dup2(pid[1], 1);
			execute_builtin(command, pid[0]);
		}
		close(1);
		return ;
	}
	if (command->type == OUTPUT_TO_COMMAND)
	{
		pipe(pid);
		dup2(pid[1], 1);
		command->pid = pid[0];
	}
	if (command_old != NULL && command_old->pid > 2)
		dup2(0, command_old->pid);
	cpid = fork();
	if (cpid == 0)
	{
		if (execve(command->command, command->args, NULL) < 0)
		{
			ft_printf("Command not found\n");
			exit(0);
		}
		if (command->type == OUTPUT_TO_COMMAND)
			close(pid[1]);
	}
	else
	{
		wait(NULL); //its not guaranteed that the child process will execute first.
	}
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
	t_command	*command_old;

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
		entry = *commands;
		while (entry)
		{
			command = entry->content;
			search_in_path(command->command); //What does this do?
			eval(command_old, command);
			entry = entry->next;
			command_old = command;
		}
		free_input_args(input, args);
		input = readline("some shell>");
	}
	return (0);
}
