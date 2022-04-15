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
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h> 
#include <stdio.h>
#include <errno.h>

t_signal	g_signal;

t_hash_table	*get_hash_table(void)
{
	static t_hash_table	*table;

	table = create_env_h_table();
	if (table == NULL)
	{
		return (NULL);
	}
	return (table);
}

t_bool	increase_shell_level(t_minishell *minishell)
{
	int		val;
	char	*level;
	char	*increased_level;
	t_bool	success;

	level = getenv("SHLVL");
	if (!level)
	{
		return (false);
	}
	val = ft_atoi(level, &success);
	if (success == false)
	{
		return (false);
	}
	increased_level = ft_itoa(val + 1);
	if (!increased_level)
	{
		return (false);
	}
	return (ft_set_env("SHLVL", increased_level, minishell->env, true));
}

t_bool	init_succeeded(t_minishell *minishell)
{
	char	*cur_dir;
	t_bool	success;

	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		return (false);
	}
	rl_getc_function = (int (*)(FILE *)) interruptible_getc;
	minishell->cur_wd = cur_dir;
	minishell->env = get_hash_table();
	if (minishell->env == NULL)
	{
		return (false);
	}
	if (set_pwd(ft_strdup(cur_dir), minishell) == false)
	{
		return (false);
	}
	increase_shell_level(minishell);
	minishell->home = ft_get_env_val("HOME", minishell->env, &success);
	if (success == false)
	{
		return (false);
	}
	return (true);
}

int	main(void)
{
	t_minishell		minishell;

	if (init_succeeded(&minishell) == false)
	{
		exit(1);
	}
	init_signal();
	while (g_signal.veof != 1 && g_signal.shell_level >= 0)
	{
		while (g_signal.sigint != 1 && g_signal.veof != 1)
		{
			start_program_loop(&minishell);
		}
		check_status(&minishell);
	}
	return (0);
}

/*

void	init_keyboard()
{
	//tcgetattr(g_signal.terminal_descriptor, &g_signal.old_termios); //get initial setups
	//tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.new_termios);

	g_signal.new_termios = g_signal.old_termios;
	tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.new_termios);

	//g_signal.new_termios.c_cc[VMIN] = 1;
	//g_signal.new_termios.c_cc[VMIN] = 0;
	//g_signal.new_termios.c_lflag |= (ICANON | ISIG | ECHO);
	//g_signal.new_termios.c_cc[VINTR] = 3;
	//g_signal.new_termios.c_cc[VEOF] = 4;
	//g_signal.new_termios.c_lflag &= ~ISIG;
	//g_signal.new_termios.c_cc[VQUIT] = 15;

	//g_signal.old_termios.c_lflag |= (ICANON | ECHO);
	//g_signal.old_termios.c_cc[VINTR] = 3;
	//g_signal.old_termios.c_cc[VEOF] = 4;
	//g_signal.old_termios.c_cc[VQUIT] = 2;

	signal(SIGINT, sigquit_handler); //this is important for crtlc
	signal(SIGQUIT, SIG_IGN);

}
void	init_termios(void)
{
	g_signal.old_termios.c_lflag |= ISIG;

	if (isatty(STDERR_FILENO))
		g_signal.terminal_descriptor = STDERR_FILENO;
	if (isatty(STDIN_FILENO))
		g_signal.terminal_descriptor = STDIN_FILENO;
	if (isatty(STDOUT_FILENO))
		g_signal.terminal_descriptor = STDOUT_FILENO;
	
	//g_signal.old_termios.c_cc[VMIN] = 1;
	//g_signal.old_termios.c_cc[VMIN] = 0;
	//g_signal.old_termios.c_lflag |= (ICANON | ECHO);

	//g_signal.old_termios.c_cc[VINTR] = 3;
	//g_signal.old_termios.c_cc[VEOF] = 4;
	if (tcgetattr(g_signal.terminal_descriptor, &g_signal.old_termios)
		|| tcgetattr(g_signal.terminal_descriptor, &g_signal.new_termios))
	{
		exit(1);
	}
	tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.new_termios);
	//tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.new_termios);

}

*/