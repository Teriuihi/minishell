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
		const int *cur_pid, t_data *data)
{
	start_child(old_pid, cur_pid);
	if (execute_builtin(command, data))
		ft_printf("Unable to execute command: %s\n", command->command);
	exit(0);
}

void	child_external(t_command *command, const int *old_pid,
		const int *cur_pid, t_data *data)
{
	start_child(old_pid, cur_pid);
	if (execve(command->command, command->args, get_envp(data->env)) < 0) //here we should pass instead of NULL an array of strings for env variable
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
			t_bool is_built_in, t_data *data)
{
	pid_t	c_pid;

	if (ft_streq(command->command, "exit"))
		exit (0);
	if (is_built_in == false)
	{
		command->command = search_in_path(command->command); //this is where we could just pull from hashtable
		if (command->command == NULL)
		{
			ft_printf("Command not found: %s\n", *command->args);
			return ;
		}
		*command->args = command->command;
	}
	c_pid = fork();
	if (c_pid == 0) //only in this case do we pass the child_htable
	{
		if (is_built_in == true)
			child_built_in(command, old_pid, cur_pid, data);
		else
			child_external(command, old_pid, cur_pid, data);
	}
	else
		parent(c_pid, old_pid);
}
