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
#include <readline/readline.h>
#include <unistd.h>
#include <fcntl.h>
#include "../buildins/buildins.h"
#include "../run_commands/run_commands.h"
#include <sys/wait.h>

/**
 * Assign pid's to STDOUT or STDIN as needed
 * Close unused file descriptors for child
 * Reset wd to current working directory
 *
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	type		Type of pipe for this child process
 * @param	minishell	Data for minishell
 */
void	init_child(const int *old_pid, const int *cur_pid, t_pipe_type type,
					t_minishell *minishell)
{
	char	*cur_dir;
	if (old_pid[0] > -1)
	{
		dup2(old_pid[0], STDIN_FILENO);
		close(old_pid[0]);
		close(old_pid[1]);
	}
	if (cur_pid[0] > -1)
	{
		if (type != DELIMITER_INPUT)
		{
			dup2(cur_pid[1], STDOUT_FILENO);
			close(cur_pid[1]);
		}
		close(cur_pid[0]);
	}
	cur_dir = get_pwd(minishell); //does this work as it should?
	chdir(cur_dir);
}

/**
 * Read input using readline until delimiter is found and write it to pipe
 *
 * @param	command		Current command
 * @param	write_pid	PID to write to
 */
void	read_input_write(t_cmd_data *cmd_data, int old_pid[2], int cur_pid[2], t_minishell *minishell)
{
	char		*input;
	t_command	*command;

	(void)minishell;
	command = cmd_data->command;
	if (old_pid[0])
	{
		dup2(old_pid[0], STDIN_FILENO);
		close(old_pid[0]);
		close(old_pid[1]); //this one is not inited when trying to close it
	}
	input = readline("heredoc> ");
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		ft_putstr_fd(ft_strjoin(input, "\n"), cur_pid[1]); 
		input = readline("heredoc> ");
	}
	close(cur_pid[1]);
}
/* prob not needed
//if (cur_pid[0]) //are both of these need to be executed? //this is parent where there is no OLDPID
	//{
		//dup the cur pids?
		//PROB WE CANT CLOSE THIS JUST YET
		//close(cur_pid[0]); //this is where second process reads?
		//ft_printf("%d is type at cur pid, CURPID[0] == %d, CURPID[1] == %d\n", type, cur_pid[0], cur_pid[1]);
		//close(cur_pid[0]);
	//}
*/


/**
 * Redirects the output to a location pointed by cmd_data->output.file
 *
 * @param	cmd_data		Current command and it's attributes
 * @param	minishell		Data for minishell
 */
void	redirect_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
		char	*path;
		int		fd;

		path = get_pwd(minishell);
		if (path == NULL)
			err_exit("out of memory", 0);
		chdir(path);
		fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_TRUNC, 0777) ;
		if (fd < 0)
			err_exit("no such file or directory", 0);
		dup2(fd, STDOUT_FILENO);
}

void	append_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
		char	*path;
		int		fd;

		path = get_pwd(minishell);
		if (path == NULL)
			err_exit("out of memory", 0);
		chdir(path);
		fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_APPEND, 0777) ;
		if (fd < 0)
			err_exit("no such file or directory", 0);
		dup2(fd, STDOUT_FILENO); //fd is gonna be now stdout
}

/**
 * Redirect the contents of a file to a pipe
 *
 * @param	command		Current command
 * @param	minishell	Data for minishell
 */
void	redirect_file(t_cmd_data *cmd_data, int *old_pid, int *cur_pid, t_minishell *minishell)
{
	char	buffer[1000];
	char	*path;
	int		fd;
	int		len;

	(void)cur_pid;
	path = get_pwd(minishell);
	if (path == NULL)
		err_exit("out of memory", 0);
	chdir(path);
	if (old_pid[0])
	{
		close(old_pid[0]);
		close(old_pid[1]);
	}
	pipe(old_pid); 
	fd = open(cmd_data->input.file, O_RDONLY);
	if (fd < 0)
		err_exit("no such file or directory", 0);
	len = 1000;
	while (len == 1000)
	{
		ft_bzero(buffer, sizeof(char) * 1000);
		len = read(fd, buffer, 1000);
		write(old_pid[1], buffer, len);
	}
	close(old_pid[1]);
}

