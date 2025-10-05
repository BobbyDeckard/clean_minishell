/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 22:30:03 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 16:28:57 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	init_cmd(t_shell *data, t_cmd *cmd, int count)
{
	cmd->path = NULL;
	cmd->args = (char **) ft_calloc(count, sizeof(char *));
	if (!cmd->args)
		malloc_error(data->root, data, data->tokens);
	cmd->exp = (int *) ft_calloc(count, sizeof(int));
	if (!cmd->exp)
	{
		free(cmd->args);
		malloc_error(data->root, data, data->tokens);
	}
	cmd->cat = (int *) ft_calloc(count, sizeof(int));
	if (!cmd->cat)
	{
		free(cmd->args);
		free(cmd->exp);
		malloc_error(data->root, data, data->tokens);
	}
}

int	count_args(t_token **tokens, int start, int end)
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
		else if (is_arg(current->type))
			count++;
		else if (is_redir_token(current))
			i++;
	}
	return (count);
}

int	is_arg(t_token_type type)
{
	if (is_word(type) || type == ENV_VAR || type == EXIT_STATUS)
		return (1);
	return (0);
}
