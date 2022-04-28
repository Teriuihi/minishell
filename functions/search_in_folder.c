/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   search_in_folder.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/18 17:04:38 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/18 17:04:38 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../builtins/builtins.h"
#include <dirent.h>
#include "internal_functions.h"

t_bool	init_search_in_folder(char **directory_path, DIR **directory,
								struct dirent **file, t_minishell *minishell)
{
	char	*cwd_path;

	cwd_path = get_pwd(minishell);
	if (cwd_path == NULL)
		return (false);
	*directory = opendir(cwd_path);
	if (!*directory)
		return (false);
	*directory_path = ft_strjoin(cwd_path, "/");
	if (!*directory_path)
	{
		free(*directory);
		return (false);
	}
	*file = readdir(*directory);
	return (true);
}

t_exit_state	search_in_folder3(struct stat sb, struct dirent *file,
					char *command, char **result)
{
	char	*tmp;

	if (!S_ISDIR(sb.st_mode))
	{
		tmp = ft_strjoin("./", file->d_name);
		if (!tmp)
		{
			free(*result);
			return (BREAK);
		}
		if (ft_streq(command, tmp))
		{
			free(tmp);
			return (RET);
		}
		free(tmp);
	}
	free(*result);
	return (CONTINUE);
}

t_exit_state	search_in_folder2(char *directory_path, struct dirent *file,
					char *command, char **result)
{
	struct stat		sb;

	*result = ft_strjoin(directory_path, file->d_name);
	if (!*result || stat(*result, &sb) == -1)
	{
		free(*result);
		return (BREAK);
	}
	return (search_in_folder3(sb, file, command, result));
}

char	*search_in_folder(char *command, t_minishell *minishell)
{
	char			*directory_path;
	DIR				*directory;
	struct dirent	*file;
	char			*result;
	t_exit_state	state;

	if (init_search_in_folder(&directory_path, &directory,
			&file, minishell) == false)
		return (NULL);
	while (file)
	{
		state = search_in_folder2(directory_path, file, command, &result);
		if (state == RET)
		{
			closedir(directory);
			free(directory_path);
			return (result);
		}
		if (state == BREAK)
			break ;
		file = readdir(directory);
	}
	closedir(directory);
	free(directory_path);
	return (NULL);
}
