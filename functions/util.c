/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   functions.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 14:13:16 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/04 14:13:16 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * Checks if the str is | > or >> should check for < and << later as well
 * Should return a value based on the type of pipe that's needed maybe?
 *
 * @param	str	string to check
 *
 * @return	0 if it's a command separator
 */

#include <unistd.h>
#include "../headers/structs.h"
#include "../headers/minishell.h"
#include "../headers/functions.h"
#include <stdio.h>
#include <errno.h>
#include "../builtins/builtins.h"

t_pipe_type	command_separator_type(char *str)
{
	if (str == NULL)
		return (NONE);
	if (ft_streq(str, "|"))
		return (OUTPUT_TO_COMMAND);
	if (ft_streq(str, ">"))
		return (REDIRECT_OUTPUT);
	if (ft_streq(str, "<"))
		return (REDIRECT_INPUT);
	if (ft_streq(str, "<<"))
		return (DELIMITER_INPUT);
	if (ft_streq(str, ">>"))
		return (APPEND_OUTPUT);
	return (NONE);
}

int	interruptible_getc(void)
{
	int		r;
	char	c;

	r = read(0, &c, 1);
	if (r == -1 && errno == EINTR && g_signal.sigint != 1)
	{
		return (0);
	}
	if (c == 4)
	{
		g_signal.veof = 1;
	}
	if (r == 1)
	{
		return (c);
	}
	else
	{
		return (EOF);
	}
}

t_bool	enter_curdir(t_minishell *minishell)
{
	char	*cur_dir;

	cur_dir = get_pwd(minishell);
	if (!cur_dir)
	{
		return (new_set_exit_status(1, NULL));
	}
	if (chdir(cur_dir) == -1)
	{
		if (chdir(minishell->home) == -1)
			return (new_set_exit_status(1, NULL));
	}
	return (true);
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
 * Check if a command should be executed in a child process
 *
 * @param	command	Command to check
 *
 * @return	True if it should be executed in a child process, false if not
 */
t_bool	should_be_child(t_command *command)
{
	if (env_variable_found(command) == true)
		return (false);
	if (ft_streq(command->command, "cd"))
		return (false);
	if (ft_streq(command->command, "unset"))
		return (false);
	return (true);
}
