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
#include "../hashtable/export.h"
#include "split_functions.h"
/**
 * Takes an array of arrays (user input)
 * Selects the function to execute based on the first argument (args[0])
 * @param	args	the input given by the user
 *
 * @return	void
 */

static t_bool	export_found(t_command *command, t_minishell *minishell)
{
	int		i;

	i = 0;
	if (!command->args || !command->args[i] || command->args_len <= 1)
	{
		return (false);
	}
	command->export_found = false;
	while (command->args[i] != NULL)
	{
		if (!ft_streq(command->args[i], "export"))
		{
			i++;
			continue ;
		}
		if (splitter(&i, command, minishell) == false)
			return (false);
		return (true);
	}
	return (false);
}

static t_bool	env_var_added(t_command *command, t_minishell *minishell)
{
	char	**splitted;
	t_bool	success;
	char	*env_val;

	if (!command || !minishell)
		return (set_exit_status(minishell, 2, NULL, false));
	if (command->export_found == true)
		return (export_found(command, minishell));
	splitted = ft_split_first(command->command, '=');
	if (!splitted)
		return (set_exit_status(minishell, 1, NULL, false));
	env_val = ft_get_env_val(splitted[0], minishell->env, &success);
	if (env_val != NULL)
	{
		free(env_val);
		if (succesful_insert(minishell->env, splitted[0], splitted[1], true)
			== true)
			return (set_exit_status(minishell, 0, NULL, false));
		else
			return (set_exit_status(minishell, 1, NULL, false));
	}
	else if (ft_set_env(splitted[0], splitted[1], minishell->env, false)
		== false)
	{
		free_splitted(splitted);
		return (set_exit_status(minishell, 1, NULL, false));
	}
	free_splitted(splitted);
	return (set_exit_status(minishell, 0, NULL, false));
}

t_bool	execute_builtin(t_command *command, t_minishell *minishell)
{
	char		*cur_dir;

	cur_dir = get_pwd(minishell);
	if (!command->command || !minishell || !cur_dir)
		return (set_exit_status(minishell, 1, NULL, false));
	if (ft_streq(command->command, "export") == 1 && command->args_len == 1)
		return (export_cmd((void *)minishell));
	else if (env_variable_found(command) == true)
		return (env_var_added(command, minishell));
	else if (ft_streq(command->command, "echo"))
		return (ft_echo(command, 1, minishell));
	else if (ft_streq(command->command, "pwd"))
		return (ft_pwd(cur_dir, minishell));
	else if (ft_streq(command->command, "env"))
		return (print_h_table(minishell->env, command->args_len));
	else
		return (set_exit_status(minishell, 1, NULL, false));
}

t_bool	remove_all_exported_vars(char **args, t_minishell *minishell)
{
	t_bool	res;
	int		i;

	i = 1;
	while (args[i])
	{
		res = ft_remove_exported_var(args[i], minishell->env, minishell);
		if (res == false)
			return (res);
		i++;
	}
	return (true);
}

t_bool	execute_non_forked_builtin(t_command *command, t_minishell *minishell)
{
	char	*cur_dir;

	cur_dir = get_pwd(minishell);
	if (!command->command || !minishell || !cur_dir)
		return (set_exit_status(minishell, 1, NULL, false));
	else if (ft_streq(command->command, "unset"))
		return (remove_all_exported_vars(
				command->args, minishell));
	else if (ft_streq(command->command, "cd"))
		return (cd(command, minishell));
	else if (env_variable_found(command) == true)
		return (env_var_added(command, minishell));
	else
		return (set_exit_status(minishell, 1, NULL, false));
}
