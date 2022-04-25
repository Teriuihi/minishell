/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   split_functions.c                                        :+:    :+:      */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "split_functions.h"
#include "verify_key.h"

int	split_len(char **splitted)
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

t_exit_state	splitter2(t_command *command, int i, char **splitted,
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
		return (CONTINUE);
	}
	return (CONTINUE);
}

char	**create_array_with_one_arg(char *str)
{
	char	**arr;

	arr = ft_calloc(2, sizeof(char *));
	if (arr == NULL)
		return (NULL);
	arr[0] = ft_strdup(str);
	if (arr[0] == NULL)
	{
		free (arr);
		return (NULL);
	}
	return (arr);
}

t_bool	splitter1(int *i, t_command *command, t_minishell *minishell,
			char **splitted)
{
	if (split_len(splitted) == 1)
	{
		if (splitter2(command, *i, splitted, minishell) == RET)
			return (true);
	}
	ft_set_env(splitted[0], splitted[1], minishell->env, true);
	free_splitted(splitted);
	(*i)++;
	return (false);
}

t_bool	splitter(int *i, t_command *command, t_minishell *minishell)
{
	char	**splitted;

	while (*i < command->args_len - 1)
	{
		if (!ft_contains(command->args[*i + 1], '='))
			splitted = create_array_with_one_arg(command->args[*i + 1]);
		else
			splitted = ft_split_first(command->args[*i + 1], '=');
		if (splitted == NULL)
			return (false);
		if (splitted[0][0] == 0 || var_names_correct(splitted[0]) == false)
		{
			ft_printf(2, "some shell: export: '%s': not a valid identifier\n",
				splitted[0]);
			(*i)++;
			free_splitted(splitted);
			g_signal.exit_status = 1;
			continue ;
		}
		if (splitter1(i, command, minishell, splitted) == true)
			return (true);
	}
	return (true);
}
