/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:23:57 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 14:39:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	make_changes(t_token *heredoc, t_token *quote1, t_token *quote2,
t_token *word)
{
	heredoc->type = HEREDOC_EXP;
	heredoc->next = word;
	word->previous = heredoc;
	word->next = quote2->next;
	free(quote1->content);
	free(quote1);
	free(quote2->content);
	free(quote2);
}

void	mark_heredoc_exp(t_token **tokens)
{
	t_token	*heredoc;
	t_token	*quote1;
	t_token	*quote2;
	t_token	*word;

	heredoc = *tokens;
	while (heredoc && heredoc->type != HEREDOC)
		heredoc = heredoc->next;
	if (!heredoc)
		return ;
	quote1 = heredoc->next;
	if (!quote1 || quote1->type != DOUBLE_QUOTE)
		return ;
	word = quote1->next;
	if (!word || !is_word(word->type))
		return ;
	quote2 = word->next;
	if (!quote2 || quote2->type != DOUBLE_QUOTE)
		return ;
	return (make_changes(heredoc, quote1, quote2, word));
}

void	mark_for_expansion(t_token **tokens)
{
	t_token	*current;
	int		in_double;
	int		in_single;

	in_double = 0;
	in_single = 0;
	current = *tokens;
	while (current)
	{
		if (current->type == SINGLE_QUOTE && !in_double)
			in_single = !in_single;
		else if (current->type == DOUBLE_QUOTE && !in_single)
			in_double = !in_double;
		else if (is_word(current->type) && contains_dol(current->content)
			&& !in_single)
			current->needs_expansion = 1;
		if (is_word(current->type) && in_double)
			current->in_double_quotes = 1;
		current = current->next;
	}
	mark_heredoc_exp(tokens);
}
