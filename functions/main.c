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
		ft_printf("sigint registered\n");
		g_signal.sigint = 1;
		if (g_signal.pid == 0) //if we are in a child process
		{
			//we have to stop the child process, kill it?
			//ft_printf("setting sigint to 1 inside CHILD\n");
			kill(g_signal.pid, SIGKILL);
			g_signal.sigint = 0;
		}
		if (g_signal.pid != 0) //if we are in the parent
		{
			//g_signal.sigquit = 0;
			//ft_printf("setting sigint to 1 inside parent\n");
			g_signal.pid = 1;
			g_signal.sigint = 1; //this sets the loop var to one, breaks start program loop
			//kill(g_signal.pid, SIGINT);
		}
		//ft_printf("\nsome shell>");
	}
	
}
*/

void	sigquit_handler(int this_signal)
{
	if (this_signal == SIGINT) //crtl c lett ez?
	{
	
		g_signal.sigint = 1;
		//ft_printf("%d is sigint now\n", g_signal.sigint);
		//close stdin etc?
			//close(STDOUT_FILENO);
		//close(STDERR_FILENO);


		/*
		if (g_signal.pid != 0)
		{
			//g_signal.sigquit = 0;
			ft_printf("entered here pid %d\n", g_signal.pid);
			g_signal.sigquit = 1;
			exit(0); //if its the parent we have to exit it
			//kill(getpid(), SIGKILL);
		}
		else //else its now we are in the child process, quit that while waiting 
		{
			//g_signal.sigquit = 0;
			kill(g_signal.pid, SIGKILL);
			//exit(0);
		}
		*/
		//exit(0); //just set the status, and maybe check at the parent?
		//we only need to exit the forked pid not the entire shell
	}
	if (this_signal == SIGQUIT)
	{
		ft_printf("sigveof CAUGHT\n");
	}
	//exit(0);
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

/*
// Restore terminal to original settings
static void terminal_done(void)
{
    if (g_signal.terminal_descriptor != -1)
        tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.terminal_original);
}
 
static void terminal_signal(int signum)
{
	ft_printf("%d is signum, received\n", signum);

	if (signum == SIGINT)
	{
		ft_printf("we also sent sigint here\n");
		if (g_signal.terminal_descriptor != -1)
			tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.terminal_original);
		ft_printf("gonna exit in terminal signal w 128 + %\n", signum);
		g_signal.sigint = 1;
		return ;
		//_exit(128 + signum);
	}	
}

void termios_init(t_minishell *minishell)
{
	//struct termios terminal_original, terminal_settings;
	struct sigaction act;

	if (isatty(STDERR_FILENO))
		g_signal.terminal_descriptor = STDERR_FILENO;
	if (isatty(STDIN_FILENO))
		g_signal.terminal_descriptor = STDIN_FILENO;
	if (isatty(STDOUT_FILENO))
		g_signal.terminal_descriptor = STDOUT_FILENO;
	else
        ft_printf("Error while initializing terminal\n");

	if (tcgetattr(g_signal.terminal_descriptor, &g_signal.terminal_original) ||
		tcgetattr(g_signal.terminal_descriptor, &g_signal.terminal_settings))
	{
		ft_printf("Error while initializing terminal2\n");
	}
	if (atexit(terminal_done))
		ft_printf("Error while calling atexit\n");
	sigemptyset(&act.sa_mask);
	act.sa_handler = terminal_signal;
	if (sigaction(SIGHUP,  &act, NULL) ||
        sigaction(SIGINT,  &act, NULL) ||
        sigaction(SIGQUIT, &act, NULL) ||
        sigaction(SIGTERM, &act, NULL) ||
#ifdef SIGXCPU
        sigaction(SIGXCPU, &act, NULL) ||
#endif
#ifdef SIGXFSZ    
        sigaction(SIGXFSZ, &act, NULL) ||
#endif
#ifdef SIGIO
        sigaction(SIGIO,   &act, NULL) ||
#endif
        sigaction(SIGPIPE, &act, NULL) ||
        sigaction(SIGALRM, &act, NULL))

 // Let BREAK cause a SIGINT in input. /
    g_signal.terminal_settings.c_iflag &= ~IGNBRK;
    g_signal.terminal_settings.c_iflag |=  BRKINT;
 
    // Ignore framing and parity errors in input. /
    g_signal.terminal_settings.c_iflag |=  IGNPAR;
    g_signal.terminal_settings.c_iflag &= ~PARMRK;
 
    // Do not strip eighth bit on input. /
    g_signal.terminal_settings.c_iflag &= ~ISTRIP;
 
    // Do not do newline translation on input. /
    g_signal.terminal_settings.c_iflag &= ~(INLCR | IGNCR | ICRNL);
 
#ifdef IUCLC
    // Do not do uppercase-to-lowercase mapping on input. /
    terminal_settings.c_iflag &= ~IUCLC;
#endif
 
    // Use 8-bit characters. This too may affect standard streams,
     // but any sane C library can deal with 8-bit characters. /
   	g_signal.terminal_settings.c_cflag &= ~CSIZE;
    g_signal.terminal_settings.c_cflag |=  CS8;
    // Enable receiver. /
    g_signal.terminal_settings.c_cflag |=  CREAD;
    // Let INTR/QUIT/SUSP/DSUSP generate the corresponding signals. /
    g_signal.terminal_settings.c_lflag |=  ISIG;
    // Enable noncanonical mode.
    // This is the most important bit, as it disables line buffering etc. /
    g_signal.terminal_settings.c_lflag &= ~ICANON;
    // Disable echoing input characters. /
    g_signal.terminal_settings.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    // Disable implementation-defined input processing. /
    g_signal.terminal_settings.c_lflag &= ~IEXTEN;
    // To maintain best compatibility with normal behaviour of terminals,
     // we set TIME=0 and MAX=1 in noncanonical mode. This means that
     // read() will block until at least one byte is available. /
    g_signal.terminal_settings.c_cc[VTIME] = 0;
    g_signal.terminal_settings.c_cc[VMIN] = 1;
 	g_signal.terminal_settings.c_cc[VINTR] = 3; //C, sends SIGINT SIGNAL
	g_signal.terminal_settings.c_cc[VEOF] = 4;//_POSIX_VDISABLE;//4; //D
    // Set the new terminal settings.
     // Note that we don't actually check which ones were successfully
     // set and which not, because there isn't much we can do about it. /
    tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.terminal_settings);

}

*/
//https://cboard.cprogramming.com/linux-programming/158476-termios-examples.html
//https://stackoverflow.com/questions/68602211/forcing-a-terminal-not-to-print-ctrl-hotkeys-when-signals-are-caughts
//https://www.mkssoftware.com/docs/man5/struct_termios.5.asp
//https://www.gnu.org/software/libc/manual/html_node/Local-Modes.html
//https://viewsourcecode.org/snaptoken/kilo/04.aTextViewer.html
//https://blog.nelhage.com/2009/12/a-brief-introduction-to-termios/s
//https://www.cons.org/cracauer/sigint.html
//https://www.gnu.org/software/libc/manual/html_node/Canonical-or-Not.html
//https://github.com/scocoyash/Text-Editor-In-C/blob/master/stex.cs
//https://en.wikibooks.org/wiki/Serial_Programming/termios
//https://stackoverflow.com/users/14701326/mampac?tab=profile
//https://www.youtube.com/watch?v=RU0ULe2f6hI


int	main(void)
{ 
	t_minishell minishell;
	
	init_signal();
	//termios_init(&minishell);
	
	struct termios old_termios, new_termios;

	//tcgetattr(0, &old_termios); //THIS DOES SMTH INTERESTING
	//????


	//(void)signal(SIGQUIT, SIG_IGN);
	(void)signal(SIGINT, sigquit_handler);
	(void)signal(SIGINT, sigquit_handler);

	new_termios.c_lflag |= ICANON; //Talking of pipe here is misleading. CTRL-D is only relevant for terminal devices, not pipes, and it's only relevant on the master side of the pseudo-terminal or when sent by the (real) terminal, and only when in icanon mode.
	new_termios = old_termios;
	new_termios.c_lflag |= ISIG; //If ISIG is set each input character is checked against the special control character INTR and QUIT. If an input character matches one of these control character the function associated with that character is performed. If ISIG is not set, no checking is done. Thus these special functions are possible only if ISIG is set.
	
	new_termios.c_cc[VINTR] = 4; //C, sends SIGINT SIGNAL
	new_termios.c_cc[VEOF] = 3;//_POSIX_VDISABLE;//4; //D
	new_termios.c_cc[VQUIT] = 34; // crtl backslashs
	//new_termios.c_lflag &= ~ECHOCTL; //https://stackoverflow.com/questions/608916/ignoring-ctrl-c
	//new_termios.c_lflag |= ECHO; //this alone would disable echoing c
	new_termios.c_lflag |= ECHOK;
	new_termios.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &new_termios);

	//int c;
    //termios_init(&minishell);
	/*
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
	*/
	while (g_signal.sigquit != 1)
	{
		init(&minishell);
		while (g_signal.sigint != 1 && g_signal.sigquit != 1)
		{
			start_program_loop(&minishell);
		}
		if (g_signal.sigquit == 1)
		{
			ft_printf("\b\b");
			exit(0);
		}
		if (g_signal.sigint == 1)
		{
			g_signal.sigint = 0;
		}
	}
	//tcsetattr(0,TCSANOW,&old_termios);	
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