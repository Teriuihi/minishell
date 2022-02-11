/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_path.c                                               :+:    :+:      */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include	"../libft/libft.h"
/**
* searches the PATH variable in extern char **environ; 
*
* @param	input	User input
*
* @return	the PATH variable as string 
*/

extern char	**environ;

char	*get_path(void)
{
	int	i;

	i = 0;
	while (environ[i] != NULL)
	{
		if (ft_strncmp(environ[i], "PATH", 4) == 0)
			return (environ[i]);
		i++;
	}
	return (NULL);
}
