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
#include <stdarg.h>

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

void	err_exit(char *err, int status)
{
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(status);
}

int	err_int_return(char *err, int status)
{
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (status);
}

void	*err_ptr_return(char *err, void *ptr)
{
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (ptr);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	i;

	dst_len = ft_strlen(dst) >= dstsize ? dstsize : ft_strlen(dst);
	i = 0;
	if (dstsize == dst_len || dstsize == 0)
		return (dstsize + ft_strlen(src));
	if (ft_strlen(src) >= dstsize - dst_len)
	{
		while (i < dstsize - dst_len - 1)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	else
	{
		while (src[i] && i < dstsize - dst_len)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	dst[dst_len + i] = '\0';
	return (dst_len + ft_strlen(src));
}

void	free_splitted(char **splitted)
{
	int	i;

	i = 0;
	if (!splitted)
	{
		return ;
	}
	while (splitted[i])
	{
		free(splitted[i]);
		i++;
	}
	free(splitted);
}

/**
 * Free's a command
 *
 * @param	cmd_data	The command to free
 */
void	free_cmd(t_cmd_data *cmd_data)
{
	if (cmd_data->command != NULL)
	{
		free(cmd_data->command->command);
		while (cmd_data->command->args_len)
		{
			free(cmd_data->command->args[cmd_data->command->args_len - 1]);
			cmd_data->command->args_len--;
		}
		free(cmd_data->command->args);
		free(cmd_data->command);
	}
	free(cmd_data->input.file);
	free(cmd_data->output.file);
	free(cmd_data);
}

void	free_command(void *content)
{
	t_cmd_data	*cmd_data;

	if (!content)
		return ;
	cmd_data = (t_cmd_data *)content;
	free_cmd(cmd_data);
}

void	free_commands(t_list **head)
{
	ft_lstclear(head, free_command);
}

void	free_char_arr(char **args)
{
	char	**tmp;

	tmp = args;
	while (args && *args)
	{
		free(*args);
		args++;
	}
	free(tmp);
}

t_bool	new_set_exit_status(int status, const char *str, ...)
{
	va_list	ap;

	g_signal.exit_status = status;
	if (str != NULL)
	{
		g_signal.print_basic_error = false;
		va_start(ap, str);
		if (status != 0)
			ft_printf_va(2, str, ap);
		else
			ft_printf_va(1, str, ap);
	}
	if (status == 0)
		return (true);
	else
		return (false);
}

t_bool	set_exit_status(t_minishell *minishell, int status, char *message,
			t_bool should_free)
{
	g_signal.exit_status = status;
	if (message != NULL)
	{
		g_signal.print_basic_error = false;
		//if status != 0 then send it to err
		if (status != 0)
			ft_printf(2, "%s\n", message); //if error then to stderr
		else
			ft_printf(1, "%s\n", message);
		if (should_free == true)
			free(message);
	}
	if (status == 0)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

int interruptible_getc(void) //crtl \ -> errno = 4, -1 r , //crtl D -> errno = 2, 1 r
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
	return (r == 1 ? c : EOF);
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