/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:40:50 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 16:30:46 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*tokenize_and(char **command, t_token *token);
t_token	*tokenize_double_quote(char **command, t_token *token);
t_token	*tokenize_heredoc(char **command, t_token *token);
t_token	*tokenize_or(char **command, t_token *token);
t_token	*tokenize_paren_close(char **command, t_token *token);
t_token	*tokenize_paren_open(char **command, t_token *token);
t_token	*tokenize_pipe(char **command, t_token *token);
t_token	*tokenize_redir_append(char **command, t_token *token);
t_token	*tokenize_redir_in(char **command, t_token *token);
t_token	*tokenize_redir_out(char **command, t_token *token);
t_token	*tokenize_single_quote(char **command, t_token *token);
t_token	*tokenize_whitespace(t_shell *shell, char **command, t_token **list, t_token *token);
t_token	*tokenize_word(t_shell *shell, char **command, t_token **list, t_token *token);

static t_token	*init_token(t_shell *shell, t_token **tokens)
{
	t_token	*new;

	new = (t_token *) malloc(sizeof(t_token));
	if (!new)
		malloc_error(NULL, shell, tokens);
	new->content = NULL;
	new->next = NULL;
	new->prev = NULL;
	new->type = UNSET;
	return (new);
}

static void	link_token(t_token **list, t_token *token)
{
	t_token	*current;

	if (!*list)
	{
		*list = token;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = token;
	token->prev = current;
}

static t_token	*make_token(t_shell *shell, char **command, t_token **list, t_token *token)
{
	if (**command == '|' && **command + 1 && **command + 1 == '|')
		return (tokenize_or(command, token));
	else if (**command == '|')
		return (tokenize_pipe(command, token));
	else if (**command == '&' && **command + 1 && **command + 1 == '&')
		return (tokenize_and(command, token));
	else if (**command == '(')
		return (tokenize_paren_open(command, token));
	else if (**command == ')')
		return (tokenize_paren_close(command, token));
	else if (**command == '"')
		return (tokenize_double_quote(command, token));
	else if (**command == '\'')
		return (tokenize_single_quote(command, token));
	else if (**command == '<' && **command + 1 && **command + 1 == '<')
		return (tokenize_heredoc(command, token));
	else if (**command == '<')
		return (tokenize_redir_in(command, token));
	else if (**command == '>' && **command + 1 && **command + 1 == '>')
		return (tokenize_redir_append(command, token));
	else if (**command == '>')
		return (tokenize_redir_out(command, token));
	else if (**command == ' ' || **command == 9)
		return (tokenize_whitespace(shell, command, list, token));
	return (tokenize_word(shell, command, list, token));
}

static t_token	**extract_token(t_shell *shell, char **command, t_token **list)
{
	t_token	*new;

	new = init_token(shell, list);
	new = make_token(shell, command, list, new);
	if (new)
		link_token(list, new);
	return (list);
}

void	print_token_type(t_t_type type);

static void print_last_token(t_token **list)
{
	t_token	*current;

	current = *list;
	while (current->next)
		current = current->next;
	printf("Extracted token of type ");
	print_token_type(current->type);
	if (current->content)
		printf(" with content = '%s'", current->content);
	printf("\n");
}

t_token	**tokenize_command(t_shell *shell, char *command)
{
	t_token		**tokens;
	
	tokens = (t_token **) malloc(sizeof(t_token *));
	if (!tokens)
		malloc_error(NULL, shell, NULL);
	*tokens = NULL;
	while (*command)
	{
		printf("Remaining command: '%s'\n", command);
		tokens = extract_token(shell, &command, tokens);
		print_last_token(tokens);
	}
	return (tokens);
}
