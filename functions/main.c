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
#include <readline/history.h>
#include <sys/stat.h>
#include <unistd.h>

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

void	copy_pid(const int *cur_pid, int *old_pid)
{
	old_pid[0] = cur_pid[0];
	old_pid[1] = cur_pid[1];
}

void	run_commands(t_list **head, t_data *data)
{
	int			cur_pid[2];
	int			old_pid[2];
	t_command	*command;
	t_list		*entry;

	entry = *head;
	cur_pid[0] = 0;
	while (entry)
	{
		command = entry->content;
		if (cur_pid[0])
			copy_pid(cur_pid, old_pid);
		if (command->type)
			pipe(cur_pid);
		exec_command(command, old_pid, cur_pid,
			is_builtin(command, data), data);
		entry = entry->next;
	}
}

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

int	main(void)
{
	t_signal	*signal_struct;
	char		*input;
	char		**args;
	t_list		**head;
	t_data		data;
	char		*cur_dir;

	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		ft_printf("Error\n");
		return (0);
	}
	set_pwd(cur_dir); //doens't need free
	set_data(&data); //assigns hashtables
	signal_struct = init_signal();
	signal(SIGQUIT, sigquit_handler);
	input = readline("some shell>");
	while (input)
	{
		add_history(input);
		args = get_args(input); //
		if (args == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		head = find_commands(args);
		if (head == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		run_commands(head, &data);
		free_input_args(input, args);
		input = readline("some shell>");
	}
	return (0);
}
