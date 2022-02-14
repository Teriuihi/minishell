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
#include <sys/stat.h>
#include <unistd.h>

t_hash_table	*get_hash_table(void)
{
	static t_hash_table	*table;

	if (!table)
	{
		table = duplicates_are_found_in_argv();
		if (table == NULL)
		{
			ft_printf("Error\n");
			exit (0);
		}
	}
	return (table);
}

void	set_data(t_data *data)
{
	data->export_flag = 0;
	data->current_env = get_hash_table();
	data->env = get_hash_table();
}

void	init(t_minishell *minishell)
{
	t_data		data;
	char		*cur_dir;
	t_signal	*signal_struct;

	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		ft_printf("Error\n");
		exit(0);
	}
	minishell->cur_wd = cur_dir;
	set_data(&data); //assigns hashtables
	minishell->data = &data;
	set_pwd(cur_dir, minishell);
	signal_struct = init_signal();
	signal(SIGQUIT, sigquit_handler);
}

int	main(void)
{
	t_minishell	minishell;

	init(&minishell);
	start_program_loop(&minishell);
	return (0);
}
