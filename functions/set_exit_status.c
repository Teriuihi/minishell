/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   set_exit_status.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 14:13:16 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/04 14:13:16 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "set_exit_status.h"

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
		if (status != 0)
			ft_printf(2, "%s\n", message);
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
