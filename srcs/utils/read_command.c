/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:23:57 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 16:11:30 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

static void	truncate_cwd(char cwd[256], char full[256])
{
	int	i;

	i = 0;
	while (full[i])
		i++;
	while (full[i] != '/')
		i--;
	i++;
	ft_strlcpy(cwd, "\e[1;32m", 256);
	if (ft_strncmp(full, "/", 2) && ft_strlen(full + i) + 13 < 256)
		ft_strlcat(cwd, full + i, 256);
	else
		ft_strlcat(cwd, "minishell", 256);
	ft_strlcat(cwd, "\x1b[0m", 256);
	ft_strlcat(cwd, " ", 256);
}

char	*read_command(t_shell *shell)
{
	char	*command;
	char	full[256];
	char	cwd[256];

	if (!getcwd(full, 256))
	{
		perror("getcwd");
		clean_shell(shell);
		exit(1);
	}
	truncate_cwd(cwd, full);
	command = readline(cwd);
	return (command);
}
