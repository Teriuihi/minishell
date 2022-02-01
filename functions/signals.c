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



void	crtld_handler(int signum) //https://stackoverflow.com/questions/1516122/how-to-capture-controld-signal
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
}

