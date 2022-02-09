/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 18:15:27 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/09 18:15:27 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"
#include <sys/stat.h>
#include <unistd.h>

void	start_child(const int *old_pid, const int *cur_pid)
{
	char	*cur_dir;

	if (old_pid)
	{
		dup2(old_pid[0], STDIN_FILENO);
		close(old_pid[0]);
		close(old_pid[1]);
	}
	if (cur_pid)
	{
		dup2(cur_pid[1], STDOUT_FILENO);
		close(cur_pid[1]);
		close(cur_pid[0]);
	}
	cur_dir = get_pwd();
	chdir(cur_dir);
	free(cur_dir);
}

void	child_built_in(t_command *command, const int *old_pid,
		const int *cur_pid)
{
	start_child(old_pid, cur_pid);
	if (command->type == REDIRECT_INPUT)
	{
		//TODO read from file into stdout
		exit(0);
	}
	if (command->type == DELIMITER_INPUT)
	{
		//TODO read from stdin to stdout until command->args[1] is found
		exit(0);
	}
	if (execute_builtin(command))
		ft_printf("Unable to execute command: %s\n", command->command);
	exit(0);
}

void	child_external(t_command *command, const int *old_pid,
		const int *cur_pid)
{
	start_child(old_pid, cur_pid);
	if (execve(command->command, command->args, NULL) < 0)
	{
		ft_printf("Unable to execute command: %s\n", command->command);
		exit(0);
	}
}

void	parent(pid_t c_pid, const int *old_pid)
{
	int	status;

	if (old_pid)
	{
		close(old_pid[1]);
		close(old_pid[0]);
	}
	waitpid(c_pid, &status, 0);
}

void	exec_command(t_command *command, int *old_pid, int *cur_pid,
			int built_in)
{
	pid_t	c_pid;

	if (ft_streq(command->command, "exit"))
		exit (0);
	if (!built_in)
	{
		command->command = search_in_path(command->command);
		if (command->command == NULL)
		{
			ft_printf("Command not found: %s\n", *command->args);
			return ;
		}
		*command->args = command->command;
	}
	c_pid = fork();
	if (c_pid == 0)
	{
		if (built_in)
			child_built_in(command, old_pid, cur_pid);
		else
			child_external(command, old_pid, cur_pid);
	}
	else
		parent(c_pid, old_pid);
}
