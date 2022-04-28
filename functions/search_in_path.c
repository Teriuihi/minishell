/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   search_in_path.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "internal_functions.h"
#include "../hashtable/manipulate_env.h"

t_exit_state	search_in_path_2(char *command, char *split_path,
					char **path)
{
	char		*tmp_path;
	struct stat	sb;

	if (chdir(split_path) == 0 && stat(command, &sb) == 0)
	{
		tmp_path = ft_strjoin(split_path, "/");
		if (!tmp_path)
			return (ERROR);
		*path = ft_strjoin(tmp_path, command);
		free(tmp_path);
		if (!*path)
			return (ERROR);
		else
			return (RET);
	}
	return (CONTINUE);
}

char	*search_in_path_1(char **split_path, char *command)
{
	char			*result;
	t_exit_state	state;
	int				i;

	i = 0;
	while (split_path[i])
	{
		state = search_in_path_2(command, split_path[i], &result);
		i++;
		if (state == ERROR)
		{
			free_splitted(split_path);
			return (NULL);
		}
		if (state == RET)
			return (result);
	}
	return (NULL);
}

/**
 * Searches for executables in the path environ variable
 * 	Concatenates the directory path if executable (eg: ls)
 * 	is found in the PATH variable
 *
 * @param	input	User input args from terminal
 *
 * @return	void
 */
char	*search_in_path(char *command, t_minishell *minishell)
{
	char			*path;
	char			**split_path;
	t_bool			success;

	path = ft_get_env_val("PATH", minishell->env, &success);
	if (path == NULL)
		return (NULL);
	split_path = ft_split(path, ':');
	free(path);
	if (!split_path)
		return (NULL);
	path = search_in_path_1(split_path, command);
	free_splitted(split_path);
	return (path);
}
