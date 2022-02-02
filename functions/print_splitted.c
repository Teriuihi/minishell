/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_working_directory.c                                  :+:    :+:      */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"

/*
 * Takes an char ptr path
 * If path == NULL we get the current working directory and return it as a string.
 * Else we return path, esentially functioning as a setter
 *
 * @param	args	path string
 *
 * @return	a path, either current working dir or what we set it to be
*/




void	print_splitted(char **args)
{
	int i;

	i = 0;
	if (!args)
	{
		return ;
	}
	while (args[i])
	{
		ft_printf("%s\n", args[i]);
		i++;
	}
}