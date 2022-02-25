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
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

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

	//<< < > >>
	//
	//STDOUT WILL POINT TO CUR PID1
	//if you just write one command like ls, there will be no old pits
	//if there is two then there will be an old pid
	if (old_pid) //are both of these need to be executed?
	{
		//is there old pid in case of cat > file
		//ft_printf("%d is type at old pid, old_pid[0] == %d, old_pid[1] == %d\n", type, old_pid[0], old_pid[1]);
		dup2(old_pid[0], STDIN_FILENO);
		close(old_pid[0]);
		close(old_pid[1]); //this one is not inited when trying to close it
	}
	if (cur_pid) //are both of these need to be executed?
	{
		//ft_printf("%d is type at cur pid, CURPID[0] == %d, CURPID[1] == %d\n", type, cur_pid[0], cur_pid[1]);
		//write(1, "GOOD CUR PID\n", 14);
		if (type != DELIMITER_INPUT) //this is where we are essentially
			dup2(cur_pid[1], STDOUT_FILENO); //
		if (type != DELIMITER_INPUT)
			close(cur_pid[1]);
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
void	read_input_write(t_command *command, int write_pid)
{
	char	*input;

	if (command->args_len != 2 || command->args[1] == NULL)
		err_exit("parse error read input write", 0);
	input = readline("heredoc> ");
	while (input && !ft_streq(input, command->args[1]))
	{
		ft_putstr_fd(input, write_pid);
		input = readline("heredoc> ");
	}
	close(write_pid);
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
	//close any old pids
	if (old_pid[0])
	{
		close(old_pid[0]);
		close(old_pid[1]);
	}
	pipe(old_pid); //this is gonna create a pipe between this and the next process
	//if (command->args_len != 2 || command->args[1] == NULL) //fails here cuz args are not working
	//	err_exit("parse error redirect file", 0);
	//ft_printf("%s is from where we have to get an input\n", cmd_data->input.file);
	fd = open(cmd_data->input.file, O_RDONLY);//open(command->args[1], O_RDONLY); //this arg should be file essentially
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
 * Redirect the contents of a file to a pipe
 *
 * @param	command		Current command
 * @param	minishell	Data for minishell
 */
/*
void	redirect_file(t_cmd_data *cmd_data, t_minishell *minishell)
{
	char	buffer[1000];
	char	*path;
	int		fd;
	int		len;

	//write(1, "entered to the right place in redirect file \n", 46);
	path = get_pwd(minishell);
	if (path == NULL)
		err_exit("out of memory", 0);
	chdir(path);
	
	//if (command->args_len != 2 || command->args[1] == NULL) //fails here cuz args are not working
	//	err_exit("parse error redirect file", 0);
	//ft_printf("%s is from where we have to get an input\n", cmd_data->input.file);
	fd = open(cmd_data->input.file, O_RDONLY);//open(command->args[1], O_RDONLY); //this arg should be file essentially
	if (fd < 0)
		err_exit("no such file or directory", 0);
	len = 1000;
	while (len == 1000)
	{
		ft_bzero(buffer, sizeof(char) * 1000);
		len = read(fd, buffer, 1000);
		write(STDOUT_FILENO, buffer, len); //write(1, buffer, len); <- was before,  we have to write to the read end of cat
	}
}
*/
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

	//ft_printf("in child execute built in \n");
	//write(1, "in child execute built in \n", 28);
	command = cmd_data->command; //this is also prob not needed &cmd_data->command; 
	ft_printf("%s is the command before we init child, %d is the inputtype\n", command->command, cmd_data->input.type);
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
		ft_printf("%s is cmd command, %d is output type, %d is input type\n", cmd_data->command->command, cmd_data->output.type, cmd_data->input.type);
		read_input_write(command, cur_pid[1]);
		exit(0);
	}
	if (execute_builtin(command, minishell) == false)
		ft_printf("Unable to execute command: %s\n", command->command);
	exit(0);
}

void	init_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid, t_minishell *minishell)
{
	if (cmd_data->input.type)
	{
		//check which input type
		if (cmd_data->input.type == REDIRECT_INPUT)
		{
			//prob we need also a pipe?
			redirect_file(cmd_data, old_pid, cur_pid, minishell);
		}
	}
	if (cmd_data->output.type)
	{
		//check which input type
		//if (cmd_data->input.type == REDIRECT_INPUT)
		//{
			//prob we need also a pipe?
		//	redirect_file(cmd_data, old_pid, cur_pid, minishell);
		//}
	}
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

	command = cmd_data->command; // &cmd_data->command; was before cuz command was cmd_data.command
	//init_child(old_pid, cur_pid, cmd_data->output.type, minishell); //should this be with output type or input type?
	//PIPE TYPE DOESNT REALLY WORK HERE
	//ft_printf("%d is pipetype before we pass it to init child\n", cmd_data->output.type);
	//ft_printf("%s is command->command BEFORE getting execve\n", command->command); //cat
	//print_splitted(command->args); //there are no command->args?
	//ft_printf("are the args AFTER getting execve, %d is output type\n", cmd_data->output.type);
	//ft_printf("Execute external, %s is command, %d is type of it\n", cmd_data->command->command, cmd_data->input.type);
	//t_printf("%d oldpid[0], %d oldpid[1], %d curpid[0], %d curpid[1], \n", old_pid[0], old_pid[1], cur_pid[0], cur_pid[1]);
	//INIT CHILD
	//here we could check what is the input type of this
	//ft_printf("REACHED INIT CHILD\n");
	(void)old_pid;
	(void)cur_pid;
	(void)minishell;
	dup2(old_pid[0], STDIN_FILENO);
	//if its a child then we call the function which would execute the program (eg:cat or pwd)
	//write(1, "HEREEEE\n", 9);
	if (execve(command->command, command->args,
			NULL) < 0) //here we should pass instead of NULL an array of strings for env variable
	{
		ft_printf("Unable to execute command: %s\n", command->command);
		close(old_pid[0]);
		exit(0);
	}
	close(old_pid[0]);
}

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

	if (old_pid[0])
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
	//cat 

	command = cmd_data->command; //no need for adress
	//ft_printf("%s is command, %p is redirected file adress, %d is pipetype\n", cmd_data->command->command, cmd_data->input.file, cmd_data->input.type);
	if (ft_streq(command->command, "exit"))
		exit (0);
	//here?
	//hey check input output pipes, init child accordingly
		//check for inputs first
		//then outputs
	init_pipes(cmd_data, old_pid, cur_pid, minishell); //
	if (is_built_in == false)
	{
		command->command = search_in_path(command->command); //this is where we could just pull from hashtable
		if (command->command == NULL)
		{
			ft_printf("Command not found: %s\n", *command->args);
			return ;
		}
		free(*command->args); //do we still have command args?
		*command->args = ft_strdup(command->command);
	}
	//if (is_built_in && should_be_child(command) == false)
	//{
		//only cd, export, unset
	//	child_execute_built_in_not_child(command, minishell);
	//	return ;
	//}
	c_pid = fork();
	if (c_pid == 0) //only in this case do we pass the child_htable
	{
		if (is_built_in == true) //we have to make this somehow true
			child_execute_built_in(cmd_data, old_pid, cur_pid, minishell);
		else
			child_execute_external(cmd_data, old_pid, cur_pid, minishell); //here first we have to enter the built in, organize pipes then execute it
	}
	else
		parent(c_pid, old_pid);
}
