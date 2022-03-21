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
#include "../headers/arguments.h"

t_list	*get_arg_at_pos(t_list *entry, int i)
{
	while (entry && --i)
		entry = entry->next;
	return (entry);
}

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
	if (!cmd_data->command)
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
 * Create a command from a list of arguments, length, and a pipe type
 *
 * @param	head		List containing all commands
 * @param	args		Arguments to create command from
 * @param	len			Amount of arguments to add to command
 * @param	pipe_type	Pipe type that command pipes to
 *
 * @return	True if command was created successfully
 */
t_cmd_data	*create_new_cmd(t_list **head, char *arg)
{
	t_cmd_data	*cmd_data;

	cmd_data = create_command_data(&arg, 1);
	if (!cmd_data)
		return (NULL);
	return (store_command(cmd_data, head));
}

t_bool	append_arguments_to_command(t_command *cmd, t_list *entry, int len)
{
	char	**new_args;
	int		pos;

	new_args = ft_calloc(cmd->args_len + len + 1, sizeof(char *));
	if (!new_args)
		return (false);
	ft_memcpy(new_args, cmd->args, cmd->args_len * sizeof(char **));
	pos = cmd->args_len;
	free(cmd->args);
	cmd->args = new_args;
	cmd->args_len += len;
	len += pos;
	while (pos != len)
	{
		cmd->args[pos] = ft_strdup(((t_arg *)entry->content)->arg->s);
		if (cmd->args[pos] == NULL) //TODO does args_len need to be updated to ensure proper freeing?
			return (false);
		pos++;
		entry = entry->next;
	}
	return (true);
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

char *str_from_arg(t_list *entry)
{
	t_arg	*arg;

	arg = entry->content;
	return (arg->arg->s);
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
t_bool	pipe_command(t_list **head, t_list **args, int *cmd_len, t_pipe_type pipe_type)
{
	t_cmd_data	*cmd_data;
	t_list		*entry;

	entry = *args;
	cmd_data = create_new_cmd(head, ((t_arg *)entry->content)->arg->s);
	if (cmd_data == NULL)
		return (false);
	if (append_arguments_to_command(cmd_data->command, entry->next, (*cmd_len - 1)) == false)
		return (false);
	//Do the above only once
	entry = get_arg_at_pos(entry, (*cmd_len) + 1);
	*cmd_len = 0;
	while (entry != NULL)
	{
		pipe_type = command_separator_type(str_from_arg(entry));
		if (pipe_type == NONE)
			return (true);
		if (pipe_type == OUTPUT_TO_COMMAND)
		{
			entry = entry->next;
			*args = entry;
			return (true);
		}
		entry = entry->next;
		if (entry == NULL)
			return (false);
		if (pipe_type == DELIMITER_INPUT || pipe_type == REDIRECT_INPUT)
		{
			cmd_data->input.type = pipe_type;
			if (cmd_data->input.file)
				free(cmd_data->input.file);
			cmd_data->input.file = ft_strdup(str_from_arg(entry));
		}
		else if (pipe_type == APPEND_OUTPUT || pipe_type == REDIRECT_OUTPUT)
		{
			cmd_data->output.type = pipe_type;
			if (cmd_data->output.file)
				free(cmd_data->output.file);
			cmd_data->output.file = ft_strdup(str_from_arg(entry));
		}
		entry = entry->next;
	}
	*args = entry;
	return (true);
}

/**
 * Handles command creation for all pipe_type's not handled by pipe_command
 *
 * @param	t_list		All data related to commands
 * @param	args		All arguments
 * @param	start_pos	Start of arguments for this command in args
 * @param	len			Amount of arguments after start that belong to
 * 	this command
 *
 * @return	non zero on error, 0 on success
 */
t_bool	output_pipe_command(t_list **head, t_list **args, int *cmd_len, t_pipe_type pipe_type)
{
	t_cmd_data	*cmd_data;
	t_list		*entry;

	entry = get_arg_at_pos(*args, *cmd_len);
	if (pipe_type == NONE || pipe_type == OUTPUT_TO_COMMAND)
	{
		cmd_data = create_new_cmd(head, ((t_arg *) (*args)->content)->arg->s);
		if (!cmd_data)
			return (false);
		if (*cmd_len > 1)
			if (append_arguments_to_command(cmd_data->command, (*args)->next, *cmd_len - 1) == false)
				return (false);
		cmd_data->output.type = pipe_type;
		set_input(head, cmd_data); // TODO free cmd_data and return false
		*args = entry->next;
		return (true);
	}
	else
		return (pipe_command(head, args, cmd_len, pipe_type));
}

t_pipe_type	loop_arg(t_arg *arg)
{
	t_pipe_type	pipe_type;

	if (arg->literal)
		return (NONE);
	pipe_type = command_separator_type(arg->arg->s);
	if (pipe_type == NONE)
		return (NONE);
	return (pipe_type);
}

t_list	*get_command_start(t_list *cur, int cmd_len)
{
	while (cmd_len--)
		cur = cur->prev;
	return (cur);
}

/**
 * Starts looping through all arguments to find the commands
 *
 * @param	t_list	All data related to commands
 * @param	args	All arguments
 *
 * @return	non zero on error, 0 on success
 */
t_bool	find_commands_in_args(t_list **head, t_list **args)
{
	t_pipe_type	pipe_type;
	t_bool		success;
	int			cmd_len;
	t_list		*cur;

	success = true;
	cmd_len = 0;
	cur = *args;
	while (cur)
	{
		pipe_type = loop_arg(cur->content); //TODO rename
		//ft_printf("%d is pipetype atm in the loop\n", pipe_type);
		if (pipe_type == DELIMITER_INPUT
			|| pipe_type == REDIRECT_INPUT)
			{
				cur = get_command_start(cur, cmd_len);
				//ft_printf("%d is len in find commands in args first if statement \n", len);
				success = pipe_command(head, &cur, &cmd_len, pipe_type);
			}
		else if (pipe_type)
		{
			cur = get_command_start(cur, cmd_len);
			success = output_pipe_command(head, &cur, &cmd_len, pipe_type);
		}
		else
			cmd_len++;
		if (success == false)
			return (success);
		if (!cur)
			break;
		cur = cur->next;
	}
	if (cmd_len != 0)
		success = output_pipe_command(head, args, &cmd_len, pipe_type);
	return (success);
}

/**
 * Finds all the commands and stores them in t_list
 *
 * @param	args	Arguments to create the commands from
 *
 * @return	NULL on error, command data success
 */
t_list	**find_commands(t_list **args) //TODO REMINDER IF COMMAND IS NULL YOU MIGHT NEED TO EXEC PIPE
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
