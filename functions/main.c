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

t_signal g_signal;

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
		//ft_printf("sigint registered\n");
		g_signal.sigint = 1;
		g_signal.exit_status = 1;
		ft_printf("\nsome shell>");
		//ft_printf("global signal received andchanged to 1\n");
	}
	
	//exit(0);
}


void	sigquit_handler(int this_signal)
{
	if (this_signal == SIGQUIT) //crtl c lett ez?
	{
		ft_printf("exit\n");
		g_signal.sigquit = 1;
		exit(0);
	}
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

/*
void	main_loop()
{
	
	
}
*/
int	main(void)
{
	t_minishell minishell;
	struct termios old_termios, new_termios;
	init_signal();
	//signal(SIGINT, sigint_handler);
	//signal(SIGQUIT, sigquit_handler);


	tcgetattr(0, &old_termios);
	signal(SIGINT, sigint_handler); //crtl c
	signal(SIGQUIT, sigquit_handler); 
	new_termios = old_termios;
	new_termios.c_cc[VEOF] = 3; //C
	new_termios.c_cc[VINTR] = 34; //D, lehet crtl backslash kene ide
	new_termios.c_cc[VQUIT] = 4; //ez a d jo
	tcsetattr(0, TCSANOW, &new_termios);
	//main loop
	while (g_signal.sigquit != 1)
	{
		//init stuff et
		
		//ssignal(SIGQUIT, sigint_handler);
		init(&minishell);
		while (g_signal.sigint != 1 && g_signal.sigquit != 1)
		{
			ft_printf("in small loop\n");
			start_program_loop(&minishell);
		}
		g_signal.sigint = 0;
		ft_printf("Resetting sigint, %d is sigquit\n", g_signal.sigquit);
	}
	ft_printf("outside after mainloop\n");
	exit(0);
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