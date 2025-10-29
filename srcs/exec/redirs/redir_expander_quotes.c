/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_expander_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:07:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/29 19:06:10 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	rdr_double_expand(t_ast *node, t_rdr *rdr, char **envp, int index);
void	rdr_handle_empty_quotes(t_rdr *rdr, int index);
int		contains_dol(const char *str);
int		rdr_remove_arg(t_rdr *rdr, int i);

static void	single_quotes_loop(t_rdr *rdr, char *new, int st_end[2], int len)
{
	int	start;
	int	end;
	int	i;

	start = st_end[0];
	end = st_end[1];
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, rdr->args[start], len);
		if (rdr->args[start + 1] && ft_strncmp(rdr->args[start + 1], "'", 2))
			rdr_remove_arg(rdr, start);
		else if (*new)
		{
			free(rdr->args[start]);
			rdr->args[start] = new;
			rdr_remove_arg(rdr, start + 1);
			break ;
		}
		else
			free(new);
	}
}

int	rdr_handle_single_quotes(t_ast *node, t_rdr *rdr, int start)
{
	char	*new;
	int		st_end[2];
	int		len;
	int		end;

	len = 0;
	end = start;
	while (rdr->args[++end] && ft_strncmp(rdr->args[end], "'", 2))
		len += ft_strlen(rdr->args[end]);
	rdr_remove_arg(rdr, start);
	if (!len)
	{
		rdr->args[start][0] = 0;
		return (0);
	}
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	st_end[0] = start;
	st_end[1] = end;
	single_quotes_loop(rdr, new, st_end, len);
	return (0);
}

static void	expand_inside_double(t_ast *node, t_rdr *rdr, int i)
{
	while (rdr->args[++i] && ft_strncmp(rdr->args[i], "\"", 2))
	{
		while (contains_dol(rdr->args[i]))
			rdr_double_expand(node, rdr, node->shell->envp, i);
	}
}

static void	double_quotes_loop(t_rdr *rdr, char *new, int st_end[2], int len)
{
	int	start;
	int	end;
	int	i;

	start = st_end[0];
	end = st_end[1];
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, rdr->args[start], len);
		if (rdr->args[start + 1] && ft_strncmp(rdr->args[start + 1], "\"", 2))
			rdr_remove_arg(rdr, start);
		else if (*new)
		{
			free(rdr->args[start]);
			rdr->args[start] = new;
			rdr_remove_arg(rdr, start + 1);
			break ;
		}
		else
			free(new);
	}
}

int	rdr_handle_double_quotes(t_ast *node, t_rdr *rdr, int start)
{
	char	*new;
	int		st_end[2];
	int		len;
	int		end;

	len = 0;
	end = start;
	expand_inside_double(node, rdr, start);
	while (rdr->args[++end] && ft_strncmp(rdr->args[end], "\"", 2))
		len += ft_strlen(rdr->args[end]);
	rdr_remove_arg(rdr, start);
	if (!len)
	{
		rdr->args[start][0] = 0;
		return (0);
	}
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	st_end[0] = start;
	st_end[1] = end;
	double_quotes_loop(rdr, new, st_end, len);
	return (0);
}
