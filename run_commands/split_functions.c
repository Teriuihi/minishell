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

static t_bool	is_special_char(char c)
{
	return (c == '~' || c == '#' || c == '$'|| c == '&' 
		|| c == '*' || c == '(' || c == ')' || c == '|'
		|| c == '[' || c == ']' || c == '{' || c == '}'
		|| c == ';' || c == '>' || c == '<' || c == '/'
		|| c == '?' || c == '!' || c == '-' || c == '%');
}

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


static t_bool	var_names_correct(char *key) //export var1$ = hi var3 var
{
	int		j;
	
	j = 0;
	ft_printf(1, "%s is key\n", key);
	while (key[j] != '\0' && is_special_char(key[j]) == false)
	{
		j++;
	}
	return (key[j] == 0);
}

t_exit_state	splitter2(t_command *command, int i, char **splitted,
								t_minishell *minishell)
{
	int	was_there_equal;
	int	k;

	was_there_equal = 0;
	k = 0;
	//if splitted[0] is valid continue, otherwise flikker
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
		{
			return (false);
		}
		if (var_names_correct(splitted[0]) == false)
		{
			ft_printf(2, "some shell: export: '%s': not a valid identifier\n", splitted[0]);
					(*i)++;
			continue ;
			//return (CONTINUE);
		}	
		if (split_len(splitted) == 1)
		{
			if (splitter2(command, *i, splitted, minishell) == RET)
				return (true);
		}
		ft_set_env(splitted[0], splitted[1], minishell->env, true);
		free_splitted(splitted);
		(*i)++;
	}
	return (true);
}
