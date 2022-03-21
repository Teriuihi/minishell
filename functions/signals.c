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

/**
 * checks whether input is NULL (VEOF) or sigint flag is on
 *
 * @param	input string, NULL if VEOF has been reached
 *
 * @return	void
 */
void	signal_check(char *input)
{
	if (input == 0 || g_signal.sigint == 1)
	{
		if (g_signal.sigint != 1)
		{
			g_signal.veof = 1;
		}
	}
}

void	check_status(void)
{
	if (g_signal.veof == 1)
	{
		ft_printf("\b\bexit\n");
	}
	if (g_signal.sigint == 1)
	{
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
}

void	init_signal(void)
{
	g_signal.sigint = 0;
	g_signal.veof = 0;
	g_signal.finished = false;
	g_signal.pid = getpid();
	g_signal.keep_running = 1;
}
