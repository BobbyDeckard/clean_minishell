#include "../../../incl/minishell.h"

int	is_lone_redir_node(t_ast *node);

void	cmd_not_found(t_ast *node, char *name)
{
	if (!is_lone_redir(node))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
}