/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   create_new_command.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 19:21:58 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/06 19:21:58 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include <fcntl.h>

/**
 * Stores a command, on error command is freed
 * 	all other command data isn't freed
 *
 * @param	cmd_data	Command to store
 * @param	head		List to store the command in
 *
 * @return	The command data that was stored or NULL on failure
 */
static t_cmd_data	*store_command(t_cmd_data *cmd_data, t_list **head)
{
	t_list	*new;
	char	*entry;

	new = ft_lstnew(cmd_data);
	if (new == NULL)
	{
		free(cmd_data->command->command);
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
	ft_lstadd_back(head, new);
	return (cmd_data);
}

/**
 * Stores the first len arguments from char args in the command
 *
 * @param	args	Args to store
 * @param	len		Amount of arguments to store
 * @param	command	The command to store them in
 *
 * @return	True on success false on failure
 */
static t_bool	store_args(char **args, int len, t_command *command)
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
 * Creates space for the command arguments and adds them to the command
 * 	Free's on failure
 *
 * @param	command	Command to add arguments to
 * @param	len		Length of arguments array
 * @param	args	Arguments for this command
 *
 * @return	Boolean indicating success
 */
static t_bool	create_command_args(t_command *command, int len, char **args)
{
	command->args = ft_calloc(len + 1, sizeof(char *));
	if (!command->args)
	{
		free(command->command);
		free(command);
		return (err_int_return("Not enough memory.", false));
	}
	if (store_args(args, len, command) == false)
		return (false);
	command->args_len = len;
	return (true);
}

/**
 * Creates a new command from the given parameters, by duplicating them
 * 	frees created data on failure
 *
 * @param	args	All arguments
 * @param	len		Amount of arguments after start that belong to
 * 	this command
 *
 * @return	The command data for this command or NULL on failure
 */
static t_cmd_data	*create_command_data(char **args, int len)
{
	t_cmd_data	*cmd_data;
	t_command	*command;

	cmd_data = ft_calloc(1, sizeof(t_cmd_data));
	if (!cmd_data)
		return (err_ptr_return("Not enough memory.", NULL));
	cmd_data->command = ft_calloc(1, sizeof(t_command));
	if (!cmd_data->command)
		return (err_ptr_return("Not enough memory.", NULL));
	if (args == NULL)
		cmd_data->command->command = NULL;
	else
		cmd_data->command->command = ft_strdup(*args);
	command = (cmd_data->command);
	if (!command->command && args != NULL)
	{
		free(command);
		return (err_ptr_return("Not enough memory.", NULL));
	}
	if (create_command_args(command, len, args) == false)
		return (NULL);
	return (cmd_data);
}

/**
 * Create a command from a list of arguments, length, and a pipe type
 *
 * @param	head		List containing all commands
 * @param	args		Arguments to create command from
 *
 * @return	The command data for the new command or NULL on failure
 */
t_cmd_data	*create_new_cmd(t_list **head, char *arg)
{
	t_cmd_data	*cmd_data;

	if (arg == NULL)
		cmd_data = create_command_data(NULL, 0);
	else
		cmd_data = create_command_data(&arg, 1);
	if (!cmd_data)
		return (NULL);
	return (store_command(cmd_data, head));
}
