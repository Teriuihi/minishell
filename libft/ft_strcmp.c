/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strcmp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/18 19:30:17 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/18 19:30:17 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_strcmp(char *str1, char *str2)
{
	if (str1 == NULL || str2 == NULL)
		return (0);
	while (*str1 != 0)
	{
		if (*str2 == 0)
			return (1);
		else if (*str1 != *str2)
			break ;
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}
