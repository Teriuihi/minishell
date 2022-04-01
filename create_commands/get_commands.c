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
#include <fcntl.h>

/**
 * Get the entry i positions ahead of the current one
 *
 * @param	entry	t_list entry to start from
 * @param	i		Amount of positions to move forward
 *
 * @return	The t_list entry at the given position from the given start
 */
t_list	*get_arg_at_pos(t_list *entry, int i)
{
	if (i < 0)
	{
		while (entry && i++)
			entry = entry->prev;
	}
	else
	{
		while (entry && i--)
			entry = entry->next;
	}
	return (entry);
}

/**
 * Safely get pipe type from argument
 *
 * @param	arg	arg to get pipe type from
 *
 * @return	pipe_type of arg or NONE if arg is literal
 */
t_pipe_type	pipe_type_from_arg(t_arg *arg)
{
	t_pipe_type	pipe_type;

	if (arg->literal)
		return (NONE);
	pipe_type = command_separator_type(arg->arg->s);
	if (pipe_type == NONE)
		return (NONE);
	return (pipe_type);
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
	command->args = ft_calloc(len + 1, sizeof(char *));
	if (!command->args)
	{
		free(command->command);
		free(command);
		return (err_ptr_return("Not enough memory.", NULL));
	}
	if (store_args(args, len, command) == false)
		return (NULL);
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

	if (arg == NULL)
		cmd_data = create_command_data(NULL, 0);
	else
		cmd_data = create_command_data(&arg, 1);
	if (!cmd_data)
		return (NULL);
	return (store_command(cmd_data, head));
}

/**
 * Append len given arguments to given command
 *
 * @param	cmd		Command to append arguments to
 * @param	entry	Entry to start adding from
 * @param	len		Amount of entries to add
 *
 * @return	true on success false on failure
 */
t_bool	append_arguments_to_command(t_command *cmd, t_list *entry, int len, t_bool prefix)
{
	char	**new_args;
	int		pos;

	new_args = ft_calloc(cmd->args_len + len + 1, sizeof(char *));
	if (!new_args)
		return (false);
	if (prefix)
		ft_memcpy(new_args + len, cmd->args, cmd->args_len * sizeof(char **));
	else
		ft_memcpy(new_args, cmd->args, cmd->args_len * sizeof(char **));
	pos = cmd->args_len;
	free(cmd->args);
	cmd->args = new_args;
	cmd->args_len += len;
	if (prefix)
		pos = 0;
	else
		len += pos;
	while (pos != len)
	{
		cmd->args[pos] = ft_strdup(((t_arg *)entry->content)->arg->s);
		if (cmd->args[pos] == NULL)
			return (false);
		pos++;
		entry = entry->next;
	}
	return (true);
}

/**
 * Get the string from a t_list entry which contains a t_arg
 *
 * @param	entry	Entry to get string from
 *
 * @return	String from entry
 */
