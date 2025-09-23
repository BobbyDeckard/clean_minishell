/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_sigs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:00:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:25:35 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	sigint_heredoc_handler(int sig)
{
	(void) sig;
	g_signal_received = SIGINT;
	close(STDIN_FILENO);
}

void	init_sp_handler_sig(t_ast *node, struct sigaction *new,
struct sigaction *old)
{
	new->sa_handler = sigint_heredoc_handler;
	sigemptyset(&(new->sa_mask));
	new->sa_flags = 0;
	node->data->state = HEREDOC_MODE;
	sigaction(SIGINT, new, old);
}

void	heredoc_end(t_ast *node, struct sigaction *new, struct sigaction *old,
int stdin_bu)
{
	sigaction(SIGINT, old, NULL);
	if (stdin_bu >= 0)
		close(stdin_bu);
	node->data->state = INTERACTIVE;
	g_signal_received = 0;
}
