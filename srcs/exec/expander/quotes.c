/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:05:21 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/29 18:48:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	double_expand(t_ast *node, t_cmd *cmd, char **envp, int index);
int		contains_dol(const char *str);
int		handle_empty_quotes(t_cmd *cmd, int index);
int		is_whitespace(const char *str);
int		remove_arg(t_cmd *cmd, int i);

static void	single_quotes_loop(t_cmd *cmd, char	*new, int st_end[2], int len)
{
	int	start;
	int	end;
	int	i;

	start = st_end[0];
	end = st_end[1];
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, cmd->args[start], len);
		if (cmd->args[start + 1] && ft_strncmp(cmd->args[start + 1], "'", 2))
			remove_arg(cmd, start);
		else if (*new)
		{
			free(cmd->args[start]);
			cmd->args[start] = new;
			remove_arg(cmd, start + 1);
			break ;
		}
		else
			free(new);
	}
}

int	handle_single_quotes(t_ast *node, t_cmd *cmd, int start)
{
	char	*new;
	int		st_end[2];
	int		len;
	int		end;

	len = 0;
	end = start;
	while (cmd->args[++end] && ft_strncmp(cmd->args[end], "'", 2))
		len += ft_strlen(cmd->args[end]);
	remove_arg(cmd, start);
	if (!len)
		return (handle_empty_quotes(cmd, start));
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	st_end[0] = start;
	st_end[1] = end;
	single_quotes_loop(cmd, new, st_end, len);
	return (0);
}

static void	expand_inside_double(t_ast *node, t_cmd *cmd, int i)
{
	while (cmd->args[++i] && ft_strncmp(cmd->args[i], "\"", 2))
	{
		if (contains_dol(cmd->args[i]))
			double_expand(node, cmd, node->shell->envp, i);
	}
}

static void	double_quotes_loop(t_cmd *cmd, char *new, int st_end[2], int len)
{
	int	start;
	int	end;
	int	i;

	start = st_end[0];
	end = st_end[1];
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, cmd->args[start], len);
		if (cmd->args[start + 1] && ft_strncmp(cmd->args[start + 1], "\"", 2))
			remove_arg(cmd, start);
		else if (new)
		{
			free(cmd->args[start]);
			cmd->args[start] = new;
			remove_arg(cmd, start + 1);
			break ;
		}
		else
			free(new);
	}
}

int	handle_double_quotes(t_ast *node, t_cmd *cmd, int start)
{
	char	*new;
	int		st_end[2];
	int		len;
	int		end;

	len = 0;
	end = start;
	expand_inside_double(node, cmd, start);
	while (cmd->args[++end] && ft_strncmp(cmd->args[end], "\"", 2))
		len += ft_strlen(cmd->args[end]);
	remove_arg(cmd, start);
	if (!len)
		return (handle_empty_quotes(cmd, start));
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	st_end[0] = start;
	st_end[1] = end;
	double_quotes_loop(cmd, new, st_end, len);
	return (0);
}
