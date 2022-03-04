/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_commands.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 19:14:49 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/09 19:14:49 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"

/**
 * Stores a command in command data, on error command is freed
 * 	all other command data isn't freed
 *
 * @param	command	Command to add
 * @param	t_list	All data related to commands
 *
 * @return	non zero on error, 0 on success
 */
t_cmd_data	*store_command(t_cmd_data *cmd_data, t_list **head)
{
	t_list	*new;
	char	*entry;

	new = ft_lstnew(cmd_data); //????? new->content = cmd_data
	if (new == NULL)
	{
		free(cmd_data->command->command); //??
		entry = *cmd_data->command->args;
		while (*entry)
		{
			free(entry);
			entry++;
		}
		free(cmd_data->input.file);
		free(cmd_data->output.file);
		free(cmd_data);
		return (err_ptr_return("Not enough memory.", NULL));
	}
	ft_lstadd_back(head, new); //actually the head will not be a simple list but a cmd_data
	return (cmd_data);
}

t_bool	store_args(char **args, int len, t_command *command)
{
	int		i;
	char	*entry;

	i = -1;
	while (++i < len)
	{
		entry = args[i];
		command->args[i] = ft_strdup(entry);
		if (command->args[i] == NULL)
		{
			free(command->command);
			entry = *command->args;
			while (*entry)
			{
				free(entry);
				entry++;
			}
			free(command);
			return (err_int_return("Not enough memory.", false));
		}
	}
	return (true);
}

/**
 * Creates a new command from the given parameters, by duplicating them
 * 	frees created data on failure
 *
 * @param	pipe_type	What type of redirect should be
 * 	used when executing this command
 * @param	args		All arguments
 * @param	start_pos	Start of arguments for this command in args
 * @param	len			Amount of arguments after start that belong to
 * 	this command
 *
 * @return	non zero on error, 0 on success
 */
t_cmd_data	*create_command_data(char **args, int len)
{
	t_cmd_data	*cmd_data;
	t_command	*command;

	cmd_data = ft_calloc(1, sizeof(t_cmd_data));
	if (!cmd_data)
		return (err_ptr_return("Not enough memory.", NULL));
	cmd_data->command = ft_calloc(1, sizeof(t_command));
	if (!cmd_data)
		return (err_ptr_return("Not enough memory.", NULL));
	cmd_data->command->command = ft_strdup(*args);; //cant write here?
	command = (cmd_data->command);
	if (!command->command)
	{
		free(command);
		return (err_ptr_return("Not enough memory.", NULL));
	}
	command->args = ft_calloc(len + 1, sizeof(char *));
	if (!command->args)
	{
		free(command->command);
		free(command);
		return (err_ptr_return("Not enough memory.", NULL));
	}
	if (store_args(args, len, command) == false)
	{
		return (NULL);
	}
	//print_splitted(cmd_data->command->args);
	//ft_printf("ARE TO NEWLY STORED ARGS\n");
	command->args_len = len;
	return (cmd_data);
}

/**
 * Loop through args until the end is reached or a command separator is found
 *
 * @param	args	Arguments to loop through
 *
 * @return	Amount of arguments until separator is found
 */
int	len_till_seperator(char **args)
{
	int	i;

	i = 0;
	while (args[i] && !command_separator_type(args[i]))
		i++;
	return (i);
}

/**
 * Create a command from a list of arguments, length, and a pipe type
 *
 * @param	head		List containing all commands
 * @param	args		Arguments to create command from
 * @param	len			Amount of arguments to add to command
 * @param	pipe_type	Pipe type that command pipes to
 *
 * @return	True if command was created successfully
 */
t_cmd_data	*create_cmd_from_args(t_list **head, char **args, int len)
{
	t_cmd_data	*cmd_data;

	cmd_data = create_command_data(args, len);
	//UNTIL HERE ALL GOOD
	if (!cmd_data)
		return (NULL);
	return (store_command(cmd_data, head));
}

t_bool	set_input(t_list **head, t_cmd_data *cmd_data)
{
	t_list		*prev_list_entry;
	t_cmd_data	*prev_cmd_data;

	prev_list_entry = ft_lstlast(*head)->prev;
	if (!prev_list_entry)
		return (true);
	prev_cmd_data = prev_list_entry->content;
	cmd_data->input.type = prev_cmd_data->output.type;
	if (prev_cmd_data->output.file)
	{
		cmd_data->input.file = ft_strdup(prev_cmd_data->output.file);
		if (cmd_data->input.file == NULL)
			return (false);
	}
	return (true);
}

/**
 * Should be called when > or >> separator type is found
 * 	creates two commands, one to write/append to file and
 * 	one command that can be anything depending on what the user specified
 *
 * @param	head		List containing all commands
 * @param	args		All arguments given by user
 * @param	start_pos	Start pos of current command
 * @param	len			Length of current command
 *
 * @return	True if commands were created successfully
 */
