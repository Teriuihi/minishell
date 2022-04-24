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
#include "../hashtable/hash_utils.h"
#include "split_functions.h"

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
