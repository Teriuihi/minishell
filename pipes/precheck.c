/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.h                                       :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor       #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "precheck.h"
#include "redirects.h"

/**
 * Check if a command should be executed in a child process
 *
 * @param	executable			the complete absolute path of the executable
 * @param	should_path_extend	boolean to check if the
 * 				executable root has to be extended
 * @param	command				Command to check
 *
 * @return	True if it should be executed in a child process,
 * 	false if something fails
 */
static t_bool	assign_path_to_command(char *executable,
						t_bool should_path_extend, t_command *command)
{
	free(*command->args);
	*command->args = ft_strdup(command->command);
	if (!command->args)
	{
		return (false);
	}
	if (should_path_extend == true)
	{
		free(command->command);
		command->command = ft_strdup(executable);
		free(executable);
		if (!command->command)
		{
			return (false);
		}
	}
	return (true);
}

/**
 * Search for executable either from PATH or in the current
 * 	working directory/folder
 *
 * @param	cmd_data	struct containing information about the typed in command
 * @param	minishell	minishell struct
 *
 * @return	True if executable found and assigned/extended,
 * 	false if executable not found or assignment failed
 */
static t_bool	search_executable(t_cmd_data *cmd_data,
				t_minishell *minishell)
{
	char		*executable;
	t_command	*command;

	executable = NULL;
	command = cmd_data->command;
	if (ft_streq(command->command, "..") == 1)
		return (false);
	if (command->command != NULL && command->command[0] == '/')
	{
		return (assign_path_to_command(executable, false, command));
	}
	executable = search_in_folder(command->command, minishell);
	if (executable != NULL)
	{
		return (assign_path_to_command(executable, true, command));
	}
	executable = search_in_path(command->command, minishell);
	if (executable != NULL)
	{
		return (assign_path_to_command(executable, true, command));
	}
	else
	{
		return (false);
	}
}

t_bool	pre_fork_check_2(t_cmd_data *cmd_data, t_bool is_built_in,
			t_minishell *minishell)
{
	t_bool		succeeded;
	t_command	*command;

	command = cmd_data->command;
	if (is_built_in == false)
		cmd_data->executable_found = search_executable(cmd_data, minishell);
	if (should_be_child(command) == false)
	{
		succeeded = execute_non_forked_builtin(command, minishell);
		if (succeeded == false && g_signal.print_basic_error == true)
		{
			ft_printf(2, "command not found: %s\n", command->command);
			g_signal.stop_curr_execution = true;
		}
		g_signal.print_basic_error = false;
		return (false);
	}
	return (true);
}

/**
 * Checking function before forking. Also calls execute non-forked builtin
 * 	in case the command shouldn't be forked
 *
 * @param	cmd_data	struct containing information about the typed in command
 * @param	old_pid		previous pipe fds
 * @param	cur_pid		current pipe fds
 * @param	minishell	minishell struct
 *
 * @return	true if it everythng went according to expectations,
 * 	all functions returned true (succeeded), false otherwise
 */
t_bool	pre_fork_check(t_cmd_data *cmd_data, int *old_pid,
			t_minishell *minishell)
{
	t_command	*command;
	t_bool		is_built_in;

	is_built_in = is_builtin(cmd_data->command);
	command = cmd_data->command;
	if (ft_streq(command->command, "exit"))
	{
		if (cmd_data->input.type == NONE && cmd_data->output.type == NONE)
			exit(0);
		else
			return (false);
	}
	if (check_input_redir(cmd_data, old_pid, minishell) == false)
		return (false);
	return (pre_fork_check_2(cmd_data, is_built_in, minishell));
}

/**
 * Checking function before forking. Also calls execute non-forked
 * 	builtin in case the command shouldn't be forked
 *
 * @param	cmd_data	struct containing information about the typed in command
 * @param	old_pid		prevous pipe fds
 * @param	cur_pid		current pipe fds
 * @param	minishell	minishell struct
 *
 * @return	true if it everiythng went according to expectations or there
 * 	was no input redirection, false if redirection failed for any reason
 */
t_bool	check_input_redir(t_cmd_data *cmd_data, int *old_pid,
			t_minishell *minishell)
{
	if (cmd_data->input.type == REDIRECT_INPUT)
	{
		return (redirect_file(cmd_data, old_pid, minishell));
	}
	else if (cmd_data->input.type == DELIMITER_INPUT)
	{
		return (read_input_write(cmd_data, old_pid));
	}
	return (true);
}
