/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 16:13:06 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 16:13:30 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	cleanup(t_ast *node);

void	clean_exit(t_ast *node, int status)
{
	cleanup(node->root);
	exit(status);
}
