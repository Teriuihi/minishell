/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_program_utils.h                               :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "run_program_utils.h"
/**
 * Copies cur_pid to old_pid
 *
 * @param	cur_pid	Current PIDs
 * @param	old_pid	Old PIDs
 */
void	copy_pid(const int *cur_pid, int *old_pid)
{
	old_pid[0] = cur_pid[0];
	old_pid[1] = cur_pid[1];
}

void	init_run_commands(t_list **entry, t_list **head, int *cur_pid,
			int *old_pid)
{
	*entry = *head;
	cur_pid[0] = -1;
	old_pid[0] = -1;
	cur_pid[1] = -1;
	old_pid[1] = -1;
}

/**
 * Checks if the input should be used
 * 	if it's only white space it shouldn't
 *
 * @param	input	Input to check
 *
 * @return	true if the input should be used, false if not
 */
t_bool	should_use(char *input)
{
	if (input == NULL)
		return (false);
	while (*input && ft_iswhite_space(*input))
		input++;
	return ((*input) != '\0');
}

char	*prompt(void)
{
	char	*input;

	if (g_signal.sigquit == 1 && g_signal.exit_status != 0)
	{
		if (g_signal.exit_status == 131)
			ft_printf(1, "Quit: 3\n");
		g_signal.sigquit = 0;
	}
	input = readline("some shell>");
	return (input);
}
