/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:43:17 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/14 10:53:34 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void retokenize_word(t_shell *shell, t_token **list, t_token *token,
char *content)
{
	int	len;

	token->type = WORD;
	len = ft_strlen(content) + 1;
	token->content = (char *) malloc(len * sizeof(char));
	if (!token->content)
		malloc_error(NULL, shell, list);
	ft_strlcpy(token->content, content, len);
}

static void	uninterpret_token(t_shell *shell, t_token **list, t_token *token)
{
	if (token->type == WHITESPACE)
		token->type = WORD;
	else if (token->type == SINGLE_QUOTE)
		retokenize_word(shell, list, token, "'");
	else if (token->type == DOUBLE_QUOTE)
		retokenize_word(shell, list, token, "\"");
	else if (token->type == PAREN_CLOSE)
		retokenize_word(shell, list, token, ")");
	else if (token->type == PAREN_OPEN)
		retokenize_word(shell, list, token, "(");
	else if (token->type == PIPE)
		retokenize_word(shell, list, token, "|");
	else if (token->type == OR)
		retokenize_word(shell, list, token, "||");
	else if (token->type == AND)
		retokenize_word(shell, list, token, "&&");
	else if (token->type == REDIR_IN)
		retokenize_word(shell, list, token, "<");
	else if (token->type == REDIR_OUT)
		retokenize_word(shell, list, token, ">");
	else if (token->type == REDIR_APPEND)
		retokenize_word(shell, list, token, ">>");
	else if (token->type == HEREDOC)
		retokenize_word(shell, list, token, "<<");
}

int	check_quotes(t_shell *shell, t_token **list)
{
	t_token	*current;
	int		in_double;
	int		in_single;

	in_double = 0;
	in_single = 0;
	current = *list;
	while (current)
	{
		if (current->type == DOUBLE_QUOTE && !in_single)
			in_double = !in_double;
		else if (current->type == SINGLE_QUOTE && !in_double)
			in_single = !in_single;
		else if (in_single || in_double)
			uninterpret_token(shell, list, current);
		current = current->next;
	}
	if (in_single || in_double)
		return (1);
	return (0);
}
