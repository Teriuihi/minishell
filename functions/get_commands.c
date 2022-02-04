/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_commands.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 20:02:53 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/02 20:02:53 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 18:29:13 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/02 18:29:13 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"

void	free_func(void *obj)
{
	free(obj);
}

int	next_command(t_list **top, t_command **pipe_args_p, int *len)
{
	t_list	*new;

	*pipe_args_p = ft_calloc(1, sizeof(t_command));
	if (*pipe_args_p == NULL)
		return (-1);
	new = ft_lstnew(*pipe_args_p);
	if (new == NULL)
	{
		free(pipe_args_p);
		return (-1);
	}
	ft_lstadd_back(top, new);
	*len = 0;
	return (0);
}

int	find(t_command **cmd, char **args, t_list **top, int *len)
{
	char		**new_args;
	t_command	*command;

	command = *cmd;
	command->type = command_separator_type(*args);
	if (!command->command)
		command->command = *args;
	else if (command->type != NONE)
	{
		command->args_len = *len;
		new_args = ft_calloc((*len) + 1, sizeof(char *));
		if (!new_args)
			return (-1);
		ft_memcpy(new_args, args - *len, (*len) * sizeof(char *));
		command->args = new_args;
		if (next_command(top, cmd, len))
			return (-1);
	}
	else
		(*len)++;
	return (0);
}

int	run_find_loop(char **args, t_list **top)
{
	t_command	*command;
	char		**new_args;
	int			len;

	if (next_command(top, &command, &len))
		return (-1);
	while (*args)
	{
		if (find(&command, args, top, &len))
		{
			ft_lstclear(top, free_func);
			free(top);
			return (-1);
		}
		args++;
	}
	command->args_len = len;
	new_args = ft_calloc(len + 1, sizeof(char *));
	if (!new_args)
		return (-1);
	ft_memcpy(new_args, args - len, len * sizeof(char *));
	command->args = new_args;
	return (0);
}

t_list	**find_commands(char **args)
{
	t_list		**top;
	t_command	*command;

	if (args == NULL)
		return (NULL);
	top = ft_calloc(1, sizeof(t_list *));
	if (top == NULL)
		return (NULL);
	if (run_find_loop(args, top))
		return (NULL);
	command = ft_lstlast(*top)->content;
	if (command->command == NULL)
	{
		ft_lstclear(top, free_func);
		free(top);
		return (NULL);
	}
	return (top);
}
