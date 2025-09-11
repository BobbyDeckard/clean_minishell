/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_arr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:18:42 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 15:21:33 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	char_arr_len(char **arr)
{
	int	i;

	if (!arr)
		return (-1);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	free_char_array_size(char **arr, int i)
{
	while (--i >= 0)
		free(arr[i]);
	free(arr);
}

void	free_char_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}
