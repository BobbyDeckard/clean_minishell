/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 18:42:38 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 10:24:30 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	signal_handler_execution(int signum)
{
//	printf("signal_handler_execution() called\n");
	g_signal_received = signum;
	if (signum == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
}

void	setup_execution_signals(char *command, t_shell *data)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler_execution;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction SIGINT");
		clean_data(data);
		free(command);
		exit(1);
	}
//	else
//		printf("sigaction SIGINT success in execution signals\n");
	sa.sa_handler = signal_handler_execution;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction SIGQUIT");
		clean_data(data);
		free(command);
		exit(1);
	}
//	else
//		printf("sigaction SIGQUIT SUCCESS in execution signals\n");
}

void	signal_handler_interactive(int signum)
{
//	printf("signal_handler_interactive() called\n");
//	if (data->shlvl > 1)
//		return ;
	g_signal_received = signum;
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	setup_interactive_signals(t_shell *data)
{
	struct sigaction	sa;

	data->state = INTERACTIVE;
	sa.sa_handler = signal_handler_interactive;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction SIGINT");
		clean_data(data);
		exit(1);
	}
//	else
//		printf("sigaction SIGINT success in interactive signals\n");
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction SIGQUIT");
		clean_data(data);
		exit(1);
	}
//	else
//		printf("sigaction SIGQUIT success in interactive signals\n");
}
