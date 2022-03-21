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
#include <stdio.h>
#include <errno.h>

t_signal g_signal;

t_hash_table	*get_hash_table(void) //in the beginning all should be exported
{
	static t_hash_table	*table;

	if (!table)
	{
		table = create_env_h_table();
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
}

void	sigquit_handler(int this_signal)
{
	if (this_signal == SIGINT) //crtl c
	{
		g_signal.sigint = 1;
	}
}

void	set_termios()
{
	//which stream is connected to our device
	if (isatty(STDERR_FILENO))
		g_signal.terminal_descriptor = STDERR_FILENO;
	if (isatty(STDIN_FILENO))
		g_signal.terminal_descriptor = STDIN_FILENO;
	if (isatty(STDOUT_FILENO))
		g_signal.terminal_descriptor = STDOUT_FILENO;
	//assign original terminal settings
	if (tcgetattr(g_signal.terminal_descriptor, &g_signal.old_termios) ||
		tcgetattr(g_signal.terminal_descriptor, &g_signal.new_termios))
	{
		ft_printf("error\n");
	}
	//prob this is not even needed
	g_signal.new_termios.c_lflag |= (ICANON | ISIG | ECHO);  //Talking of pipe here is misleading. CTRL-D is only relevant for terminal devices, not pipes, and it's only relevant on the master side of the pseudo-terminal or when sent by the (real) terminal, and only when in icanon mode.
	g_signal.new_termios.c_cc[VINTR] = 3; //C, sends SIGINT SIGNAL
	g_signal.new_termios.c_cc[VEOF] = 4;//_POSIX_VDISABLE;//4; //D

	//in case we want to reset
	g_signal.old_termios.c_lflag |= (ICANON | ISIG | ECHO);  //Talking of pipe here is misleading. CTRL-D is only relevant for terminal devices, not pipes, and it's only relevant on the master side of the pseudo-terminal or when sent by the (real) terminal, and only when in icanon mode.
	g_signal.old_termios.c_cc[VINTR] = 3; //C, sends SIGINT SIGNAL
	g_signal.old_termios.c_cc[VEOF] = 4;//_POSIX_VDISABLE;//4; //D
	signal(SIGINT, sigquit_handler);

}


void	init(t_minishell *minishell)
{
	char		*cur_dir;

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
}

int	main(void)
{ 
	t_minishell		minishell;

	init(&minishell);
	init_signal();
	while (g_signal.veof != 1) //exit instead of sigquit, or find the main parent process
	{
		set_termios();
		while (g_signal.sigint != 1 && g_signal.veof != 1)
		{
			start_program_loop(&minishell);
		}
		if (g_signal.veof == 1)
		{
			ft_printf("\b\bexit\n"); //this prints twice if we call crtld in heredoc, why? 	//if it was cancelled in heredoc, this should not be printed
		}
		if (g_signal.sigint == 1)
		{
			g_signal.sigint = 0; //errno = 0; prob not needed
		}
	}
	return (0);
}
//g_signal.new_termios = g_signal.old_termios; // sort of resetting? , //tcsetattr(g_signal.terminal_descriptor, TCSANOW, &old_termios);	 //sets everything back to the original settings

/*
	//which stream is connected to our device
	if (isatty(STDERR_FILENO))
		g_signal.terminal_descriptor = STDERR_FILENO;
	if (isatty(STDIN_FILENO))
		g_signal.terminal_descriptor = STDIN_FILENO;
	if (isatty(STDOUT_FILENO))
		g_signal.terminal_descriptor = STDOUT_FILENO;
	//termios_init(&minishell);
	//assign original terminal settings
	if (tcgetattr(g_signal.terminal_descriptor, &old_termios) ||
		tcgetattr(g_signal.terminal_descriptor, &new_termios))
	{
		ft_printf("error\n");
	}
	new_termios.c_lflag |= ICANON; //Talking of pipe here is misleading. CTRL-D is only relevant for terminal devices, not pipes, and it's only relevant on the master side of the pseudo-terminal or when sent by the (real) terminal, and only when in icanon mode.
	new_termios.c_lflag |= ISIG; //If ISIG is set each input character is checked against the special control character INTR and QUIT. If an input character matches one of these control character the function associated with that character is performed. If ISIG is not set, no checking is done. Thus these special functions are possible only if ISIG is set.	
	new_termios.c_cc[VINTR] = 3; //C, sends SIGINT SIGNAL
	new_termios.c_cc[VEOF] = 4;//_POSIX_VDISABLE;//4; //D
	//new_termios.c_cc[VQUIT] = 34; // crtl backslashs
	//new_termios.c_lflag &= ~ECHOCTL; //https://stackoverflow.com/questions/608916/ignoring-ctrl-c
	new_termios.c_lflag |= ECHO;
	new_termios = old_termios;
	*/

/*
//should we set and reset std
		//close fds
		//reset fds
	//waitpid(-1, &status, 0)
	//status = WIFEXITSTATUS
	//ft_printf("%d is g_signal.pid in loop, %d is getpid\n", g_signal.pid, getpid());
*/
//if its the last parent process then we print n at the end, otherwise not
/*

	//int c;
    //termios_init(&minishell);
    printf("Press CTRL+C or Q to quit.\n");
    while ((c = getc(stdin)) != EOF && g_signal.keep_running != 0)
	{
		ft_printf("%d is c\n", c);
		if (c == 4)
		{
			g_signal.keep_running = 0;
		}
		{
			if (c >= 33 && c <= 126)
				ft_printf("0x%02x = 0%03o = %3d = '%c'\n", c, c, c, c);
			else
				ft_printf("0x%02x = 0%03o = %3d\n", c, c, c);
	
			if (c == 3 || c == 'Q' || c == 'q')
				break;
		}	
    }
     printf("Done.\n");

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