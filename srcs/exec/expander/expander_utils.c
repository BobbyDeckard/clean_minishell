/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 20:18:46 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/22 15:02:08 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	cat_args(t_ast *node, t_cmd *cmd, int i);
void	handle_double_quotes(t_ast *node, t_cmd *cmd, int start);
void	handle_single_quotes(t_ast *node, t_cmd *cmd, int start);
int		get_name_len(const char *str);
int		handle_exit_status(t_ast *node, t_cmd *cmd, int index);
int		handle_var(t_ast *node, t_cmd *cmd, char *entry, int index);
int		is_whitespace(const char *str);
int		remove_arg(t_cmd *cmd, int i);
int		remove_var(t_ast *node, t_cmd *cmd, int *index);

int	contains_dol(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$')
			return (1);
	}
	return (0);
}

char	*get_name(t_ast *node, const char *str)
{
	char	*name;
	int		len;
	int		i;

	while (*str && *str != '$')
		str++;
	if (*(str + 1) == '?')
		return (NULL);
	len = get_name_len(str) + 1;
	name = (char *) malloc(len * sizeof(char));
	if (!name)
		malloc_error(node, node->shell, NULL);
	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		name[i - 1] = str[i];
		i++;
	}
	name[i - 1] = 0;
	return (name);
}
