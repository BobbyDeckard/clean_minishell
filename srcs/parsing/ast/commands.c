/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:21:28 by imeulema          #+#    #+#             */
/*   Updated: 2025/11/02 15:04:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*create_node(t_shell *shell, t_n_type type);
t_ast	*parse_lone_redirs(t_shell *shell, t_token **list, int start, int end);
void	clean_ast(t_ast *ast);
void	parse_args(t_shell *shell, t_ast *node, int start, int end);
void	parse_redirs(t_shell *shell, t_ast *node, int start, int end);
int		count_redirs(t_token **list, int start, int end);
int		is_lone_redir(t_token **list, int start, int end);
int		is_redir_arg(t_token *token);
int		is_redir_token(t_t_type type);

static t_ast	*create_cmd_node(t_shell *shell, int redirs)
{
	t_ast	*node;
	int		i;

	node = create_node(shell, NODE_CMD);
	if (!node)
		malloc_error(shell->root, shell, shell->tokens);
	set_root(shell, node);
	if (redirs == 1)
		return (node);
	node->children = (t_ast **) malloc(redirs * sizeof(t_ast *));
	if (!node->children)
		malloc_error(shell->root, shell, shell->tokens);
	i = -1;
	while (++i < redirs)
		node->children[i] = NULL;
	return (node);
}

t_token	*skip_redir(t_token **list, int *start, int end)
{
	t_token	*current;

	current = get_token_at_index(list, ++(*start));
	while (current && current->type == WHITESPACE && (*start)++ <= end)
		current = current->next;
	while (current && is_redir_arg(current) && (*start)++ <= end)
		current = current->next;
	return (current);
}

static int	count_args(t_token **list, int start, int end)
{
	t_token	*current;
	int		count;

	count = 0;
	current = get_token_at_index(list, start);
	while (current && current->type == WHITESPACE && start <= end)
	{
		current = current->next;
		start++;
	}
	while (current && start <= end)
	{
		if (is_redir_token(current->type))
			current = skip_redir(list, &start, end);
		else
		{
			count++;
			current = current->next;
			start++;
		}
	}
	return (count);
}

static void	init_cmd(t_shell *shell, t_ast *node, int count)
{
	int	i;

	if (!count)
		return ;
	node->cmd.args = (char **) malloc((count + 1) * sizeof(char *));
	if (!node->cmd.args)
	{
		clean_ast(node);
		malloc_error(shell->root, shell, shell->tokens);
	}
	i = -1;
	while (++i <= count)
		node->cmd.args[i] = NULL;
}

t_ast	*parse_command(t_shell *shell, t_token **list, int start, int end)
{
	t_ast	*node;
	int		count;

	if (is_lone_redir(list, start, end))
		return (parse_lone_redirs(shell, list, start, end));
	count = count_redirs(list, start, end) + 1;
	node = create_cmd_node(shell, count);
	set_root(shell, node);
	parse_redirs(shell, node, start, end);
	count = count_args(list, start, end);
	init_cmd(shell, node, count);
	parse_args(shell, node, start, end);
	return (node);
}
