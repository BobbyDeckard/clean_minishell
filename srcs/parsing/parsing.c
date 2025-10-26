/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:30:24 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 20:21:03 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

t_token	**tokenize_command(t_shell *shell, char *command);
t_ast	*create_ast(t_shell *shell, t_token **list);
void	clean_tokens(t_token **list);
int		check_operators(t_token **list);
int		check_parentheses(t_token **list);
int		check_pipes(t_token **list);
int		check_quotes(t_shell *shell, t_token **list);
int		check_redirections(t_token **list);

static int	check_words(t_token **list)
{
	t_token	*current;
	int		word;

	word = 0;
	current = *list;
	while (current)
	{
		if (current->type == WORD)
			word = 1;
		current = current->next;
	}
	return (!word);
}

static int	check_syntax(t_shell *shell, t_token **list)
{
	if (!*list)
		return (1);
	if (check_quotes(shell, list))
		return (2);
	else if (check_operators(list))
		return (3);
	else if (check_pipes(list))
		return (4);
	else if (check_redirections(list))
		return (5);
	else if (check_parentheses(list))
		return (6);
	if (check_words(list))
		return (7);
	return (0);
}

static t_ast	*invalid_syntax(t_token **list, int err)
{
	if (err != 7)
		ft_putstr_fd("minishell: syntax error in command (", 2);
	if (err == 1)
		ft_putstr_fd("unexpected tokenization error)\n", 2);
	else if (err == 2)
		ft_putstr_fd("invalid quote)\n", 2);
	else if (err == 3)
		ft_putstr_fd("invalid operator)\n", 2);
	else if (err == 4)
		ft_putstr_fd("invalid pipe)\n", 2);
	else if (err == 5)
		ft_putstr_fd("invalid redir)\n", 2);
	else if (err == 6)
		ft_putstr_fd("invalid parenthesis)\n", 2);
	clean_tokens(list);
	return (NULL);
}

void	print_token_list(t_token **token_list);
void	print_tree(t_ast *ast);
t_ast	*parse(char *command, t_shell *shell)
{
	t_token	**list;
	t_ast	*ast;
	int		err;

	list = tokenize_command(shell, command);
	if (!list)
	{
		ft_putstr_fd("Unexpected tokenization error\n", 2);
		return (NULL);
	}
	err = check_syntax(shell, list);
	if (err)
		return (invalid_syntax(list, err));
	shell->tokens = list;
	ast = create_ast(shell, list);
	return (ast);
}
