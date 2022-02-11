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

void	eval(t_command_data *command_data, t_data *data)
{
	int			*pid;
	int			*old_pid;
	t_command	*command;
	t_list		*entry;

	entry = *command_data->commands;
	old_pid = NULL;
	pid = NULL;
	while (entry)
	{
		command = entry->content;
		if (pid)
			old_pid = pid;
		if (command->type)
		{
			pid = ft_calloc(2, sizeof(int));
			if (!pid)
				return ;
			pipe(pid);
			exec_command(command, old_pid, pid, is_builtin(command, data), data);
		}
		else
			exec_command(command, old_pid, pid, is_builtin(command, data), data);
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
	t_signal		*signal_struct;
	char			*input;
	char			**args;
	t_command_data	*command_data;
	t_data			data;
	char			*cur_dir;

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
		command_data = find_commands(args);
		if (command_data == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		eval(command_data, &data);
		free_input_args(input, args);
		input = readline("some shell>");
	}
	return (0);
}
