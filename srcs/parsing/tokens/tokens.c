/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:03:10 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/28 11:20:27 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_token_type	get_token_type(char c)
{
	if (is_command_char(c))
		return (WORD);
	else if (is_quote(c))
		return (QUOTE);
	else if (is_redirection(c))
		return (REDIR);
	else if (is_logical_operator(c))
		return (OPERATOR);
	else if (is_parenthesis(c))
		return (PARENTHESIS);
	else if (is_special_character(c))
		return (SPECIAL_CHARACTER);
	else if (is_env_var(c))
		return (ENV_VAR);
	return (DEFAULT);
}

t_token	**tokenize_command(t_shell *data, char *command)
{
	t_token_type	type;
	t_token			**token_list;
	t_token			**result;

	token_list = (t_token **) malloc(sizeof(t_token *));
	if (!token_list)
		return (NULL);
	*token_list = NULL;
	while (*command)
	{
		type = get_token_type(*command);
		printf("tokenize_command: type for %s is %d\n", command, type);
		if (type == DEFAULT)
		{
			command++;
			continue ;
		}
		result = extract_token(&command, type, token_list);
		if (!result)
			return (free(token_list), NULL);	// wtf kind of error handling is that
		token_list = result;
	}
	mark_tokens_for_expansion(token_list, data->envp);
	return (token_list);
}
