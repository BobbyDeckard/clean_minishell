/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:24:33 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/30 10:28:09 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*handle_token_type(t_shell *data, t_token **tl, char	**command, t_token_type type, t_token *new_token)
{
	if (type == WORD)
		new_token = tokenize_word(data, tl, command, new_token);
	else if (type == QUOTE)
		new_token = tokenize_quote(data, tl, command, new_token);
	else if (type == REDIR)
		new_token = tokenize_redir(data, tl, command, new_token);
	else if (type == OPERATOR)
		new_token = tokenize_operator(data, tl, command, new_token);
	else if (type == PARENTHESIS)
		new_token = tokenize_parenthesis(data, tl, command, new_token);
	else if (type == SPECIAL_CHARACTER)
		new_token = tokenize_special_character(data, tl, command, new_token);
	return (new_token);
}

int	is_special_character(char c)
{
	// removed c == '$', should only be flagged for env_var ?
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
