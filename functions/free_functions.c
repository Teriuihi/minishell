/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   fre_functions.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 14:13:16 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/04 14:13:16 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "free_functions.h"

void	free_splitted(char **splitted)
{
	int	i;

	i = 0;
	if (!splitted)
	{
		return ;
	}
	while (splitted[i])
	{
		free(splitted[i]);
		i++;
	}
	free(splitted);
}

/**
 * Free's a command
 *
 * @param	cmd_data	The command to free
 */
void	free_cmd(t_cmd_data *cmd_data)
{
	if (cmd_data->command != NULL)
	{
		free(cmd_data->command->command);
		while (cmd_data->command->args_len)
		{
			free(cmd_data->command->args[cmd_data->command->args_len - 1]);
			cmd_data->command->args_len--;
		}
		free(cmd_data->command->args);
		free(cmd_data->command);
	}
	free(cmd_data->input.file);
	free(cmd_data->output.file);
	free(cmd_data);
}

void	free_command(void *content)
{
	t_cmd_data	*cmd_data;

	if (!content)
		return ;
	cmd_data = (t_cmd_data *)content;
	free_cmd(cmd_data);
}

void	free_parse(void *content)
{
	t_arg	*arg;

	arg = content;
	free_string(arg->arg);
	free(arg);
}

void	free_parse_and_commands(t_list **commands, t_list **parse_results)
{
	if (commands != NULL)
		ft_lstclear(commands, free_command);
	free(commands);
	if (parse_results != NULL)
		ft_lstclear(parse_results, free_parse);
	free(parse_results);
}
