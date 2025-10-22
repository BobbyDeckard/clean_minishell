/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:05:21 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/22 17:10:04 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	double_expand(t_ast *node, t_cmd *cmd, char **envp, int index);
void	expand(t_ast *node, t_cmd *cmd, char **envp, int index);
int		contains_dol(const char *str);
int		remove_arg(t_cmd *cmd, int i);

void	handle_single_quotes(t_ast *node, t_cmd *cmd, int start)
{
	char	*new;
	int		len;
	int		end;
	int		i;

	len = 0;
	end = start;
	while (cmd->args[++end] && ft_strncmp(cmd->args[end], "'", 2))
		len += ft_strlen(cmd->args[end]);
	if (!len)
	{
		remove_arg(cmd, start);
		remove_arg(cmd, start);
	}
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, cmd->args[start + 1], len);
		remove_arg(cmd, start + 1);
	}
	free(cmd->args[start]);
	cmd->args[start] = new;
}

static void	expand_inside_double(t_ast *node, t_cmd *cmd, int i)
{
	while (cmd->args[++i] && ft_strncmp(cmd->args[i], "\"", 2))
	{
		if (contains_dol(cmd->args[i]))
	  		double_expand(node, cmd, node->shell->envp, i);
	}
}

//	Following comment is about if (*new) condition further below.
//	This condition prevents an empty string to be added as argument,
//	but we maybe want to still get an empty string for example for
//	exit "" ?
void	handle_double_quotes(t_ast *node, t_cmd *cmd, int start)
{
	char	*new;
	int		len;
	int		end;
	int		i;

	len = 0;
	end = start;
	expand_inside_double(node, cmd, start);
	while (cmd->args[++end] && ft_strncmp(cmd->args[end], "\"", 2))
		len += ft_strlen(cmd->args[end]);
	remove_arg(cmd, start);
	if (!len)
	{
		remove_arg(cmd, start);
		return ;
	}
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, cmd->args[start], len);
		if (cmd->args[start + 1] && ft_strncmp(cmd->args[start + 1], "\"", 2))
			remove_arg(cmd, start);
		else if (*new)
		{
			free(cmd->args[start]);
			cmd->args[start] = new;
		}
		else
			free(new);
	}
}
