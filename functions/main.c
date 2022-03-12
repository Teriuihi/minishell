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
#include "../buildins/buildins.h"
#include "../hashtable/hashtable.h"
#include "../headers/minishell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h> 

volatile sig_atomic_t keep_running = 1;

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

void	set_data(t_minishell *minishell)
{
	minishell->current_env = get_hash_table();
    minishell->env = get_hash_table();
	set_to_exported(minishell->current_env);
	set_to_exported(minishell->env);
    //TODO check failure
	//set is_exported in env on to all, in current env as well
}

/*
void	init_signal_struct()
{
	global_signal.sigint = 0;
	global_signal.sigquit = 0;
	global_signal.pid = 0;
	global_signal.exit_status = 0;
}
*/
void	sig_hnd(int sig)
{
	(void)sig;
	ft_printf("CRTL C\n");
	//exit(0);
}

void	sigint_handler(int this_signal)
{
	if (this_signal == SIGINT)
	{
		//global signal stuff to 0
		keep_running = 0;
		ft_printf("global signal received andchanged to 1\n");
	}
	ft_printf("exit\n");
	//exit(0);
}

void	init(t_minishell *minishell)
{
	char		*cur_dir;
	//t_signal	*signal_struct;

	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		ft_printf("Error\n");
		exit(0);
	}
	minishell->cur_wd = cur_dir;
	minishell->exit_status = 0;
	set_data(minishell); //assigns hashtables
	set_pwd(ft_strdup(cur_dir), minishell); //TODO check for failure
	
	//signal_struct = init_signal(); //TODO check for failure (NULL)
	//signal(SIGQUIT, sigquit_handler);
}

int	main(void)
{
	t_minishell	minishell;

	signal(SIGINT, sigint_handler);
	//ssignal(SIGQUIT, sigint_handler);
	init(&minishell);
	while (keep_running == 1)
	{
		//init stuff, keep running
	}
	start_program_loop(&minishell);
	return (0);
}


/*
	//struct termios old_termios, new_termios;
	tcgetattr(0, &old_termios);
	signal(SIGINT, sig_hnd); //crtl c
	signal(SIGQUIT, sig_quit); 
	new_termios = old_termios;
	new_termios.c_cc[VEOF] = 4; //C
	new_termios.c_cc[VINTR] = 3; //D
	tcsetattr(0, TCSANOW, &new_termios);
	char line[256]; int len;
  do{
    len=read(0,line,256); line[len]='\0';
    if( len <0 ) ft_printf("(len: %i)",len);
    if( len==0 ) ft_printf("(VEOF)");
    if( len >0 ){
      if( line[len-1] == 10 ) ft_printf("(line:'%.*s')\n",len-1,line);
      if( line[len-1] != 10 ) ft_printf("(partial line:'%s')",line);
    }
  }while( line[0] != 'q' );
  	tcsetattr(0,TCSANOW,&old_termios);	
	*/