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
#include "run_commands_internal.h"

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

static t_exit_state	splitter2(t_command *command, int i, char **splitted,
								 t_minishell *minishell)
{
	int	was_there_equal;
	int	k;

	was_there_equal = 0;
	k = 0;
	while (command->args[i + 1][k] != '\0')
	{
		if (command->args[i + 1][k] == '=')
			was_there_equal = 1;
		k++;
	}
	if (was_there_equal == 1)
	{
		ft_set_env(splitted[0], "", minishell->env, true);
		free_splitted(splitted);
		return (RETURN);
	}
	return (CONTINUE);
}

static t_bool	splitter(int *i, t_command *command, t_minishell *minishell)
{
	char	**splitted;

	while (*i < command->args_len - 1)
	{
		splitted = ft_split(command->args[*i + 1], '=');
		if (splitted == NULL)
			return (false);
		if (splitted[2] != 0)
		{
			ft_printf(2, "YOU DIDNT STOP AFTER THE FIRST EQUALS SIGN\n"); //TODO fix
			return (true);
		}
		if (split_len(splitted) == 1)
		{
			if (splitter2(command, *i, splitted, minishell) == RETURN)
				return (true);
		}
		ft_set_env(splitted[0], splitted[1], minishell->env, true);
		free_splitted(splitted);
		(*i)++;
	}
	return (true);
}

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

	if (!command || !minishell)
		return (set_exit_status(minishell, 2, NULL, false));
	if (command->export_found == true)
		return (export_found(command, minishell));
	splitted = ft_split(command->command, '=');
	if (!splitted)
		return (set_exit_status(minishell, 1, NULL, false));
	if (ft_get_env_val(splitted[0], minishell->env, &success) != NULL)
	{
		if (succesful_insert(minishell->env, splitted[0], splitted[1], true)
			== true)
			return (set_exit_status(minishell, 0, NULL, false));
		else
			return (set_exit_status(minishell, 1, NULL, false));
	}
	else if (ft_set_env(splitted[0], splitted[1], minishell->env, false)
		== false)
		return (set_exit_status(minishell, 1, NULL, false));
	else
		return (set_exit_status(minishell, 0, NULL, false));
}

t_bool	execute_builtin(t_command *command, t_minishell *minishell)
{
	char		*cur_dir;

	cur_dir = get_pwd(minishell);
	if (!command->command || !minishell || !cur_dir)
		return (set_exit_status(minishell, 1, NULL, false));
	if (ft_streq(command->command, "export") == 1 && command->args_len == 1)
		return (export((void *)minishell));
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
		return (ft_remove_exported_var(command->args[1], minishell->env,
				minishell));
	else if (ft_streq(command->command, "cd"))
		return (cd(command, minishell));
	else if (env_variable_found(command) == true)
		return (env_var_added(command, minishell));
	else
		return (set_exit_status(minishell, 1, NULL, false));
}

t_bool	ft_pwd(char *cur_dir, t_minishell *minishell)
{
	ft_putstr_fd(cur_dir, 1);
	ft_putstr_fd("\n", 1);
	return (set_exit_status(minishell, 0, NULL, false));
}
