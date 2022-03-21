/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:22:29 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:22:29 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../buildins/buildins.h"
#include "../hashtable/hashtable.h"
#include "../headers/minishell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h> 
#include <stdio.h>
#include <errno.h>

t_signal	g_signal;

t_hash_table	*get_hash_table(void)
{
	static t_hash_table	*table;

	if (!table)
	{
		table = create_env_h_table();
		if (table == NULL)
		{
			exit (1);
		}
	}
	return (table);
}

void	set_termios(void)
{
	if (isatty(STDERR_FILENO))
		g_signal.terminal_descriptor = STDERR_FILENO;
	if (isatty(STDIN_FILENO))
		g_signal.terminal_descriptor = STDIN_FILENO;
	if (isatty(STDOUT_FILENO))
		g_signal.terminal_descriptor = STDOUT_FILENO;
	if (tcgetattr(g_signal.terminal_descriptor, &g_signal.old_termios)
		|| tcgetattr(g_signal.terminal_descriptor, &g_signal.new_termios))
	{
		exit(1);
	}
	g_signal.new_termios.c_lflag |= (ICANON | ISIG | ECHO);
	g_signal.new_termios.c_cc[VINTR] = 3;
	g_signal.new_termios.c_cc[VEOF] = 4;
	g_signal.old_termios.c_lflag |= (ICANON | ISIG | ECHO);
	g_signal.old_termios.c_cc[VINTR] = 3;
	g_signal.old_termios.c_cc[VEOF] = 4;
	signal(SIGINT, sigquit_handler);
}

void	init(t_minishell *minishell)
{
	char		*cur_dir;

	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		exit(1);
	}
	minishell->cur_wd = cur_dir;
	minishell->exit_status = 0;
	minishell->env = get_hash_table();
	set_pwd(ft_strdup(cur_dir), minishell);
}

int	main(void)
{
	t_minishell		minishell;

	init(&minishell);
	init_signal();
	while (g_signal.veof != 1)
	{
		set_termios();
		while (g_signal.sigint != 1 && g_signal.veof != 1)
		{
			start_program_loop(&minishell);
		}
		check_status();
	}
	return (0);
}