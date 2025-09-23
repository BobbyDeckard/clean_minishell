/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:11:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:34:04 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	handle_arg(t_shell *data, t_cmd *cmd, t_token *current, int j)
{
	if (current->type == ENV_VAR && current->needs_expansion)
	{
		cmd->args[j] = sf_strdup(current->content, data->tokens, cmd->args,
				data);
		cmd->exp[j] = 1;
	}
	else if (current->type == EXIT_STATUS && current->needs_expansion)
	{
		cmd->args[j] = sf_strdup("$?", data->tokens, cmd->args, data);
		cmd->exp[j] = 2;
	}
	else
	{
		cmd->args[j] = sf_strdup(current->content, data->tokens, cmd->args,
				data);
		cmd->exp[j] = 0;
	}
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
		// Why would there be a redir token inside the args ?
		// Check if we can remove or if we should just leave it there.
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
