/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   verify_key.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/25 17:05:50 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/25 17:05:50 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/bool.h"

static t_bool	is_special_char(char c)
{
	return (c == '~' || c == '#' || c == '$' || c == '&'
		|| c == '*' || c == '(' || c == ')' || c == '|'
		|| c == '[' || c == ']' || c == '{' || c == '}'
		|| c == ';' || c == '>' || c == '<' || c == '/'
		|| c == '?' || c == '!' || c == '-' || c == '%');
}

t_bool	var_names_correct(char *key)
{
	int	j;

	j = 0;
	while (key[j] != '\0' && is_special_char(key[j]) == false)
	{
		j++;
	}
	return (key[j] == 0);
}
