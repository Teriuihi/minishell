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
		return (RETURN);
	}
	return (CONTINUE);
}

t_bool	splitter(int *i, t_command *command, t_minishell *minishell)
{
	char	**splitted;

	while (*i < command->args_len - 1)
	{
		splitted = ft_split_first(command->args[*i + 1], '=');
		if (splitted == NULL)
		{
			return (false);
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
