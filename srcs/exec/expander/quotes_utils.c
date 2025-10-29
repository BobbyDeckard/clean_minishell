/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 18:41:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/29 18:46:25 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_whitespace(const char *str);
int	remove_arg(t_cmd *cmd, int i);

int	handle_empty_quotes(t_cmd *cmd, int index)
{
	if (cmd->args[index + 1] && is_whitespace(cmd->args[index + 1]))
		remove_arg(cmd, index);
	cmd->args[index][0] = 0;
	return (0);
}
