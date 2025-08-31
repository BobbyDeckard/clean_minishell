/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:35:22 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/30 09:06:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	getcwd_error(char **envp)
{
	perror("getcwd");
	free(envp);
	exit(1);
}

void	malloc_error(t_ast *node, t_shell *data, t_token **tl)
{
	perror("malloc");
	if (data)
		clean_data(*data);
	if (tl)
		free_tokens(tl);
	if (node)
		clean_exit(node->root, 1);
}
