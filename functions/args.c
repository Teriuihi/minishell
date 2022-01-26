/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   args.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

/**
 * Converts a user input string to arguments
 *
 * @param	input	User input
 *
 * @return	Malloced string array
 */
char	**get_args(char *input)
{
	if (input == NULL)
		return (NULL);
	return (ft_split(input, ' '));
}
