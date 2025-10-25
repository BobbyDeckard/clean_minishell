/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:36:51 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 16:01:20 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	sigint_heredoc_handler(int sig)
{
	(void) sig;
	g_signal_received = SIGINT;
	printf("About to close stdin\n");
	if (close(STDIN_FILENO))
		perror("close");
	printf("Closed stdin\n");
}

void	init_sp_handler_sig(t_ast *node, struct sigaction *new,
struct sigaction *old)
{
	new->sa_handler = sigint_heredoc_handler;
	sigemptyset(&(new->sa_mask));
	new->sa_flags = 0;
	node->shell->state = HEREDOC_MODE;
	sigaction(SIGINT, new, old);
}

void	heredoc_end(t_ast *node, struct sigaction *old,
int stdin_bu)
{
	sigaction(SIGINT, old, NULL);
	if (stdin_bu >= 0)
	{
		printf("About to close stdin backup\n");
		if (close(stdin_bu))
			perror("close");
		printf("Closed stdin backup\n");
	}
	node->shell->state = INTERACTIVE;
	g_signal_received = 0;
}
