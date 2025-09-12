/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:11:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:15:49 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
static char	**extract_args(t_token **tokens, int st_nd[2], t_shell *data, t_cmd *cmd)
{
	t_token	*current;
	char	**args;
	int		count;
	int		i;
	int		j;

	count = count_args(tokens, st_nd[0], st_nd[1]);
	args = (char **) malloc((count + 1) * sizeof(char *));
	if (!args)
		malloc_error(data->root, data, tokens);
	i = st_nd[0] - 1;
	j = 0;
	while (++i <= st_nd[1] && j < count)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (is_arg(current->type))
			args[j++] = sf_strdup(current->content, tokens, args, data);
		else if (is_redir_token(current))
			i++;
	}
	args[j] = NULL;
	return (args);
}
*/

static void	handle_arg(t_shell *data, t_cmd *cmd, t_token *current, int j)
{
	if (current->type == ENV_VAR && current->needs_expansion)
	{
		cmd->args[j] = sf_strdup(current->content, data->tokens, cmd->args, data);
//		printf("Transferred %s from ENV_VAR token\n", cmd->args[j]);
		cmd->exp[j] = 1;
	}
	else if (current->type == EXIT_STATUS && current->needs_expansion)
	{
		cmd->args[j] = sf_strdup("$?", data->tokens, cmd->args, data);
//		printf("Transferred %s from EXIT_STATUS token\n", cmd->args[j]);
		cmd->exp[j] = 2;
	}
	else
	{
		cmd->args[j] = sf_strdup(current->content, data->tokens, cmd->args, data);
//		printf("Transferred %s from WORD token\n", cmd->args[j]);
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
		printf("Value of i: %d, value of j: %d\n", i, j);
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
