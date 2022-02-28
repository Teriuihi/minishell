/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_string_append.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 17:01:39 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/28 17:01:39 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/string.h"
#include "string_internal.h"

/**
 * Create a new string from two strings, free's on success
 *
 * @param	s1	String 1
 * @param	s2	String 2
 *
 * @return	The new string or NULL on failure
 */
t_string	*join_to_new(t_string *s1, t_string *s2)
{
	t_string	*s_new;
	int			total_len;

	total_len = s1->len + s2->len;
	s_new = create(total_len + 100 - (total_len % 100));
	if (s_new == NULL)
		return (NULL);
	s_new->len = total_len;
	ft_strlcpy(s_new->s, s1->s, s1->len);
	ft_strlcpy(s_new->s + s1->len, s2->s, s2->len);
	free_string(s1);
	free_string(s2);
	return (s_new);
}

/**
 * Join two strings, free's anything that's no longer user
 * 	Doesn't free on failure
 * @param	s1	String one
 * @param	s2	String two
 *
 * @return	The string containing the data from s1 and s2 or NULL on failure
 */
t_string	*join_strings(t_string *s1, t_string *s2)
{
	if (s1->empty >= s2->len)
	{
		ft_strlcpy(s1->s + s1->len, s2->s, s2->len);
		s1->len += s2->len;
		s1->empty -= s2->len;
		free_string(s2);
		return (s1);
	}
	else
	{
		return (join_to_new(s1, s2));
	}
}

/**
 * Append chars to a string
 *
 * @param	s	String to append to
 * @param	arr	Chars to append
 *
 * @return	String with the data from s and arr or NULL on failure
 */
t_string	*append_char(t_string *s, char *arr)
{
	int			arr_len;
	t_string	*new_s;

	if (s == NULL)
		return (NULL);
	if (arr == NULL)
		return (s);
	arr_len = ft_strlen(arr);
	if (s->empty >= arr_len)
	{
		ft_strlcpy(s->s + arr_len, arr, arr_len);
		s->len += arr_len;
		s->empty -= arr_len;
		return (s);
	}
	else
	{
		new_s = create(s->len + arr_len + 100);
		if (s == NULL)
			return (NULL);
		new_s->len = s->len + arr_len;
		ft_strlcpy(new_s->s, s->s, s->len);
		ft_strlcpy(new_s->s + s->len, arr, arr_len);
		return (new_s);
	}
}
