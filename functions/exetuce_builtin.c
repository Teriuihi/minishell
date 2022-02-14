/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_builtin.c                                        :+:    :+:      */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"

/**
 * Takes an array of arrays (user input)
 * Selects the function to execute based on the first argument (args[0])
 * @param	args	the input given by the user
 *
 * @return	void
 */
static int	split_len(char **splitted)
{
	int	i;

	i = 0;
	if (!splitted)
	{
		return (0);
	}
	while (splitted[i])
	{
		i++;
	}
	return (i);
}

static t_bool	export_found(t_command *command, t_data *data)
{
	char	**splitted_export;
	int		i;

	i = 0;
	while (command->args[i])
	{
		if (ft_streq(command->args[i], "export")) //the next one should be the expression
		{
			//check later for error/edge cases? ls | cat | export    ->missing second part?
			splitted_export = ft_split(command->args[i + 1], '='); //what about more = eg: hello=there=johhny?
			if (split_len(splitted_export) < 2) //if its not min 2
			{
				free_splitted(splitted_export); //should free both not just ** itself
				return (false);
			}
			ft_set_env(splitted_export[0], splitted_export[1], data->env); //check if set fails for some reason?
			free_splitted(splitted_export); //also incorrect, should create a free split functio
			return (true);
		}
		i++;
	}
	return (false);
}

//for now we take it for granted that at this point we might have a correct input, but we have to find another way to check it
static t_bool	env_var_added(t_command *command, t_data *data) //cant we just assign a flag to the global struct which says, hey we have an equal sign found in is_built_in
{	
	//command "export myvar=hello"
	char	**splitted;

	if (!command || !data)
	{
		return (false);
	}
	if (export_found(command, data) == true)
	{
		return (true);
	}
	else
	{
		splitted = ft_split(command->command, '='); //what about more = eg: hello=there=johhny?
		if (split_len(splitted) < 2)
		{
			free_splitted(splitted);
			return (false);
		}
		ft_set_env(splitted[0], splitted[1], data->current_env); //check if set fails for some reason?
		return (true);
	}
}

t_bool	execute_builtin(t_command *command, t_data *data) //command->args + 1 == myvar=stmh
{
	char	*cur_dir;

	//check if command is an env var, in that case call a setter etc function
	//check for env var
	if (env_var_added(command, data) == true)
		return (true);
	cur_dir = get_pwd();
	if (!command->command)
		return (false);
	else if (ft_streq(command->command, "echo"))
		ft_echo(command, 1);
	else if (ft_streq(command->command, "cd"))
		cd(*command->args);
	else if (ft_streq(command->command, "pwd"))
	{
		ft_putstr_fd(cur_dir, 1);
		ft_putstr_fd("\n", 1);
	}
	else if (ft_streq(command->command, "env")) //next one should be the key
	{
		print_h_table(data->env);
	}
	else
		return (false);
	free(cur_dir);
	return (true);
}
