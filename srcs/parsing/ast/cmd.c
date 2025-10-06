/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:11:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/06 18:43:44 by imeulema         ###   ########.fr       */
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

int	count_redir_related_tokens(t_token *current)
{
	int				count;

//	printf("\nIn count_redir_related_tokens\n");
	count = 0;
	current = current->next;
	if (current->type == WORD)
	{
		current = current->next;
		count++;
//		printf("Starting with WORD token, count = %d\n", count);
	}
	while (current)
	{
		if (current->type == DOUBLE_QUOTE)
		{
			current = current->next;
			count++;
//			printf("Counted DOUBLE QUOTE token, count = %d\n", count);
			while (current && current->type != DOUBLE_QUOTE)
			{
				current = current->next;
				count++;
//				printf("Counted token inside double quotes, count = %d\n", count);
			}
			if (current && current->type == DOUBLE_QUOTE)
			{
				current = current->next;
				count++;
//				printf("Counted second double quote token, count = %d\n", count);
			}
		}
		else if (current->type == SINGLE_QUOTE)
		{
			current = current->next;
			count++;
//			printf("Counted SINGLE QUOTE token, count = %d\n", count);
			while (current && current->type != SINGLE_QUOTE)
			{
				current = current->next;
				count++;
//				printf("Counted token inside single quotes, count = %d\n", count);
			}
			if (current && current->type == SINGLE_QUOTE)
			{
				current = current->next;
				count++;
//				printf("Counted second single quote token, count = %d\n", count);
			}
		}
		else if (current->type == WORD_CAT)
		{
			current = current->next;
			count++;
//			printf("Counted word to cat, count = %d\n", count);
		}
		else
			break ;
	}
//	printf("Counted %d redir related tokens\n\n", count);
	return (count);
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
//		printf("About to fetch token at index %d\n", i);
		current = get_token_at_index(data->tokens, i);
//		if (current->content)
//			printf("Current has content '%s'\n", current->content);
//		else
//			printf("Current token has no content\n");
		if (!current)
			break ;
		else if (is_arg(current->type) && j < cmd->arg_count)
			handle_arg(data, cmd, current, j++);
		else if (is_redir_token(current))
			i += count_redir_related_tokens(current);
	}
	cmd->args[j] = NULL;
}

t_ast	*parse_command(t_token **tokens, int start, int end, t_shell *data)
{
	t_ast	**redirs;
	t_ast	*node;
	t_cmd	cmd;

//	printf("In parse_command(), start = %d, end = %d\n", start, end);
	parse_cmd(data, &cmd, start, end);
	node = create_cmd_node(data, tokens, cmd);
	redirs = extract_redirs(data, cmd.args, start, end);
	if (redirs)
		node->children = redirs;
	return (node);
}
