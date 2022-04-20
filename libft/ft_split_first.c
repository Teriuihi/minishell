/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split_first.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/20 16:37:29 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/20 16:37:29 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_split_first(char *str, char c)
{
	char	**res;
	int		split_char;

	split_char = 0;
	while (str[split_char] && str[split_char] != c)
		split_char++;
	if (str[split_char] != c)
		return (NULL);
	res = ft_calloc(3, sizeof(char *));
	if (res == NULL)
		return (NULL);
	res[0] = ft_calloc(split_char + 1, sizeof(char));
	res[1] = ft_strdup(str + split_char + 1);
	if (res[0] == NULL || res[1] == NULL)
	{
		free(res[0]);
		free(res[1]);
		free(res);
		return (NULL);
	}
	ft_strncpy(res[0], str, split_char);
	return (res);
}
