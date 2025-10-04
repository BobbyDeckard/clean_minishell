/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:48:13 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/04 19:29:59 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	heredoc_error(t_ast *node, char *del)
{
	free(del);
	cleanup(node);
	ft_putstr_fd("minishell: error creating temporary here-document\n", 2);
}

static void	loop_sigint(t_ast *node, t_cmd *cmd, int *stdin_bu)
{
	if (!isatty(STDIN_FILENO))
	{
		dup2(*stdin_bu, STDIN_FILENO);
		close(*stdin_bu);
		close(cmd->fd_in);
		unlink(node->file);
		cmd->fd_in = -1;
		write(STDOUT_FILENO, "\n", 1);
	}
}

static void	heredoc_loop(t_ast *node, t_cmd *cmd, char *del, int *stdin_backup)
{
	char	*line;
	int		len;

	len = ft_strlen(del) + 1;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			loop_sigint(node, cmd, stdin_backup);
			break ;
		}
		if (!ft_strncmp(line, del, len))
		{
			free(line);
			break ;
		}
		if (node->type == NODE_HEREDOC_EXP)
			line = expand_line(node, node->data->envp, line);
		ft_putstr_fd(line, cmd->fd_in);
		ft_putchar_fd('\n', cmd->fd_in);
		free(line);
	}
}

void	make_heredoc(t_ast *node, t_cmd *cmd)
{
	struct sigaction	new;
	struct sigaction	old;
	char				*del;
	int					stdin_backup;

	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	del = copy_delimiter(node);
	free(node->file);
	node->file = NULL;
	if (!open_temp(node, cmd))
		heredoc_error(node, del);
	stdin_backup = dup(STDIN_FILENO);
	init_sp_handler_sig(node, &new, &old);
	heredoc_loop(node, cmd, del, &stdin_backup);
	heredoc_end(node, &old, stdin_backup);
	if (cmd->fd_in != -1)
	{
		close(cmd->fd_in);
		cmd->fd_in = open(node->file, O_RDONLY);
		if (cmd->fd_in < 0)
			perror(node->file);
	}
	free(del);
}
