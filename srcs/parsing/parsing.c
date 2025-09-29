/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:54:10 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 14:46:34 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"
#include <unistd.h>

static t_ast	*invalid_syntax(t_shell *data, t_token **tokens, int err)
{
	ft_putstr_fd("minishell: syntax error in command (", STDERR_FILENO);
	if (err == 1)
		ft_putstr_fd("invalid quotes)\n", STDERR_FILENO);
	else if (err == 2)
		ft_putstr_fd("invlalid operator)\n", STDERR_FILENO);
	else if (err == 3)
		ft_putstr_fd("invalid redirection)\n", STDERR_FILENO);
	else if (err == 4)
		ft_putstr_fd("invalid parentheses)\n", STDERR_FILENO);
	free_tokens(tokens);
	return (NULL);
}

t_ast	*parse(char *command, t_shell *data)
{
	t_token	**token_list;
	t_ast	*ast;
	int		err;

	token_list = tokenize_command(data, command);
	if (!token_list)
		return (NULL);
	// As is, an error in tokenize_command() would give the prompt back.
	// Check for error messages and eventual exit() calls to be made.
	err = valid_syntax(data, token_list);
	if (err)
		return (invalid_syntax(data, token_list, err));
	data->tokens = token_list;
	ast = create_ast(token_list, data);
	free_tokens(token_list);
	return (ast);
}
