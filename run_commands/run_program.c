/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_program.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/14 18:54:39 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/14 18:54:39 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include <readline/history.h>
#include <readline/readline.h>
#include "run_commands.h"
#include "../create_commands/create_commands.h"
#include "../buildins/buildins.h"
#include <errno.h>
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

t_cmd_data	*init_cmd()
{
	t_cmd_data *cmd_data;

	//cmd_data->command->command needs malloc?
	//cmd_data->command->args needs malloc?
	cmd_data = (t_cmd_data *)malloc(sizeof(t_cmd_data));
	if (!cmd_data)
		return (NULL);
	cmd_data->command = (t_command *)malloc(sizeof(t_command));
	if (!cmd_data->command)
		return (NULL);
	cmd_data->command = NULL;
	//cmd_data->input = malloc(sizeof(t_redirect));
	//if (!cmd_data->input)
	//	return (NULL);
	//cmd_data->output = malloc(sizeof(t_redirect));
	//if (!cmd_data->output)
	//	return (NULL);
	cmd_data->input.type = NONE; // did we actually set space for these?
	cmd_data->output.type = NONE;
	return (cmd_data);
}

/**
 * Run all commands in given list
 *
 * @param	head		Start of list of commands
 * @param	minishell	Data for minishell
 */
void	run_commands(t_list **head, t_minishell *minishell)
{
	int			cur_pid[2];
	int			old_pid[2];
	t_cmd_data	*cmd_data;
	t_list		*entry;

	cmd_data = init_cmd();
	entry = *head;
	cur_pid[0] = -1;
	old_pid[0] = -1;
	cur_pid[1] = -1;
	old_pid[1] = -1;
	while (entry)
	{
		cmd_data = (t_cmd_data *)entry->content;
		if (cur_pid[0])
		{
			copy_pid(cur_pid, old_pid);
		}
		if (cmd_data->output.type) //non initialized yet, if it has to put smth out?
		{
			pipe(cur_pid); //now this gets two new fds
		}
		exec_command(cmd_data, old_pid, cur_pid,
			is_builtin(cmd_data), minishell);
		entry = entry->next;
	}
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
	{
		return (false);
	}
	while (*input && ft_iswhite_space(*input))
		input++;
	return ((*input) != '\0');
}


static int interruptible_getc(void)
{	
	int		r;
	char	c;

	if (g_signal.interrupted == true)
	{
		return EOF;
	}
	r = read(0, &c, 1); // read from stdin, will return -1 when interrupted by a signal
	if (r == -1 && errno == EINTR) //then this is sigint (crtl c here)
	{
		if (errno == EINTR && g_signal.sigint != 1) //we have to check which signal was which it interrupted
		{
			g_signal.interrupted = true;
		}
	}
	return (r == 1 ? c : EOF); //if we use signals this we always return EOF
}

#include <dirent.h>

void    search_folder(char *command)
{
    int             entries;
    char            *tmp;
    char            *buf;

    struct dirent   *file;
    DIR             *directory;
    char            **split_path;
    struct stat     sb;

    entries = 0;
    directory = NULL;
	tmp = NULL;
    tmp = getcwd(tmp, 1000);
    if (tmp == NULL)
    {
        ft_printf("couldnt use getcwd\n");
    }
    directory = opendir(tmp);
    while ((file = readdir(directory)))
    {
        if (file->d_name[0] == '.')
            continue ;
        entries++;
        char buffer[1024];
        tmp = ft_strjoin(ft_strjoin(tmp, "/."), file->d_name);
        ft_printf("%s is joined path\n", tmp);
        if (stat(command, &sb) == -1)
        {
            ft_printf("STAT IS ERROR\n");
        }
        ft_printf("%d is entries\n", entries);
        if (S_ISDIR(sb.st_mode))
            ft_printf("DIR\n");
        else
            ft_printf("FILE\n");
        ft_printf("%20s is filename\n", file->d_name);
    }
}



/**
 * Starts (minishell) program loop,
 * 	reads from command line to receive commands
 *
 * @param	minishell	Data for minishell
 */
void	start_program_loop(t_minishell *minishell)
{
	char		*input;
	char		**args;
	t_list		**head;

	rl_getc_function = interruptible_getc;
	input = "hi";
	args = NULL;
	search_folder(NULL);
	while (input && g_signal.sigint != 1 && g_signal.veof != 1)
	{
		
		input = readline("some shell>");
		signal_check(input);
		if (should_use(input) == true)
		{
			add_history(input);
			args = get_args(input); //TODO free
			free(input);
			input = NULL;
			head = find_commands(args); //TODO free
			if (head == NULL) //lets say that this is inside a child process, cant just exit the entire programme
			{
				signal_check(NULL);
				return ;
			}
			run_commands(head, minishell);
			free_commands(head);
			free_char_arr(args);
		}
	}
	if (input != NULL)
	{
		free(input);
	}
}

//was above the input != NULL
//if (g_signal.veof != 1) //not sure about this yet
	//{
	//	ft_printf("\n");
	//}



/*
if (g_signal.sigint != 1)
{
	//ft_printf("setting sigquit to 1\n"); //delete the prev char apostrophe D
	//int stdout_copy = dup(STDOUT_FILENO);
	//close(STDOUT_FILENO);
	//dup2(stdout_copy, STDOUT_FILENO);
	//close(stdout_copy);
	g_signal.sigquit = 1;
}
*/