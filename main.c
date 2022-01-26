/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:22:29 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:22:29 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "headers/functions.h"
#include <readline/history.h>

void	free_input_args(char *input, char **args)
{
	char	**tmp;

	tmp = args;
	free(input);
	while (*args)
	{
		free(*args);
		args++;
	}
	free(tmp);
}

int	main(void)
{
	char	*input;
	char	**args;

	input = readline("some shell> ");
	while (input)
	{
		args = get_args(input);
		if (args == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		if (!ft_strncmp(args[0], "exit", 4))
			return (0);
		free_input_args(input, args);
		input = readline("some shell> ");
	}
	return (0);
}
