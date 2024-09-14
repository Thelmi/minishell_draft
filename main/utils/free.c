/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:17:37 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/10 20:54:35 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// our freeing functions
#include "../minishell.h"

void free_env(t_env *env)
{
	t_env *tmp;
	
	tmp = env;
	while (tmp != NULL)
	{
		tmp = env->next;
		free(env->variable);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void free_export(t_export *export)
{
	t_export *tmp;
	
	tmp = export;
	while (tmp != NULL)
	{
		tmp = export->next;
		free(export->variable);
		free(export->value);
		free(export);
		export = tmp;
	}
}

void free_env_node(t_env *node)
{
    if (node)
    {
        free(node->variable);
        free(node->value);
        free(node);
    }
}