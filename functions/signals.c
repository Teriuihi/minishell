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

/**
 * checks whether input is NULL (VEOF) or sigint flag is on
 *
 * @param	input string, NULL if VEOF has been reached
 *
 * @return	void
 */
/*
void	signal_check(char *input)
{
	if (input == 0)
	{
		if (g_signal.sigint == 0 && g_signal.veof == 0 && g_signal.sigquit == 0)
		{
			if (errno == 2)
			{
				g_signal.sigquit = 0;
				g_signal.veof = 1;
				ft_printf("%d is sigveof, %d is errno\n", g_signal.veof, errno);
			}
			else if (errno == 4)
			{
				g_signal.veof = 0;
				g_signal.sigquit = 1;
				g_signal.interrupted = false;
				//ft_printf("\b\b\b\b\b\b\b\b\b\b\b");
				ft_printf("%d is sigquit, %d is errno\n", g_signal.sigquit, errno);
			}
			//ft_printf("%d is sigquit, %d sigint, %d sigveof, %d is errno\n", g_signal.sigquit, g_signal.sigint, g_signal.veof, errno);
		}
	}
}


void	check_status(t_minishell *minishell)
{
	if (g_signal.veof == 1)
	{
		g_signal.shell_level -= 1;
		ft_printf("\b\bexit\n");
	}
	if (g_signal.sigint == 1)
	{
		g_signal.sigint = 0;
		minishell->exit_status = 128 + 2;
		ft_printf("\n");
	}
	if (g_signal.sigquit == 1)
	{
		g_signal.sigquit = 0;
		minishell->exit_status = 128 + 3;
		ft_printf("\n");
	}
}

void	sigquit_handler(int this_signal)
{
	if (this_signal == SIGINT)
	{
		ft_printf("SIGINT RECEIVED\n");
		g_signal.sigint = 1;
		//g_signal.exit_status = 128 + 2;
	}
	if (this_signal == SIGQUIT)
	{
		ft_printf("SIGQUIT RECEIVED\n");
		g_signal.sigquit = 1;
		g_signal.exit_status = 128 + 3;
		//g_signal.exit_status = 128 + 2;
	}
	//REGISTER ONE FOR -/ handler when initing termios
}

void	init_signal(void)
{
	g_signal.shell_level = 2;
	g_signal.minishell_exec_found = 0;
	g_signal.sigint = 0;
	g_signal.veof = 0;
	g_signal.exit_status = 0;
	g_signal.finished = false;
	g_signal.pid = getpid();
}
*/

void	signal_check(char *input, t_bool *display_prompt, t_minishell *minishell)
{
	//if its sigquit, set exit code and redisplay the prompt
	if (g_signal.sigquit == 1)
	{
		if (display_prompt != NULL)
			*display_prompt = false;
		if (g_signal.pid > 0)
		{
			g_signal.exit_status = 128 + 3;
		}
		//if (input)
		//{
		//	free(input);
			//input = NULL;
		//}
		//ft_printf("GONNA RETURN FROM SIGQUIT 1\n");
		g_signal.sigquit = 0;
		return ;
	}
	if (input == 0 || g_signal.sigint == 1)
	{
		if (g_signal.sigint != 1)
		{
			g_signal.veof = 1;
		}
	}
	if (display_prompt != NULL)
		*display_prompt = true;
}

void	check_status(t_minishell *minishell)
{
	if (g_signal.veof == 1)
	{
		ft_printf("\b\bexit\n");
		g_signal.shell_level -= 1;
	}
	if (g_signal.sigint == 1)
	{
		//ft_printf("sigint registered\n");
		//rl_redisplay();
		//rl_on_new_line();
		//clear_history();
		//rl_replace_line();
		
		//rl_forced_update_display();
		//
		g_signal.exit_status = 128 + 2;
		g_signal.sigint = 0;
		ft_printf("\n");
	}
}

void	sigquit_handler(int this_signal)
{
	if (this_signal == SIGINT)
	{
		g_signal.sigint = 1;
	}
	if (this_signal == SIGQUIT)
	{
		g_signal.sigquit = 1;
		//g_signal.bullshit++;
		g_signal.exit_status = 128 + 3;
	}
}

void	init_signal(void)
{
	g_signal.shell_level = 2;
	g_signal.exit_status = 0;
	g_signal.minishell_exec_found = 0;
	g_signal.sigint = 0;
	g_signal.veof = 0;
	g_signal.bullshit = 0;
	g_signal.finished = false;
	g_signal.pid = getpid();
}