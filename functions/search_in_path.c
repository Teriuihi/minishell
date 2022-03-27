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
	char		*tmp;
	char		*path;
	char		**split_path;
	struct stat	sb;

	path = ft_get_env_val("PATH", minishell->env);
	//ft_printf("%s is path\n");
	if (path == NULL)
	{
		ft_printf("PATH IS NOT FOUND ANYMORE\n");
	}
	//we should pull the path from the table at this point
	//path = get_path();
	//if (path == NULL)
	//	return (NULL);
	split_path = ft_split(path, ':');
	i = 1;
	while (split_path[i])
	{
		chdir(split_path[i]);
		if (stat(command, &sb) == 0)
		{
			tmp = ft_strjoin(split_path[i], "/");
			return (ft_strjoin(tmp, command));
		}
		i++;
	}
	return (NULL);
}


char	*search_folder(char *command, t_minishell *minishell)
{
	char			*cwd_path;
	char			*directroy_path;
	char			*executable_path;
	DIR				*directory;
	struct dirent	*file;
	struct stat		sb;

	cwd_path = NULL;
	cwd_path = get_pwd(minishell);
	if (cwd_path == NULL)
		exit(1);
	directory = opendir(cwd_path);
	directroy_path = ft_strjoin(cwd_path, "/");
	if (!directroy_path)
		exit(1);
	while ((file = readdir(directory)))
	{
		executable_path = ft_strjoin(directroy_path, file->d_name);
		if (!executable_path)
			exit(1);
		if (stat(executable_path, &sb) == -1)
		{
			ft_printf("STAT HAS ERROR\n");
		}
		if (!S_ISDIR(sb.st_mode)) //if its not a directory
		{
			if (ft_streq(command, ft_strjoin("./", file->d_name))) //check if the given command is also ./ + commandname (eg: ./a.out)
			{
				//if name if ./minishell , increase level
				//change the val of $SHLVL
				if (ft_streq("minishell", file->d_name))
				{
					g_signal.shell_level += 1;
					g_signal.minishell_exec_found = 1;
				}
				//TODO FREE
				closedir(directory); //close dir before returning
				return (executable_path); //return the entire executable path
			}
		}
	}
	free(directroy_path);
	free(executable_path);
	closedir(directory);
	return (NULL);
}

