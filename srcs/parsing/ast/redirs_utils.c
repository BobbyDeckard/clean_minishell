/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:29:38 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 16:38:46 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*create_node(t_shell *shell, t_n_type type);
void	clean_ast(t_ast *ast);

t_token	*parse_redir_error(t_ast *node, int i)
{
	if (!node->children[i]->rdr.args[0])
		free(node->children[i]->rdr.args);
	clean_ast(node);
	return (NULL);
}

char	*extract_content(t_token *token)
{
	char	*str;
	int		len;

	str = NULL;
	if (token->content)
	{
		len = ft_strlen(token->content) + 1;
		str = (char *) malloc(len * sizeof(char));
		if (!str)
			return (NULL);
		ft_strlcpy(str, token->content, len);
	}
	else if (token->type == SINGLE_QUOTE || token->type == DOUBLE_QUOTE)
	{
		str = (char *) malloc(2 * sizeof(char));
		if (!str)
			return (NULL);
		if (token->type == SINGLE_QUOTE)
			ft_strlcpy(str, "'", 2);
		else
			ft_strlcpy(str, "\"", 2);
	}
	return (str);
}

static void	alloc_redir_args(t_shell *shell, t_ast *node, int count, int i)
{
	char	**args;

	args = (char **) malloc(count * sizeof(char *));
	if (!args)
	{
		clean_ast(node);
		malloc_error(shell->root, shell, shell->tokens);
	}
	while (--count >= 0)
		args[count] = NULL;
	node->children[i]->rdr.args = args;
}

static t_r_type	convert_rdr_type(t_n_type type)
{
	if (type == NODE_HEREDOC)
		return (RDR_HEREDOC_EXP);
	else if (type == NODE_REDIR_APPEND)
		return (RDR_APPEND);
	else if (type == NODE_REDIR_OUT)
		return (RDR_OUT);
	else if (type == NODE_REDIR_IN)
		return (RDR_IN);
	return (-1);
}

int	create_redir_node(t_shell *shell, t_ast *node, t_n_type type, int count)
{
	int	i;

	i = -1;
	while (node->children[++i])
		continue ;
	node->children[i] = create_node(shell, type);
	if (!node->children[i])
	{
		if (!i)
			free(node->children);
		clean_ast(node);
		return (-1);
	}
	node->children[i]->rdr.type = convert_rdr_type(type);
	alloc_redir_args(shell, node, count, i);
	return (i);
}
