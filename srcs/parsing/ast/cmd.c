/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:11:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 15:29:43 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	handle_arg(t_shell *data, t_cmd *cmd, t_token *current, int j)
{
	char	*content;

	content = current->content;
	if (is_word(current->type) && current->needs_expansion)
	{
		if (current->in_double_quotes)
			cmd->exp[j] = 2;
		else
			cmd->exp[j] = 1;
	}
	if (current->type == WORD_CAT)
		cmd->cat[j] = 1;
	cmd->args[j] = sf_strdup(content, data->tokens, cmd->args, data);
}

static void	parse_cmd(t_shell *data, t_cmd *cmd, int start, int end)
{
	t_token	*current;
	int		i;
	int		j;

	cmd->arg_count = count_args(data->tokens, start, end);
	init_cmd(data, cmd, cmd->arg_count + 1);
	i = start - 1;
	j = 0;
	while (++i <= end && j < cmd->arg_count)
	{
		current = get_token_at_index(data->tokens, i);
		if (!current)
			break ;
		else if (is_arg(current->type) && j < cmd->arg_count)
			handle_arg(data, cmd, current, j++);
		else if (is_redir_token(current))
			i++;
	}
	cmd->args[j] = NULL;
}

t_ast	*parse_command(t_token **tokens, int start, int end, t_shell *data)
{
	t_ast	**redirs;
	t_ast	*node;
	t_cmd	cmd;

	parse_cmd(data, &cmd, start, end);
	node = create_cmd_node(data, tokens, cmd);
	redirs = extract_redirs(data, cmd.args, start, end);
	if (redirs)
		node->children = redirs;
	return (node);
}
