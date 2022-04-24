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

#include "pipe_utils.h"

t_bool	close_pipes(int *pid1, int *pid2)
{
	if (pid1 != NULL && *pid1 != -1)
	{
		if (close(*pid1) == -1)
		{
			*pid1 = -1;
			return (new_set_exit_status(1, "close error pid1\n"));
		}
		else
		{
			*pid1 = -1;
		}
	}
	if (pid2 != NULL && *pid2 != -1)
	{
		if (close(*pid2) == -1)
		{
			*pid2 = -1;
			return (new_set_exit_status(1, "close error pid2\n"));
		}
		else
		{
			*pid2 = -1;
		}
	}
	return (true);
}
