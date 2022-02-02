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

/**
 * Checks if the str is | > or >> should check for < and << later as well
 * Should return a value based on the type of pipe that's needed maybe?
 *
 * @param	str	string to check
 *
 * @return	0 if it's a command separator
 */
t_pipe_type	command_separator_type(char *str)
{
	if (ft_strlen(str) == 1)
	{
		if (*str == '|')
			return (OUTPUT_TO_COMMAND);
		if (*str == '>')
			return (REDIRECT_OUTPUT);
		if (*str == '<')
			return (REDIRECT_INPUT);
	}
	else if (ft_strlen(str) == 2)
	{
		if (!ft_strncmp(str, "<<", 2))
			return (DELIMITER_INPUT);
		if (!ft_strncmp(str, ">>", 2))
			return (APPEND_OUTPUT);
	}
	return (INVALID);
}

void	free_func(void *obj)
{
	free(obj);
}

int	next_command(t_list **top, t_command **pipe_args_p, int *len)
{
	t_list	*new;

	*pipe_args_p = ft_calloc(1, sizeof(t_command));
	if (*pipe_args_p == NULL)
	{
		ft_lstclear(top, free_func);
		free(top);
		return (-1);
	}
	new = ft_lstnew(*pipe_args_p);
	if (new == NULL)
	{
		ft_lstclear(top, free_func);
		free(top);
		return (-1);
	}
	ft_lstadd_back(top, new);
	*len = 0;
	return (0);
}

int	run_find_loop(char **args, t_list **top)
{
	t_command	*command;
	int			len;

	if (next_command(top, &command, &len))
		return (-1);
	while (*args)
	{
		command->type = command_separator_type(*args);
		if (command->command == NULL)
			command->command = *args;
		else if (command->type != INVALID)
		{
			command->args_len = len;
			command->args = args - len;
			if (next_command(top, &command, &len))
				return (-1);
		}
		else
			len++;
		args++;
	}
	command->args_len = len;
	command->args = args - len;
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
