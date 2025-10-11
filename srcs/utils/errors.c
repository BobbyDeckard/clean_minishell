/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:08:17 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 14:21:23 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	clean_tokens(t_token **tokens);
void	clean_ast(t_ast *root);

void	malloc_error(t_ast *ast, t_shell *shell, t_token **tokens)
{
	perror("malloc");
	if (shell)
		clean_shell(shell);
	if (tokens)
		clean_tokens(tokens);
	if (ast)
		clean_ast(ast->root);
	exit(1);
}
