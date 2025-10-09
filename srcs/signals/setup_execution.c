/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 13:43:14 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 13:43:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	signal_handler_execution_mute(int signum)
{
	g_signal_received = signum;
}

void	signal_handler_execution(int signum)
{
	g_signal_received = signum;
	if (signum == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
}

void	setup_execution_signals(char *command, t_shell *data)
{
	struct sigaction	sa;

	if (!mute_shlvl(data->envp))
		sa.sa_handler = signal_handler_execution;
	else
		sa.sa_handler = signal_handler_execution_mute;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction SIGINT");
		clean_data(data);
		free(command);
		exit(1);
	}
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
}
