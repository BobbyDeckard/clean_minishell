/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cat_words.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:18:20 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 16:27:39 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	cat_args(t_ast *node, t_cmd *cmd, int i)
{
	char	*new;
	int		len;

	if (!i)
		return ;
	len = ft_strlen(cmd->args[i - 1]) + ft_strlen(cmd->args[i]) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(new, cmd->args[i - 1], len);
	ft_strlcat(new, cmd->args[i], len);
	free(cmd->args[i - 1]);
	cmd->args[i - 1] = new;
	free(cmd->args[i]);
	while (i < cmd->arg_count)
	{
		cmd->args[i] = cmd->args[i + 1];
		if (i + 1 < cmd->arg_count)
		{
			cmd->exp[i] = cmd->exp[i + 1];
			cmd->cat[i] = cmd->cat[i + 1];
		}
		i++;
	}
	cmd->arg_count--;
}

void	cat_words(t_ast *node, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (++i < cmd->arg_count)
	{
		if (cmd->cat[i])
			cat_args(node, cmd, i);
	}
}
