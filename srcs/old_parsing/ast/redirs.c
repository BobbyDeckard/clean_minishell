/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:14:43 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 13:56:13 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static char	*set_content(t_token *current)
{
	while (current && !is_word(current->type))
		current = current->next;
	if (!current || !current->content)
		return (NULL);
	return (ft_strdup(current->content));
}

static int	parse_redir_args(t_cmd *cmd, t_token *current)
{
	int	i;

	i = -1;
	while (current && i + 1 < cmd->arg_count)
	{
		if (is_word(current->type))
		{
			cmd->args[++i] = ft_strdup(current->content);
			if (!cmd->args[i])
			{
				while (--i >= 0)
					free(cmd->args[i]);
				free(cmd->args);
				return (1);
			}
			else if (current->type == WORD_CAT)
				cmd->cat[i] = 1;
			if (current->needs_expansion && current->in_double_quotes)
				cmd->exp[i] = 2;
			else if (current->needs_expansion)
				cmd->exp[i] = 1;
		}
		current = current->next;
	}
	return (0);
}

static t_ast	*redir_node_helper(t_shell *data, t_token *current)
{
	t_node_type	type;
	t_ast		*node;
	t_cmd		cmd;
	char		*content;

	cmd.arg_count = count_redir_args(current);
	if (cmd.arg_count == 1)
	{
		bzero_cmd(&cmd);
		content = set_content(current);
		if (!content)
			return (NULL);
	}
	else
	{
		init_cmd(data, &cmd, cmd.arg_count + 1);
		if (parse_redir_args(&cmd, current))
			return (NULL);
		content = NULL;
	}
	type = convert_types(current->type);
	node = create_redir_node(data, type, content, cmd);
	free_content_or_cmd(node, content, cmd);
	return (node);
}

static t_ast	**extract_redirs_body(t_ast **redirs, t_shell *data, int sec[3])
{
	t_token	*current;
	int		j;

	--(sec[0]);
	j = 0;
	while (++(sec[0]) <= sec[1] && j < sec[2])
	{
		current = get_token_at_index(data->tokens, sec[0]);
		if (!current)
			break ;
		else if (is_redir_token(current) && sec[0] + 1 <= sec[1])
		{
			if (current->next && is_word(current->next->type))
			{
				redirs[j] = redir_node_helper(data, current);
				if (!redirs[j])
					return (free_redirs(redirs, j));
				j++;
			}
			(sec[0])++;
		}
	}
	return (redirs);
}

t_ast	**extract_redirs(t_shell *data, char **args, int start, int end)
{
	t_token	**tokens;
	t_ast	**redirs;
	int		sec[3];
	int		count;

	tokens = data->tokens;
	count = count_redirs(tokens, start, end);
	sec[0] = start;
	sec[1] = end;
	sec[2] = count;
	if (!count)
		return (NULL);
	redirs = (t_ast **) malloc(++count * sizeof(t_ast *));
	if (!redirs)
	{
		free_char_array(args);
		malloc_error(data->root, data, tokens);
	}
	redirs = extract_redirs_body(redirs, data, sec);
	if (!redirs)
	{
		free_char_array(args);
		malloc_error(data->root, data, tokens);
	}
	return (redirs);
}