char	*str_from_arg(t_list *entry)
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
t_bool	pipe_command(t_list **head, t_list **args, int *cmd_len, t_pipe_type pipe_type, t_minishell *minishell)
{
	t_cmd_data	*cmd_data;
	t_list		*entry;
	int			fd;
	char		*message;
	t_bool		success;

	success = true;
	entry = *args;
	if (*cmd_len == 0)
	{
		cmd_data = create_new_cmd(head, NULL);
		if (cmd_data == NULL)
			return (false);
		while (entry != NULL)
		{
			pipe_type = pipe_type_from_arg(entry->content);
			if (pipe_type == NONE)
				break ;
			if (pipe_type == OUTPUT_TO_COMMAND)
			{
				*args = entry;
				return (success);
			}
			entry = entry->next;
			if (entry == NULL)
				return (false);
			if ((pipe_type == DELIMITER_INPUT || pipe_type == REDIRECT_INPUT) && success == true)
			{
				cmd_data->input.type = pipe_type;
				if (cmd_data->input.file)
					free(cmd_data->input.file);
				cmd_data->input.file = ft_strdup(str_from_arg(entry));
				if (cmd_data->input.file == NULL)
				{
					//TODO error return
					success = false;
				}
				if (pipe_type == REDIRECT_INPUT)
				{
					fd = open(cmd_data->output.file, O_RDONLY, 0);
					if (fd < 0)
					{
						message = ft_strjoin("some shell: ", ft_strjoin(cmd_data->output.file, ": No such file or directory\n"));
						if (!message)
							message = "some shell: out of memory";
						set_exit_status(minishell, 1, message);
						success = false;
					}
					close(fd);
				}
			}
			else if ((pipe_type == APPEND_OUTPUT || pipe_type == REDIRECT_OUTPUT) && success == true)
			{
				cmd_data->output.type = pipe_type;
				if (cmd_data->output.file)
					free(cmd_data->output.file);
				cmd_data->output.file = ft_strdup(str_from_arg(entry));
				if (cmd_data->output.file == NULL)
				{
					success = false;
					//TODO error return
				}
				close(open(cmd_data->output.file, O_CREAT, 0777));
			}
			entry = entry->next;
		}
		if (entry == NULL)
		{
			*args = entry;
			return (success);
		}
		cmd_data->command->command = ft_strdup(((t_arg *)entry->content)->arg->s);
		if (cmd_data->command->command == NULL)
			return (false); //TODO free?
		if (append_arguments_to_command(cmd_data->command, entry, 1, true) == false)
			return (false);
		entry = entry->next;
		while (!pipe_type_from_arg(entry->content))
		{
			cmd_len++;
			entry = entry->next;
		}
		if (*cmd_len && append_arguments_to_command(cmd_data->command, entry->next, (*cmd_len - 1), false) == false)
			return (false);
		*cmd_len = 0;
	}
	else {
		cmd_data = create_new_cmd(head, ((t_arg *)entry->content)->arg->s);
		if (cmd_data == NULL)
			return (false);
		if (append_arguments_to_command(cmd_data->command, entry->next, (*cmd_len - 1), false) == false)
			return (false);
		entry = get_arg_at_pos(entry, *cmd_len);
		*cmd_len = 0;
	}
	while (entry != NULL)
	{
		pipe_type = pipe_type_from_arg(entry->content);
		if (pipe_type == NONE)
		{
			if (append_arguments_to_command(cmd_data->command, entry, 1, false) == false)
				return (false);
			entry = entry->next;
			continue ;
		}
		if (pipe_type == OUTPUT_TO_COMMAND)
		{
			*args = entry;
			return (success);
		}
		entry = entry->next;
		if (entry == NULL)
			return (false);
		if ((pipe_type == DELIMITER_INPUT || pipe_type == REDIRECT_INPUT) && success == true)
		{
			cmd_data->input.type = pipe_type;
			if (cmd_data->input.file)
				free(cmd_data->input.file);
			cmd_data->input.file = ft_strdup(str_from_arg(entry));
			if (cmd_data->input.file == NULL)
			{
				//TODO error return
				success = false;
			}
			if (pipe_type == REDIRECT_INPUT)
			{
				fd = open(cmd_data->output.file, O_RDONLY, 0);
				if (fd < 0)
				{
					message = ft_strjoin("some shell: ", ft_strjoin(cmd_data->output.file, ": No such file or directory\n"));
					if (!message)
						message = "some shell: out of memory";
					set_exit_status(minishell, 1, message);
					success = false;
				}
				close(fd);
			}
		}
		else if ((pipe_type == APPEND_OUTPUT || pipe_type == REDIRECT_OUTPUT) && success == true)
		{
			cmd_data->output.type = pipe_type;
			if (cmd_data->output.file)
				free(cmd_data->output.file);
			cmd_data->output.file = ft_strdup(str_from_arg(entry));
			if (cmd_data->output.file == NULL)
			{
				success = false;
				//TODO error return
			}
			close(open(cmd_data->output.file, O_CREAT, 0777));
		}
		entry = entry->next;
	}
	if (entry != NULL)
		*args = entry->prev;
	else
		*args = entry;
	return (success);
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
t_bool	output_pipe_command(t_list **head, t_list **args, int *cmd_len,
						t_pipe_type pipe_type)
{
	t_cmd_data	*cmd_data;
	t_list		*entry;

	entry = *args;
	if (*cmd_len == 0)
		cmd_data = create_new_cmd(head, NULL);
	else
		cmd_data = create_new_cmd(head, ((t_arg *)entry->content)->arg->s);
	if (!cmd_data)
		return (false);
	if (*cmd_len > 1)
		if (append_arguments_to_command(cmd_data->command, entry->next,
				*cmd_len - 1, false) == false)
			return (false);
	cmd_data->output.type = pipe_type;
	entry = get_arg_at_pos(*args, *cmd_len);
	*cmd_len = 0;
	*args = entry;
	return (true);
}

t_list	*get_command_start(t_list *cur, int cmd_len)
{
	while (cmd_len--)
		cur = cur->prev;
	return (cur);
}

void	update_last_command_input(t_list **head)
{
	t_list		*entry;
	t_cmd_data	*cmd_data;

	if (head == NULL || *head == NULL)
		return ;
	entry = ft_lstlast(*head);
	cmd_data = entry->content;
	if (cmd_data->input.type != NONE || entry->prev == NULL)
		return ;
	cmd_data->input.type = OUTPUT_TO_COMMAND;
}

/**
 * Starts looping through all arguments to find the commands
 *
 * @param	t_list	All data related to commands
 * @param	args	All arguments
 *
 * @return	non zero on error, 0 on success
 */
t_bool	find_commands_in_args(t_list **head, t_list **args, t_minishell *minishell)
{
	t_pipe_type	pipe_type;
	t_bool		success;
	int			cmd_len;
	t_list		*cur;

	success = true;
	cmd_len = 0;
	cur = *args;
	{
		t_list	*tmp = *args;
		while (tmp != NULL)
		{
			t_arg	*arg = tmp->content;
			ft_printf(2, "%s - %i\n", arg->arg->s, arg->literal);
			tmp = tmp->next;
		}
	}
	while (true)
	{
		pipe_type = pipe_type_from_arg(cur->content);
		if (pipe_type == DELIMITER_INPUT
			|| pipe_type == REDIRECT_INPUT
			|| pipe_type == REDIRECT_OUTPUT
			|| pipe_type == APPEND_OUTPUT)
			{
				cur = get_command_start(cur, cmd_len);
 				success = pipe_command(head, &cur, &cmd_len, pipe_type, minishell);
			}
		else if (pipe_type)
		{
			cur = get_command_start(cur, cmd_len);
			success = output_pipe_command(head, &cur, &cmd_len, pipe_type);
		}
		else
			cmd_len++;
		update_last_command_input(head);
		if (success == false)
		{
			ft_lstremove_last(head);
			success = true;
		}
		if (cur == NULL || cur->next == NULL)
			break ; //TODO in > < >> << command check if there is a next if we end on |
		cur = cur->next;
	}
	if (cmd_len != 0 && cur != NULL)
	{
		cur = get_command_start(cur, cmd_len - 1);
		success = output_pipe_command(head, &cur, &cmd_len, pipe_type);
		update_last_command_input(head);
	}
	return (success);
}

/**
 * Finds all the commands and stores them in t_list
 *
 * @param	args	Arguments to create the commands from
 *
 * @return	NULL on error, command data success
 */
t_list	**find_commands(t_list **args, t_minishell *minishell) //TODO if a command is NULL only do input/output then continue
{
	t_list	**head;

	if (args == NULL)
		return (NULL);
	head = ft_calloc(1, sizeof(t_list *));
	if (!head)
		return (NULL);
	if (find_commands_in_args(head, args, minishell) == false)
	{
		free_commands(head);
		return (NULL);
	}
	return (head);
}
