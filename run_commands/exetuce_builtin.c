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
#include "../buildins/buildins.h"
#include "../headers/minishell.h"

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

static t_bool	export_found(t_command *command, t_minishell *minishell)
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
			ft_set_env(splitted_export[0], splitted_export[1], minishell->env); //check if set fails for some reason?
			free_splitted(splitted_export); //also incorrect, should create a free split functio
			return (true);
		}
		i++;
	}
	return (false);
}

//for now we take it for granted that at this point we might have a correct input, but we have to find another way to check it
static t_bool	env_var_added(t_command *command, t_minishell *minishell) //cant we just assign a flag to the global struct which says, hey we have an equal sign found in is_built_in
{	
	//command "export myvar=hello"
	char	**splitted;

	if (!command || !minishell)
	{
		minishell->exit_status = 2;
		return (false);
	}
	if (export_found(command, minishell) == true)
	{	//in this case we should add an: is_exported flag to true
		minishell->exit_status = 0;
		return (true);
	}
	splitted = ft_split(command->command, '='); //what about more = eg: hello=there=johhny?
	if (split_len(splitted) != 2) //should be only 2
	{
		free_splitted(splitted);
		minishell->exit_status = 2;
		return (false);
	}
	//here we should check which one we have to add to
	//ft_printf("%s %s is splitted0, splitted1 before entering to current env set\n", splitted[0], splitted[1]);
	//print_h_table(minishell->current_env);
	//ft_printf("is current env table before\n");
	if (ft_set_env(splitted[0], splitted[1], minishell->current_env) == false) //check if set fails for some reason?
	{
		minishell->exit_status = 1;
		return (false);
	}
	else
	{
		//print_h_table(minishell->current_env);
		//ft_printf("is current env table after\n");
		minishell->exit_status = 0;
		return (true);
	}
}

t_bool	ft_env(t_hash_table *h_table, t_minishell *minishell)
{
	if (print_h_table(h_table) == false)
	{
		return (false);
	}
	else
	{
		return (true);
	}
}

//command not found : 127 exit status
t_bool	execute_non_forked_builtin(t_command *command, t_minishell *minishell) //cd and unset, env ?, we do not need to call a child process for these?
{	//cd, export, unset
	char	*cur_dir;
	t_bool	did_execution_succeed;

	if (!command->command || !minishell)
		return (false);
	did_execution_succeed = false;
	cur_dir = get_pwd(minishell);
	if (!cur_dir)
		did_execution_succeed = false;
	else if (env_var_added(command, minishell) == true) //export? but also a=b should be here
	{
		did_execution_succeed = true;
	}
	else if (ft_streq(command->command, "cd"))
	{
		did_execution_succeed = cd(command, minishell);
	}
	else if (ft_streq(command->command, "env"))
	{
		did_execution_succeed = ft_env(minishell->env, minishell);
	}
	else if (ft_streq(command->command, "unset")) //unset returns always 0? even if its not in env?
	{
		did_execution_succeed = ft_remove_exported_var(command->args[1], minishell->env, minishell);
	} 
	if (did_execution_succeed == true)
		minishell->exit_status = 0;
	else
		minishell->exit_status = 1;
	return (did_execution_succeed);
}

t_bool	ft_pwd(char *cur_dir, t_minishell *minishell)
{
	if (!cur_dir)
	{
		return (false);
	}
	ft_putstr_fd(cur_dir, 1);
	ft_putstr_fd("\n", 1);
	return (true);
}

t_bool	execute_builtin(t_command *command, t_minishell *minishell) //command->args + 1 == myvar=stmh
{
	char	*cur_dir;

	if (!command->command || !minishell)
		return (false);
	cur_dir = get_pwd(minishell);
	//if (env_var_added(command, minishell) == true)
	//	return (true);
	if (ft_streq(command->command, "echo")) //echo nemtommi > file.txt so it should be forked
		return (ft_echo(command, 1, minishell));
	else if (ft_streq(command->command, "pwd")) //pwd > teso.txt so it should be forked
		return (ft_pwd(cur_dir, minishell));
	else
		return (false);

}
