/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                                :+:    :+:      */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"
#include "../headers/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <errno.h>

void	signal_check(char *input, t_minishell *minishell)
{
	if (g_signal.sigquit == 1)
	{
//		*display_prompt = false;
		g_signal.sigquit = 0;
		return ;
	}
	else if (input == 0 || g_signal.sigint == 1)
	{
		if (g_signal.heredoc)
		{
			g_signal.heredoc = false;
			ft_printf(1, "\b\b");
			return ;
		}
		if (g_signal.sigint != 1)
		{
			g_signal.veof = 1;
		}
	}
//	if (display_prompt != NULL)
//		*display_prompt = true;
}

void	check_status(t_minishell *minishell)
{
	if (g_signal.veof == 1)
	{
		ft_printf(1, "\b\bexit\n");
		g_signal.shell_level -= 1;
	}
	if (g_signal.sigint == 1 && g_signal.command == false)
	{
		g_signal.exit_status = 128 + 2;
		g_signal.sigint = 0;
		ft_printf(1, "\n");
	}
}

void	sigquit_handler(int this_signal)
{
	if (this_signal == SIGINT)
	{
		g_signal.sigint = 1;
		g_signal.exit_status = 128 + 2;
		//ft_printf("Exit status changed to %d\n", g_signal.exit_status);
	}
	if (this_signal == SIGQUIT && g_signal.command == false)
	{
		g_signal.sigquit = 1;
		g_signal.exit_status = 128 + 3;
		//ft_printf("Exit status changed to %d\n", g_signal.exit_status);
	}
}

void	init_signal(void)
{
	g_signal.shell_level = 2;
	g_signal.exit_status = 0;
	g_signal.minishell_exec_found = 0;
	g_signal.sigint = 0;
	g_signal.veof = 0;
	g_signal.print_basic_error = true;
	g_signal.pid = getpid();
	g_signal.heredoc = false;
	g_signal.command = false;
}