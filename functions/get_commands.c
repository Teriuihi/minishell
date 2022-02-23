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
t_bool	store_command(t_command *command, t_list **head)
{
	t_list	*new;
	char	*entry;

	new = ft_lstnew(command);
	if (new == NULL)
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
	ft_lstadd_back(head, new);
	return (true);
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
t_command	*create_command(t_pipe_type pipe_type, char **args, int len)
{
	t_command	*command;

	command = ft_calloc(1, sizeof(t_command));
	if (!command)
		return (err_ptr_return("Not enough memory.", NULL));
	command->command = ft_strdup(*args);
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
		return (NULL);
	command->type = pipe_type;
	command->args_len = len;
	return (command);
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
t_bool	create_command_from_args(t_list **head, char **args, int len,
	t_pipe_type pipe_type)
{
	t_command	*command;

	command = create_command(pipe_type, args, len);
	if (!command)
		return (false);
	return (store_command(command, head));
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
	int			args_after;
	char		**new_args;

	pipe_type = command_separator_type(args[*start_pos]);
	if (pipe_type) //command format is [>/>>] file cmd args
	{
		*len = 0;
		args_after = len_till_seperator(args + (*start_pos) + 2);
		if (create_command_from_args(head, args + (*start_pos) + 2, args_after,
				pipe_type) == false)
			return (false);
		*start_pos += args_after + 1;
		if (create_command_from_args(head, args + *start_pos, *len,
				NONE) == false)
			return (false);
		*start_pos += 2 + args_after + 1;
		return (true);
	}
	else //command format is cmd args [>/>>] file [args]
	{
		args_after = len_till_seperator(args + (*start_pos) + (*len) + 2);
		new_args = ft_calloc(args_after + (*len), sizeof(char *));
		ft_memcpy(new_args, args + (*start_pos), (*len) * sizeof(char *));
		ft_memcpy(new_args + (*len), args + (*start_pos) + (*len)
			+ 2, args_after * sizeof(char *));
		if (!new_args)
			return (false);
		if (create_command_from_args(head, new_args, args_after,
				command_separator_type(args[*start_pos + *len])) == false)
		{
			free(new_args);
			return (false);
		}
		free(new_args);
		if (create_command_from_args(head, args + *start_pos + *len, 2
				, NONE) == false)
			return (false);
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

	pipe_type = command_separator_type(args[(*start_pos) + (*len)]);
	if (pipe_type == NONE || pipe_type == OUTPUT_TO_COMMAND)
	{
		if (create_command_from_args(head, args + *start_pos, *len, pipe_type)
			== false)
			return (false);
		*start_pos += (*len) + 1;
		*len = 0;
		return (true);
	}
	else
		return (output_file_command(head, args, start_pos, len));
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
	int			args_after;
	char		**new_args;

	pipe_type = command_separator_type(args[(*start_pos) + (*len)]);
	if (*len == 0)
	{
		*len += 2;
		if (args[(*start_pos) + *len] == NULL)
			return (err_int_return("parse error", -1));
		if (create_command_from_args(head, args + (*start_pos), 2, pipe_type) == false)
			return (false);
//		command = create_command(pipe_type, args, *len);
//		if (!command)
//			return (false);
		if (command_separator_type(args[(*start_pos) + 2]))
			*start_pos += 3;
		else
			*start_pos += 2;
		*len = 0;
//		command->args_len = 2;
//		return (store_command(command, head));
		return (true);
	}
	else
	{
		if (create_command_from_args(head, args + *start_pos + *len, 2
				, pipe_type) == false)
			return (false);
		args_after = len_till_seperator(args + (*start_pos) + (*len) + 2);
		new_args = ft_calloc(args_after + (*len), sizeof(char *));
		ft_memcpy(new_args, args + (*start_pos), (*len) * sizeof(char *));
		ft_memcpy(new_args + (*len), args + (*start_pos) + (*len)
			+ 2, args_after * sizeof(char *));
		if (!new_args)
			return (false);
		pipe_type = command_separator_type(args[*start_pos + *len + 1]);
		if (create_command_from_args(head, new_args, args_after,
				pipe_type) == false)
		{
			free(new_args);
			return (false);
		}
		free(new_args);
		*start_pos += (*len) + args_after + 2;
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
		if (pipe_type == DELIMITER_INPUT
			|| pipe_type == REDIRECT_INPUT)
			success = input_pipe_command(head, args, &start_pos, &len);
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
t_list	**find_commands(char **args)
{
	t_list	**head;

	if (args == NULL)
		return (NULL);
	head = ft_calloc(1, sizeof(t_list *));
	if (!head)
		return (NULL);
	if (find_commands_in_args(head, args) == false)
	{
		free_commands(head);
		return (NULL);
	}
	return (head);
}