/**
 * Execute a build in command
 * 	should only be called from child process
 *
 * @param	command		Current command
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	minishell	Data for minishell
 */
void	child_execute_built_in(t_cmd_data *cmd_data, const int *old_pid,
								const int *cur_pid, t_minishell *minishell)
{
	t_command	*command;

	command = cmd_data->command; //this is also prob not needed &cmd_data->command; 
	init_child(old_pid, cur_pid, cmd_data->output.type, minishell); //SOMETHING GOES WRONG HERE
	//if (cmd_data->input.type == REDIRECT_INPUT) // ? cmd_data->output.type was before
	//{
	//	write(1, "in redirect input\n", 19);

	//	redirect_file(cmd_data, minishell);
		//redirect_file(command, minishell);
	//	exit(0);
	//}
	if (cmd_data->output.type == DELIMITER_INPUT) // Enters here but why?
	{
		//ft_printf("%s is cmd command, %d is output type, %d is input type\n", cmd_data->command->command, cmd_data->output.type, cmd_data->input.type);
		//read_input_write(command, cur_pid[1]);
		exit(0);
	}
	if (execute_builtin(command, minishell) == false)
		ft_printf("Unable to execute command: %s\n", command->command);
	exit(0);
}

void	init_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid, t_minishell *minishell)
{
//	int fd;
	//control some pipes here as well?
	if (cmd_data->input.type)
	{
		if (cmd_data->input.type == REDIRECT_INPUT)
		{
			redirect_file(cmd_data, old_pid, cur_pid, minishell);
		}
		if (cmd_data->input.type == DELIMITER_INPUT)
		{
			read_input_write(cmd_data, old_pid, cur_pid, minishell);
		}
	}
}
/* USED TO BE MAYBE NOT NECESSARY
	if (cmd_data->output.type == REDIRECT_OUTPUT)
	{
		//if (old_pid[0])
		//{
		//	close(old_pid[0]);
		//	close(old_pid[1]);
		//}
		fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_TRUNC, 0777) ;
		if (fd < 0)
			err_exit("no such file or directory", 0);
		dup2(STDOUT_FILENO, fd);
		close(STDOUT_FILENO);
		//dup2()
		//close(fd);
		//close(fd);
	}
	*/
	//if (cmd_data->output.type == APPEND_OUTPUT) //or redirect output?
	//{
		//sort of redirect file
//}

/*
void	open_dup_close(int to_close1, int to_close , int *dup_src, int dup_dst)
{
	//dup2(dup_src, dup_dst);
	
}
*/
void	print_pid(int *old_pid, int *cur_pid)
{
	ft_printf("%d oldpid[0], %d oldpid[1], %d curpid[0], %d curpid[1]\n", old_pid[0],old_pid[1],cur_pid[0],cur_pid[1]);
}

void	control_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid, t_minishell *minishell)
{
	char	*cur_dir;

	//ft_printf("Controlling pipes with command %s, output %d, input %d\n", cmd_data->command->command, cmd_data->output.type, cmd_data->input.type);
	if (cmd_data->input.type == OUTPUT_TO_COMMAND)
	{
		//write(1, "WRONG\n", 7);
		//ft_printf("%s is command and its suppose to be OUTPUT_TO_COMMAND\n", cmd_data->command->command);
		
		dup2(cur_pid[0], STDIN_FILENO);
		close(cur_pid[0]);
		close(cur_pid[1]);
	}
	if (cmd_data->input.type == DELIMITER_INPUT)
	{
		dup2(cur_pid[0], STDIN_FILENO);
		close(cur_pid[0]);
		close(cur_pid[1]);
	}
	if (cmd_data->input.type == REDIRECT_INPUT)
	{
		dup2(old_pid[0], STDIN_FILENO); //this for cat? cat < file.txt
		close(old_pid[0]); // ??? IS THIS NEEDED?
	}
	if (cmd_data->output.type == OUTPUT_TO_COMMAND)
	{
		if (old_pid[0] > -1)
		{
			dup2(old_pid[0], STDIN_FILENO);
			close(old_pid[0]);
			close(old_pid[1]);
		}
		if (cur_pid[0] > -1)
		{
			dup2(cur_pid[1], STDOUT_FILENO);
			close(cur_pid[1]);
			close(cur_pid[0]);
		}
	}
	if (cmd_data->output.type == REDIRECT_OUTPUT)
	{
		redirect_output(cmd_data, minishell);
	}
	if (cmd_data->output.type == APPEND_OUTPUT)
	{
		append_output(cmd_data, minishell);
	}
	cur_dir = get_pwd(minishell); //does this work as it should?
	chdir(cur_dir);
}
/**
 * Execute an external command
 * 	should only be called from child process
 *
 * @param	command		Current command
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	minishell	Data for minishell
 */
