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

#include "../builtins/builtins.h"
#include <readline/readline.h>

t_signal	g_signal;

static t_hash_table	*get_hash_table(char **envp)
{
	t_hash_table	*table;

	table = create_env_h_table(envp);
	if (table == NULL)
	{
		return (NULL);
	}
	return (table);
}

static t_bool	increase_shell_level(t_minishell *minishell)
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
	success = ft_set_env("SHLVL", increased_level, minishell->env, true);
	free(increased_level);
	return (success);
}

static t_bool	init_succeeded(t_minishell *minishell, char **argv, char **envp)
{
	char	*cur_dir;
	t_bool	success;

	if (ft_streq("./minishell", argv[0]) == 0)
		return (false);
	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
		return (false);
	rl_getc_function = (int (*)(FILE *)) interruptible_getc;
	minishell->cur_wd = ft_strdup(cur_dir);
	free(cur_dir);
	if (minishell->cur_wd == NULL)
		return (false);
	minishell->env = get_hash_table(envp);
	if (minishell->env == NULL)
		return (false);
	if (set_pwd(ft_strdup(minishell->cur_wd), minishell) == false)
		return (false);
	increase_shell_level(minishell);
	minishell->home = ft_get_env_val("HOME", minishell->env, &success);
	if (success == false)
		return (false);
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell		minishell;

	if (argc != 1 || init_succeeded(&minishell, argv, envp) == false)
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
	exit(g_signal.exit_status);
}
