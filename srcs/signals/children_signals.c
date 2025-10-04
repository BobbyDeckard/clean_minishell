/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:41:31 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/03 18:32:41 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	setup_child_signals(t_ast *node)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("child sigaction SIGINT");
		cleanup(node);
		exit(1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("child sigaction SIGQUIT");
		cleanup(node);
		exit(1);
	}
}
