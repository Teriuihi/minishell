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

static t_hash_table	*get_hash_table(void)
{
	t_hash_table	*table;

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
		return (false);
	rl_getc_function = (int (*)(FILE *)) interruptible_getc;
	minishell->cur_wd = cur_dir;
	minishell->env = get_hash_table();
	if (minishell->env == NULL)
		return (false);
	if (set_pwd(ft_strdup(cur_dir), minishell) == false)
		return (false);
	increase_shell_level(minishell);
	minishell->home = ft_get_env_val("HOME", minishell->env, &success);
	if (success == false)
		return (false);
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
		check_status();
	}
	return (g_signal.exit_status);
}

/* functions for the tester
int	ret_main(char *argv)
{
	t_minishell		minishell;

	if (init_succeeded(&minishell) == false)
	{
		exit(1);
	}
	init_signal();
	//while (g_signal.veof != 1 && g_signal.shell_level >= 0)
	//{
	//	while (g_signal.sigint != 1 && g_signal.veof != 1)
	//	{
			start_program_loop(&minishell, argv);
	//	}
		check_status();
	//}
	return (g_signal.exit_status);
}

int main(int argc, char **argv)
{
  // Your code...
  	//ft_printf(1, "%s is argv 1\n", argv[1]);
  //if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
    int exit_status = ret_main(argv[1]);
    ft_printf(1, " exit code : %d\n", exit_status);
	return (0);
	//exit(exit_status);
    // Above this is the function that normally launch your minishell, instead 
    // of reading line with a get_next_line or a read() on fd 0, you just have to get
    // the argv[2] (which contains the content) and execute it.
    // Your function should return the good exit status otherwise the tests may be considered as false.
  // Your code ...
}

*/