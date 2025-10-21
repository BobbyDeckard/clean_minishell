/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:07:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 09:50:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	rdr_double_expand(t_ast *node, t_rdr *rdr, char **envp, int index);
int		contains_dol(const char *str);
int		rdr_expand_cat(t_ast *node, t_rdr *rdr, char **envp, int index);
int		rdr_remove_arg(t_rdr *rdr, int i);

static void	handle_single_quotes(t_ast *node, t_rdr *rdr, int start)
{
	char	*new;
	int		len;
	int		end;
	int		i;

	len = 0;
	end = start;
	while (rdr->args[++end] && ft_strncmp(rdr->args[end], "'", 2))
		len += ft_strlen(rdr->args[end]);
	if (!len)
	{
		rdr_remove_arg(rdr, start);
		rdr_remove_arg(rdr, start);
	}
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, rdr->args[start + 1], len);
		rdr_remove_arg(rdr, start + 1);
	}
	free(rdr->args[start]);
	rdr->args[start] = new;
}

static void	rdr_expand(t_ast *node, t_rdr *rdr, char **envp, int index)
{
	while (contains_dol(rdr->args[index]))
	{
		if (rdr_expand_cat(node, rdr, envp, index))
			return ;
	}
}

static void	expand_inside_double(t_ast *node, t_rdr *rdr, int i)
{
	while (rdr->args[++i] && ft_strncmp(rdr->args[i], "\"", 2))
	{
		if (contains_dol(rdr->args[i]))
			rdr_double_expand(node, rdr, node->shell->envp, i);
	}
}

static void	handle_double_quotes(t_ast *node, t_rdr *rdr, int start)
{
	char	*new;
	int		len;
	int		end;
	int		i;

	expand_inside_double(node, rdr, start);
	len = 0;
	end = start;
	while (rdr->args[++end] && ft_strncmp(rdr->args[end], "\"", 2))
		len += ft_strlen(rdr->args[end]);
	if (!len)
	{
		rdr_remove_arg(rdr, start);
		rdr_remove_arg(rdr, start);
	}
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, rdr->args[start + 1], len);
		rdr_remove_arg(rdr, start + 1);
	}
	free(rdr->args[start]);
	rdr->args[start] = new;
}

void	redir_expander(t_ast *node, t_rdr *rdr)
{
	int	len;
	int	i;

	i = -1;
	while (rdr->args[++i])
	{
		if (!ft_strncmp(rdr->args[i], "'", 2))
			handle_single_quotes(node, rdr, i);
		else if (!ft_strncmp(rdr->args[i], "\"", 2))
			handle_double_quotes(node, rdr, i);
		else if (contains_dol(rdr->args[i]))
			rdr_expand(node, rdr, node->shell->envp, i);
	}
	i = -1;
	len = 0;
	while (rdr->args[++i])
		len += ft_strlen(rdr->args[i]);
	if (!len)
		return ;
	rdr->file = (char *) ft_calloc(++len, sizeof(char));
	if (!rdr->file)
		malloc_error(node, node->shell, NULL);
	i = -1;
	while (rdr->args[++i])
		ft_strlcat(rdr->file, rdr->args[i], len);
}