void	child_execute_external(t_cmd_data *cmd_data, const int *old_pid,
								const int *cur_pid, t_minishell *minishell)
{
	t_command	*command;

	command = cmd_data->command;
	control_pipes(cmd_data, (int *)old_pid, (int *)cur_pid, minishell);
	if (execve(command->command, command->args,
		get_envp(minishell->env)) < 0)
	{
		ft_printf("Unable to execute command: %s\n", command->command);
		close(old_pid[0]);
		exit(0);
	}
}
//ft_printf("before control pipes with command %s, output %d, %d input\n", command->command, cmd_data->output.type, cmd_data->input.type);


/**
 * Code to be ran by parent after fork
 * 	closes unused PIDs
 * 	waits for child to finish
 *
 * @param	c_pid	PID of fork
 * @param	old_pid	PIDs from previous pipes
 */
void	parent(pid_t c_pid, const int *old_pid)
{
	int	status;

	if (old_pid[0] > -1)
	{
		close(old_pid[1]);
		close(old_pid[0]);
	}
	waitpid(c_pid, &status, 0);
}

/**
 * Check if a command should be executed in a child process
 *
 * @param	command	Command to check
 *
 * @return	True if it should be executed in a child process, false if not
 */
t_bool	should_be_child(t_command *command)
{
	if (env_variable_found(command->command) == true) //what happens if its false but because of incorrect input? hello==myvar
		return (false);
	if (ft_streq(command->command, "cd"))
		return (false);
	if (ft_streq(command->command, "unset"))
		return (false);
	return (true);
}


static t_bool	search_executable_for_non_builtin(t_cmd_data *cmd_data)
{
	t_command *command;
	
	command = cmd_data->command;
	command->command = search_in_path(command->command); //this is where we could just pull from hashtable
	if (command->command == NULL)
	{
		ft_printf("Command not found: %s\n", *command->args);
		return (false);
	}
	free(*command->args); //do we still have command args?
	*command->args = ft_strdup(command->command);
	return (true);
}

/**
 * Execute a command
 *
 * @param	command		Command to execute
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	is_built_in	If a command is a built in command or not
 * @param	minishell	Data for minishell
 */
void	exec_command(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
			t_bool is_built_in, t_minishell *minishell)
{
	pid_t		c_pid;
	t_command	*command;

	command = cmd_data->command;
	if (ft_streq(command->command, "exit"))
		exit (0);
	init_pipes(cmd_data, old_pid, cur_pid, minishell);
	if (is_built_in == false) //if its not a builtin command
	{
		if (search_executable_for_non_builtin(cmd_data) == false)
			return ;
	}
	if (is_built_in == true && should_be_child(command) == false)
	{
		child_execute_built_in_not_child(command, minishell);
		return ;
	}
	c_pid = fork();
	if (c_pid == 0)
	{
		if (is_built_in == true) //we have to make this somehow true
		{
			ft_printf("wrong place is builtint true\n");
			child_execute_built_in(cmd_data, old_pid, cur_pid, minishell);
		}
		else
			child_execute_external(cmd_data, old_pid, cur_pid, minishell); //here first we have to enter the built in, organize pipes then execute it
	}
	else
		parent(c_pid, old_pid);
}
