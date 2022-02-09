/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   new_get_commands.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 19:14:49 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/09 19:14:49 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"

void	swap_last(t_list **top)
{
	t_list	*last;
	t_list	*prev;
	t_list	*prev_prev;

	last = ft_lstlast(*top);
	prev = *top;
	if (prev == NULL || prev->next == NULL)
		return ;
	if (prev->next->next == NULL)
	{
		last->next = prev->next;
		prev->next = NULL;
		return ;
	}
	while (prev->next->next != NULL)
	{
		prev_prev = prev;
		prev = prev->next;
	}
	prev_prev->next = last;
	last->next = prev;
	prev->next = NULL;
}

t_list	*new_command_entry(t_command **pipe_args_p, int *len)
{
	t_list	*new;

	*pipe_args_p = ft_calloc(1, sizeof(t_command));
	if (*pipe_args_p == NULL)
		err_exit("Not enough memory.", 0);
	new = ft_lstnew(*pipe_args_p);
	if (new == NULL)
		err_exit("Not enough memory.", 0);
	*len = 0;
	return (new);
}

void	find(t_command *cmd, char **args, t_command_data *cmd_data, int *len)
{
	char		**new_args;

	cmd->type = command_separator_type(*args);
	if (!cmd->command)
		cmd->command = *args;
	else if (cmd->type != NONE)
	{
		cmd_data->pipes += 1;
		cmd->args_len = *len;
		new_args = ft_calloc((*len) + 2, sizeof(char *));
		if (!new_args)
			err_exit("Not enough memory.", 0);
		*new_args = cmd->command;
		ft_memcpy(new_args + 1, args - *len, (*len) * sizeof(char *));
		cmd->args = new_args;
		*(cmd->args + (*len)) = 0;
	}
	else
		(*len)++;
}

int	run_find_loop(char **args, t_command_data *command_data)
{
	t_command	*command;
	char		**new_args;
	int			len;
	t_list		*new;

	new = new_command_entry(&command, &len);
	command = new->content;
	while (*args)
	{
		find(command, args, command_data, &len);
		if (command->type != NONE)
		{
			if (command->type == REDIRECT_INPUT)
			{
				ft_lstadd_front(command_data->commands, new);
				len = 1;
				new_args = ft_calloc(len + 2, sizeof(char *));
				if (!new_args)
					return (-1);
				*new_args = command->command;
				ft_memcpy(new_args + 1, args - len, len * sizeof(char *));
				command->args = new_args;
				*(command->args + len + 1) = 0;
				args++;
			}
			else
				ft_lstadd_back(command_data->commands, new);
			new = new_command_entry(&command, &len);
			command = new->content;
		}
		args++;
	}
	ft_lstadd_back(command_data->commands, new);
	command->args_len = len;
	new_args = ft_calloc(len + 2, sizeof(char *));
	if (!new_args)
		return (-1);
	*new_args = command->command;
	ft_memcpy(new_args + 1, args - len, len * sizeof(char *));
	command->args = new_args;
	*(command->args + len + 1) = 0;
	return (0);
}

t_command_data	*find_commands(char **args)
{
	t_command_data	*command_data;
	t_list			**top;
	t_command		*command;

	if (args == NULL)
		return (NULL);
	command_data = ft_calloc(1, sizeof(t_command_data));
	top = ft_calloc(1, sizeof(t_list *));
	if (!command_data || !top)
		return (NULL);
	command_data->commands = top;
	if (run_find_loop(args, command_data))
		return (NULL);
	command = ft_lstlast(*top)->content;
	if (!command->command)
		return (NULL);
	return (command_data);
}
