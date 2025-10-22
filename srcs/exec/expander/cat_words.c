/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cat_words.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:18:20 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 19:56:18 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	remove_arg(t_cmd *cmd, int i);

int	is_whitespace(const char *str)
{
	if (ft_strlen(str) != 1)
		return (0);
	else if (*str >= 9 && *str <= 13)
		return (1);
	else if (*str == 32)
		return (1);
	return (0);
}

void	cat_args(t_ast *node, t_cmd *cmd, int i)
{
	char	*new;
	int		len;

	len = ft_strlen(cmd->args[i]) + ft_strlen(cmd->args[i + 1]) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, cmd->args[i], len);
	ft_strlcat(new, cmd->args[i + 1], len);
	free(cmd->args[i]);
	cmd->args[i] = new;
	remove_arg(cmd, i + 1);
}
