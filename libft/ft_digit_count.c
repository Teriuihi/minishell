/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_digit_count.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:28 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:28 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * Checks if a string ends with a specific suffix
 *
 * @param	n		number
 * @param	base	base
 *
 * @return	the amount of digits to be found in n, in base (n = 123, base = 10 -> 3)
 */

int	ft_digit_count(long int n, int base)
{
	int	len;

	len = 0;
	if (n == 0 || n < 0)
		len++;
	if (n < 0)
		n = -n;
	while (n)
	{
		len++;
		n = n / base;
	}
	return (len);
}
