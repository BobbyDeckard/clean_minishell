#include "../../incl/minishell.h"

void	clean_args(char **args);
void	clean_ast(t_ast *ast);

void	clean_children(t_ast *ast)
{
	int	i;

	i = -1;
	while (ast->children[++i])
	{
		clean_ast(ast->children[i]);
		ast->children[i] = NULL;
	}
	free(ast->children);
	ast->children = NULL;
}

void	clean_redirs(t_ast *ast)
{
	if (ast->rdr.args)
	{
		clean_args(ast->rdr.args);
		free(ast->rdr.args);
		ast->rdr.args = NULL;
	}
	if (ast->rdr.file)
	{
		free(ast->rdr.file);
		ast->rdr.file = NULL;
	}
}