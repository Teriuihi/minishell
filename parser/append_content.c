/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   appened_content.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/04 18:12:35 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/04 18:12:35 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../string/string.h"
#include "../libft/libft.h"
#include "internal_parser.h"
#include "../headers/functions.h"

/**
 * Appends everything from input between start and pos
 *
 * @param	input	Input to append from
 * @param	start	Start pos in input
 * @param	pos		End in input (current pos)
 * @param	arg		String to append to
 *
 * @return	String we appended too (could have a different address now)
 */
t_bool	append_content(t_parse_data *data)
{
	char		*to_add;

	to_add = ft_calloc((data->pos - data->start) + 2, sizeof(char));
	if (to_add == NULL)
		return (new_set_exit_status(1, "some shell: Out of memory.\n"));
	ft_strlcpy(to_add, data->input + data->start, data->pos - data->start + 1);
	data->string = append_char_array(data->string, to_add);
	free(to_add);
	if (data->string == NULL)
		return (new_set_exit_status(1, "some shell: Out of memory.\n"));
	return (true);
}
