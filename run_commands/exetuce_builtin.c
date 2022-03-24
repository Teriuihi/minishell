/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_builtin.c                                        :+:    :+:      */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../buildins/buildins.h"
#include "../headers/minishell.h"
#include "../headers/functions.h"

/**
 * Takes an array of arrays (user input)
 * Selects the function to execute based on the first argument (args[0])
 * @param	args	the input given by the user
 *
 * @return	void
 */
static int	split_len(char **splitted)
{
	int	i;

	i = 0;
	if (!splitted)
	{
		return (0);
	}
	while (splitted[i])
	{
		i++;
	}
	return (i);
}

static t_bool	export_found(t_command *command, t_minishell *minishell)
{
	char	**splitted;
	int		i;

	i = 0;
	while (command->args[i])
	{
		if (ft_streq(command->args[i], "export"))
		{
			splitted = ft_split(command->args[i + 1], '=');
			if (split_len(splitted) < 2)
			{
				free_splitted(splitted);
				return (false);
			}
			ft_set_env(splitted[0], splitted[1], minishell->env, true);
			free_splitted(splitted);
			return (true);
		}
		i++;
	}
	return (false);
}

static t_bool	env_var_added(t_command *command, t_minishell *minishell)
{	
	char	**splitted;

	if (!command || !minishell)
	{
		minishell->exit_status = 2;
		return (false);
	}
	if (export_found(command, minishell) == true)
	{	
		minishell->exit_status = 0;
		return (true);
	}
	splitted = ft_split(command->command, '=');
	if (split_len(splitted) != 2)
	{
		free_splitted(splitted);
		minishell->exit_status = 2;
		return (false);
	}
	if (ft_set_env(splitted[0], splitted[1], minishell->env, false) == false)
	{
		minishell->exit_status = 1;
		return (false);
	}
	else
	{
		minishell->exit_status = 0;
		return (true);
	}
}

t_bool	ft_env(t_hash_table *h_table, t_minishell *minishell)
{
	if (print_h_table(h_table) == false)
	{
		return (set_exit_status(minishell, 1));
	}
	else
	{
		return (set_exit_status(minishell, 0));
	}
}

t_bool	execute_non_forked_builtin(t_command *command, t_minishell *minishell)
{
	char	*cur_dir;
	
	cur_dir = get_pwd(minishell);
	if (!command->command || !minishell || !cur_dir)
		exit(1);
	
	else if (ft_streq(command->command, "cd"))
		return (cd(command, minishell));
	else if (ft_streq(command->command, "env"))
		return (ft_env(minishell->env, minishell));
	else if (ft_streq(command->command, "unset"))
		return (ft_remove_exported_var(command->args[1], minishell->env,
				minishell));
	else if (env_var_added(command, minishell) == true)
		return (true);
	return (set_exit_status(minishell, 1));
}

t_bool	ft_pwd(char *cur_dir, t_minishell *minishell)
{
	ft_putstr_fd(cur_dir, 1);
	ft_putstr_fd("\n", 1);
	return (set_exit_status(minishell, 0));
}

t_bool	execute_builtin(t_command *command, t_minishell *minishell)
{
	char		*cur_dir;
	t_bool		did_execution_succeed;

	did_execution_succeed = false;
	if (!command->command || !minishell)
	{
		return (set_exit_status(minishell, 1));
	}
	cur_dir = get_pwd(minishell);
	if (!cur_dir)
		return (set_exit_status(minishell, 1));
	if (env_var_added(command, minishell) == true)
		return (true);
	if (ft_streq(command->command, "echo"))
		return (ft_echo(command, 1, minishell));
	else if (ft_streq(command->command, "pwd"))
		return (ft_pwd(cur_dir, minishell));
	else
		return (set_exit_status(minishell, 1));
}
