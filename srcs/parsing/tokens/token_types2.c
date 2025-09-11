/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:24:33 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 21:51:13 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*handle_token_type(t_shell *data, char	**command, t_token_type type,
t_token *token)
{
	if (type == WORD)
		token = tokenize_word(data, data->tokens, command, token);
	else if (type == QUOTE)
		token = tokenize_quote(data, data->tokens, command, token);
	else if (type == REDIR)
		token = tokenize_redir(data, data->tokens, command, token);
	else if (type == OPERATOR)
		token = tokenize_operator(data, data->tokens, command, token);
	else if (type == PARENTHESIS)
		token = tokenize_parenthesis(data, data->tokens, command, token);
	else if (type == SPECIAL_CHARACTER)
		token = tokenize_special_character(data, data->tokens, command, token);
	else if (type == SPACE)
		token = tokenize_space(command, token);
	else if (type == ENV_VAR)
		token = tokenize_env_var(data, data->tokens, command, token);
	return (token);
}

int	is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

int	is_special_character(char c)
{
	if (c == '\\' || c == '#' || c == '\n' || c == '\0')
		return (1);
	return (0);
}

int	is_env_var(char c)
{
	if (c == '$')
		return (1);
	return (0);
}
