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
