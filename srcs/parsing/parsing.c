/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:54:10 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 14:33:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

t_ast	*parse(char *command, t_shell *data)
{
	t_token	**token_list;
	t_ast	*ast;

	token_list = tokenize_command(data, command);
	if (!token_list)
		return (NULL);
	// As is, an error in tokenize_command() would give the prompt back.
	// Check for error messages and eventual exit() calls to be made.
	if (!valid_syntax(data, token_list))
	{
		free_tokens(token_list);
		ft_putstr_fd("minishell: syntax error in command\n", STDERR_FILENO);
		return (NULL);
	}
	data->tokens = token_list;
	print_token_list(token_list);
	ast = create_ast(token_list, data);
	if (!ast)
		ft_putstr_fd("Failed to create AST\n", STDERR_FILENO);
	free_tokens(token_list);
	return (ast);
}
