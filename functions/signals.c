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

/**
 * Handling different signals based on signum
 *
 *
 * @param	args signum
 *
 * @return	void
 */

//https://stackoverflow.com/questions/1516122/how-to-capture-controld-signal
void	crtld_handler(int signum)
{
	(void)signum;
}

void	sigquit_handler(int signum) /* crtl + \ , do nothing */
{
	(void)signum;
	write(1, "entered to crtl + backslash signal\n", 36);
}

void	sigint_handler(int signum) /* crtl + C , repeat prompt */
{
	(void)signum;
	//have to get somehow the exit status, 127 or 0 and assign it into a struct?
}

t_signal	*init_signal(void) //should this be global?
{
	t_signal	*signal;

	signal = ft_calloc(1, sizeof(t_signal));
	if (!signal)
	{
		return (NULL);
	}
	signal->exit_status = 0;
	signal->pid = 0;
	return (signal);
}
