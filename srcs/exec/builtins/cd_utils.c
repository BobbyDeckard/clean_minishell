/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 21:22:40 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 21:40:09 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

// Once unset, OLDPWD never comes back, unless export OLDPWD is called.
// Then, it is masked until set ?
void	update_pwd(t_ast *node, int i)
{
	
}

// At launch, OLDPWD shouldn't already be set.
// If unset PWD is called before OLDPWD is initialized, cd once then env
// prints no WD variable, cd once more then env and there is OLDPWD correctly set.
// If unset PWD is called after OLDPWD is initialized, env only prints OLDPWD
// until cd is called, then env will not print any WD variable until cd is
// called once again, then env will only print OLDPWD correctly set.
void	update_oldpwd(t_ast *node, int i, char *oldpwd)
{

}