t_bool	output_file_command(t_list **head, char **args, int *start_pos,
	int *len)
{
	t_pipe_type	pipe_type;
	char		**new_args;
	t_cmd_data	*cmd_data;
	int			args_after;

	pipe_type = command_separator_type(args[*start_pos]);
	if (pipe_type) //command format is [>/>>] file cmd args
	{
		*len = len_till_seperator(args + (*start_pos) + 2);
		cmd_data = create_cmd_from_args(head, args + (*start_pos) + 2, *len);
		if (!cmd_data)
			return (false);
		cmd_data->output.type = pipe_type;
		cmd_data->output.file = ft_strdup(args[*start_pos + 1]); //TODO free created cmd_data and return false
		set_input(head, cmd_data); // TODO free output.file and cmd_data and return false
		*start_pos += 2 + *len + 1;
		*len = 0;
		return (true);
	}
	else //command format is cmd args [>/>>] file [args]
	{
		args_after = len_till_seperator(args + (*start_pos) + (*len) + 2);
		new_args = ft_calloc(args_after + (*len), sizeof(char *));
		if (!new_args)
			return (false);
		ft_memcpy(new_args, args + (*start_pos), (*len) * sizeof(char *));
		ft_memcpy(new_args + (*len), args + (*start_pos) + (*len)
			+ 2, args_after * sizeof(char *));
		cmd_data = create_cmd_from_args(head, new_args, args_after);
		free(new_args);
		if (cmd_data == NULL)
			return (false);
		cmd_data->output.type = command_separator_type(args[*start_pos + *len]);
		cmd_data->output.file = ft_strdup(args[*start_pos + *len + 1]); //TODO free created cmd_data and return false
		set_input(head, cmd_data); // TODO free output.file and cmd_data and return false
		*start_pos += (*len) + args_after + 2;
		*len = 0;
		return (true);
	}
}

/**
 * Handles command creation for all pipe_type's not handled by input_pipe_command
 *
 * @param	t_list		All data related to commands
 * @param	args		All arguments
 * @param	start_pos	Start of arguments for this command in args
 * @param	len			Amount of arguments after start that belong to
 * 	this command
 *
 * @return	non zero on error, 0 on success
 */
t_bool	output_pipe_command(t_list **head, char **args, int *start_pos,
	int *len)
{
	t_pipe_type	pipe_type;
	t_cmd_data	*cmd_data;

	pipe_type = command_separator_type(args[(*start_pos) + (*len)]);
	if (pipe_type == NONE || pipe_type == OUTPUT_TO_COMMAND)
	{
		cmd_data = create_cmd_from_args(head, args + *start_pos, *len);
		if (!cmd_data)
			return (false);
		cmd_data->output.type = pipe_type;
		set_input(head, cmd_data); // TODO free cmd_data and return false
		*start_pos += (*len) + 1;
		*len = 0;
		return (true);
	}
	else
		return (output_file_command(head, args, start_pos, len));
}

/**
 * Set the output based on args (should start at end of prev command)
 *
 * @param	output	output to set
 * @param	args	arguments starting at end of last command
 *
 * @return	Amount of args to skip to get to next command
 * 			or -1 on error
 */
int	set_output(t_redirect *output, char **args) {//TODO use t_bool
	int			i;
	t_pipe_type	pipe_type;

	i = 0;
	while (args[i])
	{
		pipe_type = command_separator_type(args[i]);
		if (pipe_type == REDIRECT_OUTPUT || pipe_type == APPEND_OUTPUT)
		{
			ft_printf("CREATE FILE with next args %s\n", args[i + 1]);
			i += 2;
			if (args[i] && command_separator_type(args[i]))
				continue ;
			output->type = pipe_type;
			output->file = ft_strdup(args[i - 1]);// TODO return false
			ft_printf("OUTPUT FILE of %s is %s\n", output->file, args[i + 1]);
			if (!output->file)
				return (-1);
			return (i);
		}
		else if (pipe_type == REDIRECT_INPUT || pipe_type == DELIMITER_INPUT)
		{
			output->type = NONE;
			return (i);
		}
		else
		{
			output->type = pipe_type;
			return (i + 1);
		}
	}
	output->type = pipe_type;
	return (i);
}

/**
 * Handles creating commands for types that need to create input and forward it
 * 	to the next command
 *
 * @param	t_list		All data related to commands
 * @param	args		All arguments
 * @param	start_pos	Start of arguments for this command in args
 * @param	len			Amount of arguments after start that belong to
 * 	this command
 *
 * @return	non zero on error, 0 on success
 */
