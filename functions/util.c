/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string.c                                             :+:    :+:            */
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


void	free_command(void *content)
{
	t_cmd_data	*cmd_data;
	t_command	*command;
	int			i;

	if (!content)
		return ;
	cmd_data = (t_cmd_data *)content;
	command = cmd_data->command;
	i = 0;
	while (command->args[i])
	{
		free(command->args[i]);
		i++;
	}
	free(cmd_data->input.file);
	free(cmd_data->output.file);
	free(cmd_data);
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

t_bool	set_exit_status(t_minishell *minishell, int status, char *message)
{
	g_signal.exit_status = status;
	//if there is a message, sthat means we just have to
	if (message != NULL)
	{
		g_signal.print_basic_error = false;
		//if status != 0 then send it to err
		ft_printf("%s\n", message); //if error then to stderr
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



int interruptible_getc(void)
{	
	int		r;
	char	c;

	if (g_signal.interrupted == true)
	{
		return EOF;
	}
	r = read(0, &c, 1);
	//crtl \ -> errno = 4, -1 r
	//crtl D -> errno = 2, 1 r
	if (r == -1 && errno == EINTR) //then this is sigint (crtl c here)
	{
		//ft_printf("%d is errno KKKK\n", errno);
		if (errno == 4 && g_signal.sigint != 1 && r == -1) //then this should be 
		{
			//ft_printf("setting sigquit\n", g_signal.sigquit);
			g_signal.sigquit = 1;
			//g_signal.interrupted = true;
		}
		//ft_printf("%d is errno KKKK\n", errno);
		//if (errno == 4 && g_signal.sigint != 1)
		//{
		//	g_signal.sigquit == 1;
		//	ft_printf("%d is sigquit\n", g_signal.sigquit);
		//}
		else if (errno == EINTR && g_signal.sigint != 1) //we have to check which signal was which it interrupted
		{
			//ft_printf("we are interrupted, %d is errno, %d is r, %d sigquit\n", errno, r, g_signal.sigquit);

			//ft_printf("sigquit is right place\n");
			//write(1, "right place for sigquit\n", 25);
			//ft_printf("we are interrupted, %d is errno, %d is r, %d sigquit\n", errno, r, g_signal.sigquit);
			//ft_printf("GONNA SET INTERRUPTED\n");
			g_signal.interrupted = true;
			//return (1);
		}
	}
	//r == 1 ? printf("%c to return\n", c) : printf("EOF to return\n");
	return (r == 1 ? c : EOF); //if we use signals this we always return EOF
}
