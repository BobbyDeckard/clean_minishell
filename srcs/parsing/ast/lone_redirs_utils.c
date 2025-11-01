/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lone_redirs_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:38:52 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/28 16:38:54 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*skip_spaces(t_token **list, int *start, int end);
int		is_arg_token(t_t_type type);
int		is_redir_arg(t_token *token);
int		is_redir_token(t_t_type type);

int	is_lone_redir(t_token **list, int start, int end)
{
	t_token	*current;

	printf("\nIn is_lone_redir for sequence: %d-%d\n", start, end);
	current = get_token_at_index(list, start);
	if (!is_redir_token(current->type))
		return (0);
	printf("Confirmed token is redir\n");
	current = current->next;
	start++;
	printf("Skipped redir token, new sequence: %d-%d\n", start, end);
	current = skip_spaces(list, &start, end);
	printf("Skipped spaces, new sequence %d-%d\n", start, end);
	while (current && is_redir_arg(current) && start < end)
	{
		current = current->next;
		start++;
	}
	printf("Skipped redir args, new sequence: %d-%d\n", start, end);
	while (current && current->type == WHITESPACE && start < end)
	{
		current = current->next;
		start++;
	}
	printf("Skipped spaces, new sequence: %d-%d\n", start, end);
	if (current && is_redir_token(current->type))
		return (is_lone_redir(list, start, end));
	else if (current && is_arg_token(current->type))
	{
		printf("Following command found, returning 0, sequence: %d-%d\n", start, end);
		return (0);
	}
	printf("Flagging lone redir, sequence: %d-%d\n", start, end);
	return (1);
}

void	alloc_redir_args(t_shell *shell, t_ast *node, int count)
{
	node->rdr.args = (char **) malloc(count * sizeof(char *));
	if (!node->rdr.args)
		malloc_error(shell->root, shell, shell->tokens);
	while (--count >= 0)
		node->rdr.args[count] = NULL;
}

static void	make_arg_error(t_shell *shell, t_ast *node)
{
	if (!node->rdr.args[0])
		free(node->rdr.args);
	malloc_error(shell->root, shell, shell->tokens);
}

char	*make_arg(t_shell *shell, t_ast *node, t_token *current)
{
	char	*str;
	int		len;

	str = NULL;
	if (current->content)
	{
		len = ft_strlen(current->content) + 1;
		str = (char *) malloc(len * sizeof(char));
		if (!str)
			make_arg_error(shell, node);
		ft_strlcpy(str, current->content, len);
	}
	else if (current->type == SINGLE_QUOTE || current->type == DOUBLE_QUOTE)
	{
		str = (char *) malloc(2 * sizeof(char));
		if (!str)
			make_arg_error(shell, node);
		if (current->type == SINGLE_QUOTE)
			ft_strlcpy(str, "'", 2);
		else
			ft_strlcpy(str, "\"", 2);
	}
	return (str);
}
