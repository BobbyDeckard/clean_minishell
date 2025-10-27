/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:48:13 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 16:51:27 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*copy_delimiter(t_ast *node);
char	*expand_line(t_ast *node, char **envp, char *line);
void	cleanup(t_ast *node);
void	init_sp_handler_sig(t_ast *node, struct sigaction *new,
		struct sigaction *old);
void	heredoc_end(t_ast *node, struct sigaction *old,
		int stdin_bu);
void	make_del(t_ast *node);
int		open_temp(t_ast *node, t_cmd *cmd);

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
		if (close(*stdin_bu))
			perror("close");
		*stdin_bu = -1;
		if (close(cmd->fd_in))
			perror("close");
		unlink(node->rdr.file);
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
		if (node->rdr.type == RDR_HEREDOC_EXP)
			line = expand_line(node, node->shell->envp, line);
		ft_putstr_fd(line, cmd->fd_in);
		ft_putchar_fd('\n', cmd->fd_in);
		free(line);
	}
}

static void	heredoc_end_end(t_ast *node, t_cmd *cmd, char *del)
{
	if (cmd->fd_in >= 0)
	{
		if (close(cmd->fd_in))
			perror("close");
		cmd->fd_in = open(node->rdr.file, O_RDONLY);
		if (cmd->fd_in < 0)
			perror(node->rdr.file);
	}
	free(del);
}

void	make_heredoc(t_ast *node, t_cmd *cmd)
{
	struct sigaction	new;
	struct sigaction	old;
	char				*del;
	int					stdin_backup;

	if (cmd->fd_in != STDIN_FILENO)
	{
		if (close(cmd->fd_in))
			perror("close");
	}
	make_del(node);
	del = copy_delimiter(node);
	free(node->rdr.file);
	node->rdr.file = NULL;
	if (!open_temp(node, cmd))
		heredoc_error(node, del);
	stdin_backup = dup(STDIN_FILENO);
	init_sp_handler_sig(node, &new, &old);
	heredoc_loop(node, cmd, del, &stdin_backup);
	heredoc_end(node, &old, stdin_backup);
	heredoc_end_end(node, cmd, del);
}
