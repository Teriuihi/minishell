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
 * Handling different signals based on signum
 *
 *
 * @param	args signum
 *
 * @return	void
 */


//https://www.cons.org/cracauer/sigint.html

//https://stackoverflow.com/questions/1516122/how-to-capture-controld-signal
void	crtld_handler(int signum)
{
	(void)signum;
}



/*
void	sigquit_handler(int signum) //crtl + \ 
{
	(void)signum; //write(1, "entered to crtl + backslash signal\n", 36);
	if (global_signal.pid != 0)
	{
		global_signal.exit_status = 128 + 3;
		global_signal.sigquit = 1;
	}
	else
	{
		//some error somewhere?
	}
}

void	sigint_handler(int signum) // crtl + C , repeat prompt /
{
	(void)signum;
	global_signal->sigint = 1;
	if (global_signal->pid == 0) //if its a child process
	{
		global_signal->exit_status = 1;
		keep_running = 0;
	}
	else
	{
		global_signal->exit_status = 128 + 2; //https://unix.stackexchange.com/questions/386836/why-is-doing-an-exit-130-is-not-the-same-as-dying-of-sigint
		keep_running = 0;
	}
	//have to get somehow the exit status, 1 iter27 or 0 and assign it into a struct?
}
*/

void	init_signal(void) //should this be global?
{
	g_signal.sigint = 0;
	g_signal.sigquit = 0;
	g_signal.finished = false;
	g_signal.pid = getpid();
	g_signal.keep_running = 1;
}
