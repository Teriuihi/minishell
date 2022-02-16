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
int	store_command(t_command *command, t_list **head)
{
	t_list	*new;
	char	*entry;

	new = ft_lstnew(command);
	if (new == NULL)
	{
		free_commands(head);
		free(command->command);
		entry = *command->args;
		while (*entry)
		{
			free(entry);
			entry++;
		}
		free(command);
		err_int_return("Not enough memory.", -1);
	}
	ft_lstadd_back(head, new);
	return (0);
}

t_bool	store_args(char **args, int len, int start_pos, t_command *command)
{
	int		i;
	char	*entry;

	i = -1;
	while (++i < len)
	{
		entry = args[start_pos + i];
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
t_command	*create_command(t_pipe_type pipe_type, char **args, int start_pos,
							int len)
{
	t_command	*command;

	command = ft_calloc(1, sizeof(t_command));
	if (!command)
		return (err_ptr_return("Not enough memory.", NULL));
	command->command = ft_strdup(args[start_pos]);
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
	store_args(args, len, start_pos, command);
	command->type = pipe_type;
	command->args_len = len;
	return (command);
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
int	output_pipe_command(t_list **head, char **args, int *start_pos, int *len)
{
	t_command	*command;
	t_pipe_type	pipe_type;

	pipe_type = command_separator_type(args[(*start_pos) + (*len)]);
	command = create_command(pipe_type, args, *start_pos, *len);
	if (!command)
		return (-1);
	store_command(command, head);
	*start_pos += (*len) + 1;
	*len = 0;
	return (0);
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
int	input_pipe_command(t_list **head, char **args, int *start_pos, int *len)
{
	t_command	*command;
	t_pipe_type	pipe_type;

	pipe_type = command_separator_type(args[(*start_pos) + (*len)]);
	if (*len == 0)
	{
		*len += 2;
		if (args[(*start_pos) + *len] == NULL)
			return (err_int_return("parse error", -1));
		command = create_command(pipe_type, args, *start_pos, *len);
		if (!command)
			return (-1);
		if (command_separator_type(args[(*start_pos) + (*len)]))
			*start_pos += (*len) + 1;
		else
			*start_pos += (*len);
		*len = 0;
		command->args_len = 2;
		store_command(command, head);
		return (0);
	}
	return (0); //TODO implement being able to find << and < anywhere in the command
}

/**
 * Starts looping through all arguments to find the commands
 *
 * @param	t_list	All data related to commands
 * @param	args	All arguments
 *
 * @return	non zero on error, 0 on success
 */
int	find_commands_in_args(t_list **head, char **args)
{
	t_pipe_type	pipe_type;
	int			len;
	int			err;
	int			start_pos;

	err = 0;
	start_pos = 0;
	len = 0;
	while (args[start_pos + len])
	{
		pipe_type = command_separator_type(args[start_pos + len]);
		if (pipe_type == DELIMITER_INPUT
			|| pipe_type == REDIRECT_INPUT)
			err = input_pipe_command(head, args, &start_pos, &len);
		else if (pipe_type)
			err = output_pipe_command(head, args, &start_pos, &len);
		else
			len++;
		if (err)
			return (err);
	}
	if (len != 0)
		err = output_pipe_command(head, args, &start_pos, &len);
	return (err);
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
	if (find_commands_in_args(head, args))
		return (NULL);
	return (head);
}
