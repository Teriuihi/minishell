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

# include "../libft/libft.h"
# include "../headers/functions.h"
# include "../buildins/buildins.h"
# include <dirent.h>

extern char		**environ;

/*
* Searches for excecutables in the path environ variable
* Concatenates the directory path if executable (eg: ls) 
				is found in the PATH variable
* @param	input	User input args from terminal
*
* @return void
*/
char	*search_in_path(char *command, t_minishell *minishell)
{
	int			i;
	char		*tmp_path;
	char		*executable_path;
	char		*path;
	char		**split_path;
	struct stat	sb;
	t_bool		success;

	path = ft_get_env_val("PATH", minishell->env, &success);
	if (path == NULL)
		return (NULL);
	split_path = ft_split(path, ':');
	free(path);
	if (!split_path)
		return (NULL);
	i = 1;
	while (split_path[i])
	{
		if (chdir(split_path[i]) == 0) //meaning it just doestn exit
		{
			if (stat(command, &sb) == 0)
			{
				tmp_path = ft_strjoin(split_path[i], "/");
				if (!tmp_path)
				{
					free_splitted(split_path);
					return (NULL);
				}
				executable_path = ft_strjoin(tmp_path, command);
				free_splitted(split_path);
				free(tmp_path);	
				if (!executable_path)
					return (NULL);
				else
					return (executable_path);
			}
		}
		i++;
	}
	free_splitted(split_path);
	return (NULL);
}

char	*search_in_folder(char *command, t_minishell *minishell)
{
	char			*cwd_path;
	char			*directory_path;
	char			*executable_path;
	char			*tmp;
	DIR				*directory;
	struct dirent	*file;
	struct stat		sb;

	executable_path = NULL;
	cwd_path = get_pwd(minishell);
	if (cwd_path == NULL)
		return (NULL);
	directory = opendir(cwd_path);
	if (!directory)
		return (NULL);
	directory_path = ft_strjoin(cwd_path, "/");
	if (!directory_path)
		return (NULL);
	while ((file = readdir(directory)))
	{
		executable_path = ft_strjoin(directory_path, file->d_name);
		if (!executable_path || stat(executable_path, &sb) == -1)
			break ;
		if (!S_ISDIR(sb.st_mode)) //we could already here assign stuff?
		{
			tmp = ft_strjoin("./", file->d_name);
			if (!tmp)
				break ;
			if (ft_streq(command, tmp)) //check if the given command is also ./ + commandname (eg: ./a.out)
			{
				if (ft_streq("minishell", file->d_name))
				{
					g_signal.shell_level += 1;
					g_signal.minishell_exec_found = 1;
				}
				free(directory_path);
				free(tmp);
				closedir(directory);
				return (executable_path);
			}
			free(tmp);
		}
		free(executable_path);
		executable_path = NULL;
	}
	if (directory_path != NULL)
		free(directory_path);
	if (executable_path != NULL)
		free(executable_path);
	closedir(directory);
	return (NULL);
}

