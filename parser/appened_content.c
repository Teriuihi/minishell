#include "../string/string.h"
#include "../libft/libft.h"

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
t_string	*append_content(char *input, int start, int pos, t_string *arg)
{
	char		*to_add;
	t_string	*result;

	if (input == NULL)
	{
		free_string(arg);
		return (NULL);
	}
	to_add = ft_calloc((pos - start) + 2, sizeof(char));
	if (to_add == NULL)
	{
		free_string(arg);
		return (NULL);
	}
	ft_strlcpy(to_add, input + start, pos - start + 1);
	result = append_char(arg, to_add);
	if (result == NULL)
		free_string(arg);
	return (result);
}
