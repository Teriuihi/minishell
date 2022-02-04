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

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"
#include <readline/history.h>
#include <sys/stat.h>
#include <unistd.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	i;

	dst_len = ft_strlen(dst) >= dstsize ? dstsize : ft_strlen(dst);
	i = 0;
	if (dstsize == dst_len || dstsize == 0)
		return (dstsize + ft_strlen(src));
	if (ft_strlen(src) >= dstsize - dst_len)
	{
		while (i < dstsize - dst_len - 1)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	else
	{
		while (src[i] && i < dstsize - dst_len)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	dst[dst_len + i] = '\0';
	return (dst_len + ft_strlen(src));
}

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

void	eval(char **args)
{
	pid_t	cpid;

	if (is_builtin(args) == true)
	{
		execute_builtin(args);
		return ;
	}
	cpid = fork();
	if (cpid == 0)
	{
		if (execve(args[0], args, NULL) < 0)
		{
			ft_printf("Command not found\n");
			exit(0);
		}
	}
	else
	{
		wait(NULL); 
	}
}

int	main(void)
{
	t_signal		*signal_struct;
	//t_hash_table	*h_table;
	char			*input;
	char			**args;
	int				i;

	
	//print_splitted(environ);
	/*
	h_table = duplicates_are_found_in_argv();
	ft_printf("%s\n", ft_get_env_val("PWD", h_table));
	ft_set_env("TURO", "NEW VAR TURO", h_table);
	//ft_printf("%s\n", ft_get_env_val("TURO", h_table));
	//ft_remove_env("TURO", h_table);
	//ft_printf("%s\n", ft_get_env_val("TURO", h_table));
	ft_set_env("TERI", "NEW VAR TERI", h_table);
	ft_printf("%s\n", ft_get_env_val("TERI", h_table));
	ft_printf("%s\n", ft_get_env_val("TURO", h_table));
	 */
	signal_struct = init_signal();
	signal(SIGQUIT, sigquit_handler);
	i = 0;
	input = readline("some shell>");
	while (input)
	{
		add_history(input);
		args = get_args(input);
		if (args == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		search_in_path(args);
		eval(args);
		free_input_args(input, args);
		input = readline("some shell>");
	}
	return (0);
}
