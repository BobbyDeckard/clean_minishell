/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:03:10 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 15:27:46 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	link_token(t_token *token, t_token **token_list)
{
	t_token	*current;

	if (!*token_list)
	{
		*token_list = token;
		return ;
	}
	current = *token_list;
	while (current->next)
		current = current->next;
	current->next = token;
	token->previous = current;
}

static t_token	*create_token(t_shell *data, t_token **tl)
{
	t_token	*new;

	new = (t_token *) malloc(sizeof(t_token));
	if (!new)
		malloc_error(NULL, data, tl);
	new->type = 0;
	new->content = NULL;
	new->needs_expansion = 0;
	new->in_double_quotes = 0;
	new->next = NULL;
	new->previous = NULL;
	return (new);
}

static t_token	**extract_token(char **command, t_token_type type,
t_shell *data, t_token **token_list)
{
	t_token	*new_token;

	new_token = create_token(data, token_list);
	new_token = handle_token_type(data, command, type, new_token);
	if (new_token)
		link_token(new_token, token_list);
	return (token_list);
}

static t_token_type	get_token_type(char c)
{
	if (is_quote(c))
		return (QUOTE);
	else if (is_redirection(c))
		return (REDIR);
	else if (is_operator(c))
		return (OPERATOR);
	else if (is_parenthesis(c))
		return (PARENTHESIS);
//	else if (is_special_character(c))
//		return (SPECIAL_CHARACTER);
	else if (is_space(c))
		return (SPACE);
	else if (is_command_char(c))
		return (WORD);
	return (DEFAULT);
}

t_token	**tokenize_command(t_shell *data, char *command)
{
	t_token_type	type;
	t_token			**token_list;

	token_list = (t_token **) malloc(sizeof(t_token *));
	if (!token_list)
		malloc_error(NULL, data, NULL);
	*token_list = NULL;
	while (*command)
	{
		type = get_token_type(*command);
		if (type == DEFAULT)
		{
			command++;
			continue ;
		}
		token_list = extract_token(&command, type, data, token_list);
	}
	return (token_list);
}
