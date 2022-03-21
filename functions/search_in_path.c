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
char	*search_in_path(char *command)
{
	int			i;
	char		*tmp;
	char		*path;
	char		**split_path;
	struct stat	sb;

	path = get_path();
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

/*
void	search_folder(char *command)
{
	int				entries;
	char			*tmp;
	char			*buf;

	struct dirent	*file;
	DIR				*directory;
	char			**split_path;
	struct stat		sb;

	entries = 0;
	directory = NULL;
	tmp = NULL;
	tmp = getcwd(tmp, 0);
	if (tmp == NULL)
	{
		ft_printf("couldnt use getcwd\n");
	}
	directory = opendir(tmp);
	while ((file = readdir(directory)))
	{
		if (file->d_name[0] == '.') //what happens if its not hidden?
			continue ;
		entries++;
		char buffer[1024];
		tmp = ft_strjoin(ft_strjoin(tmp, "/"), file->d_name);
		ft_printf("%s is joined path\n", tmp);
		if (stat(command, &sb) == -1)
		{
			ft_printf("STAT IS ERROR\n");
		}
		ft_printf("%d is entries\n", entries);
		if (S_ISDIR(sb.st_mode))
			ft_printf("DIR\n");
		else
			ft_printf("FILE\n");
		ft_printf("%s is filename\n", file->d_name);
	}
}
*/
