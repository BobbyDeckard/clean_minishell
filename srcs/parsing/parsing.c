/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:54:10 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 12:19:40 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"
#include <unistd.h>

int	valid_syntax(t_shell *data, t_token **token_list);

t_ast	*parse_command(char *command, t_shell *data)
{
	t_token	**token_list;
	t_ast	*ast;

	if (!command)
		return (NULL);
	token_list = tokenize_command(data, command);
	if (!token_list)
		return (NULL);	// check if tokenization errors should exit completely or not
	if (!valid_syntax(data, token_list))
	{
		free_tokens(token_list);
		ft_putstr_fd(" syntax error in command\n", STDERR_FILENO);
		return (NULL);
	}
	data->tokens = token_list;
	ast = create_ast(token_list, data);
	if (!ast)
	{
		free_tokens(token_list);
		ft_putstr_fd("Failed to create AST\n", STDERR_FILENO);
		return (NULL);
	}
	free_tokens(token_list);
	return (ast);
}