t_bool	input_pipe_command(t_list **head, char **args, int *start_pos, int *len)
{
	t_pipe_type	pipe_type;
	t_cmd_data	*cmd_data;
	int			args_after;
	char		**new_args;
	int			len_till_sep;
	int			cmd_start;
	int			end_cmd;

	//write(1, "IN INPUT PIPE COMMAND\n", 23);
	pipe_type = command_separator_type(args[(*start_pos) + (*len)]);
	if (*len == 0)
	{
		ft_printf("WE ARE IN INPUT PIPE COMMAND LEN == 0\n");
		*len += 2;
		if (args[(*start_pos) + *len] == NULL)
			return (err_int_return("parse error INPUT\n", -1));
		if (command_separator_type(args[(*start_pos) + 2]))
			cmd_start = 3;
		else
			cmd_start = 2;
		len_till_sep = len_till_seperator(args + *start_pos + cmd_start);
		cmd_data = create_cmd_from_args(head, args + *start_pos + cmd_start, len_till_sep);
		if (!cmd_data)
			return (false);
		cmd_data->input.type = pipe_type;
		cmd_data->input.file = ft_strdup(args[*start_pos + 1]); // TODO free cmd_data and return false
		if (!cmd_data->input.file)
			return (false);
		end_cmd = set_output(&(cmd_data->output), args + *start_pos + cmd_start + len_till_sep); //TODO on fail free cmd_data and return false (might need to pass an int pointer for end_cmd)
		if (end_cmd < 0)
			return (false);
		*start_pos += cmd_start + len_till_sep + end_cmd;
		if (cmd_data->output.type == APPEND_OUTPUT || cmd_data->output.type == REDIRECT_OUTPUT)
			*start_pos += 1;
		if (command_separator_type(args[*start_pos]))
			*start_pos += 1;
		*len = 0;
		return (true);
	}
	else
	{
		//ft_printf("WE ARE IN INPUT PIPE COMMAND else, %d LEN, %d STARTPOS\n", *len, *start_pos);
		//print_splitted(args);
		//ft_printf("\nare the current args in input pipe\n");

		args_after = len_till_seperator(args + (*start_pos) + (*len) + 2);
		//ft_printf("\n %d is args after len in input pipe\n", args_after);
		new_args = ft_calloc(args_after + (*len), sizeof(char *));
		if (!new_args)
			return (false);
		ft_memcpy(new_args, args + (*start_pos), (*len) * sizeof(char *));
		ft_memcpy(new_args + (*len), args + (*start_pos) + (*len)
			+ 2, args_after * sizeof(char *)); //what is this doing?
		//ft_printf("\nare the NEW ARGS in input pipe\n"); //essentially only cat
		pipe_type = command_separator_type(args[*start_pos + *len]);
		cmd_data = create_cmd_from_args(head, new_args, args_after);
		//print_splitted(cmd_data->command->args);
		//ft_printf("\nare the CMD DATA COMMAND ARGS\n"); //essentially only cat
		free(new_args);
		if (!cmd_data)
			return (false);
		cmd_data->input.type = pipe_type;
		cmd_data->input.file = ft_strdup(args[*start_pos + *len + 1]);
		//ft_printf("ELSE, CMDDATA %s, input type %d, %s input file\n", cmd_data->command->command, cmd_data->input.type, cmd_data->input.file);

		if (!cmd_data->input.file) //TODO free cmd_data
			return (false);
		end_cmd = set_output(&cmd_data->output, args + *start_pos + *len + 2 + args_after); //TODO on fail free cmd_data and return false (might need to pass an int pointer for end_cmd)
		*start_pos += (*len) + 2 + args_after + end_cmd;
		*len = 0;
		return (true);
	}
}

/**
 * Starts looping through all arguments to find the commands
 *
 * @param	t_list	All data related to commands
 * @param	args	All arguments
 *
 * @return	non zero on error, 0 on success
 */
t_bool	find_commands_in_args(t_list **head, char **args)
{
	t_pipe_type	pipe_type;
	int			len;
	t_bool		success;
	int			start_pos;

	success = true;
	start_pos = 0;
	len = 0;
	while (args[start_pos + len])
	{
		pipe_type = command_separator_type(args[start_pos + len]);
		//ft_printf("%d is pipetype atm in the loop\n", pipe_type);
		if (pipe_type == DELIMITER_INPUT
			|| pipe_type == REDIRECT_INPUT)
			{
				//ft_printf("%d is len in find commands in args first if statement \n", len);
				success = input_pipe_command(head, args, &start_pos, &len);
			}
		else if (pipe_type)
			success = output_pipe_command(head, args, &start_pos, &len);
		else
			len++;
		if (success == false)
			return (success);
	}
	if (len != 0)
		success = output_pipe_command(head, args, &start_pos, &len);
	return (success);
}

/**
 * Finds all the commands and stores them in t_list
 *
 * @param	args	Arguments to create the commands from
 *
 * @return	NULL on error, command data success
 */
t_list	**find_commands(char **args) //TODO REMINDER IF COMMAND IS NULL YOU MIGHT NEED TO EXEC PIPE
//TODO or command can be a < b mayb???
{
	t_list	**head;

	if (args == NULL)
		return (NULL);
	head = ft_calloc(1, sizeof(t_list *)); //?
	if (!head)
		return (NULL);
	if (find_commands_in_args(head, args) == false)
	{
		free_commands(head);
		return (NULL);
	}
	return (head);
}
