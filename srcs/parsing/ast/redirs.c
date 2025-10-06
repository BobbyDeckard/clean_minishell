/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:14:43 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/06 15:20:25 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	count_redirs(t_token **tokens, int start, int end)
{
	t_token	*current;
	int		count;
	int		i;

	count = 0;
	i = start - 1;
	while (++i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (is_redir_token(current) && i + 1 <= end)
		{
			current = get_token_at_index(tokens, i + 1);
			if (current && is_word(current->type))
				count++;
		}
	}
	return (count);
}

static t_ast	**free_redirs(t_ast **redirs, int i)
{
	while (--i >= 0)
		free(redirs[i]);
	free(redirs);
	return (NULL);
}

static int	count_redir_args(t_token *current)
{
	int	i;

	i = 0;
	current = current->next;
	if (current && current->type == WORD)
	{
		current = current->next;
		i++;
	}
	while (current)
	{
		if (current->type == WORD_CAT)
			i++;
		else if (current->type != SINGLE_QUOTE && current->type != DOUBLE_QUOTE)
			break ;
		current = current->next;
	}
	printf("Counted %d args for redir\n", i);
	return (i);
}

static void	free_cmd(t_cmd cmd)
{
	int	i;

	i = -1;
	while (cmd.args[++i])
		free(cmd.args[i]);
	free(cmd.args);
	free(cmd.exp);
	free(cmd.cat);
}

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
	if (!node && content)
		free(content);
	else if (!node && cmd.args)
		free_cmd(cmd);
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


/*
t_ast	*parse_redir(t_shell *data, t_token **tokens, int *start, int end)
{
	t_token	*current;
	t_ast	*redir;
	t_cmd	cmd;
	int		i;
	int		j;

	current = get_token_at_index(tokens, *start);
	cmd.arg_count = count_redir_related_tokens(current);
	init_cmd(data, &cmd, cmd.arg_count + 1);
	i = *start - 1;
	j = 0;
	while (++i <= end && j < cmd.arg_count)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		else if (is_word(current->type))
		{
			cmd.args[j] = (char *) malloc((ft_strlen(current->content) + 1) * sizeof(char));
			if (!cmd.args[j])
			{
				while (--j >= 0)
					free(cmd.args[j]);
				free(cmd.args);
				free(cmd.exp);
				free(cmd.cat);
				return (NULL);
			}
			else if (current->type == WORD_CAT)
				cmd.cat[j] = 1;
			if (current->needs_expansion && current->in_double_quotes)
				cmd.exp[j] = 2;
			else if (current->needs_expansion)
				cmd.exp[j] = 1;
			j++;
		}
	}
	*start += j;
	return (create_redir(data, get_token_at_index
}
*/

/*
// Why pass args to below func ?
t_ast	**extract_redirs(t_shell *data, char **args, int start, int end)
{
	t_token	**tokens;
	t_ast	**redirs;
	int		count;
	int		i;

	tokens = data->tokens;
	count = count_redirs(tokens, start, end);
	if (!count)
		return (NULL);
	redirs = (t_ast **) malloc((count + 1) * sizeof(t_ast *));
	if (!redirs)
	{
		free_char_array(args);
		malloc_error(data->root, data, tokens);
	}
	i = -1;
	while (++i < count)
	{
		redirs[i] = parse_redir(data, tokens, &start, end);
		if (!redirs[i])
		{
			free_char_array(args);
			while (--i >= 0)
				free(redirs[i]);
			free(redirs);
			malloc_error(data->root, data, tokens);
		}
	}
}
*/
