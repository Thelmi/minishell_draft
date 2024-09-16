/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:17:37 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/16 20:30:30 by mrhelmy          ###   ########.fr       */
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
		env = tmp;
		free(env->variable);
		free(env->value);
		free(env);
		tmp = env->next;
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

void freecmd(struct cmd *cmd)
{
    int i;
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if (cmd == NULL)
        return;
    if (cmd->type == EXEC) {
        ecmd = (struct execcmd *)cmd;
        i = 0;
        while (ecmd->argv[i] != NULL) 
        { 
            free(ecmd->argv[i]);
            i++;
        }
        free(cmd);
    } else if (cmd->type == PIPE) {
        pcmd = (struct pipecmd *)cmd;
        freecmd(pcmd->left);
        freecmd(pcmd->right);
        free(cmd);
    } else if (cmd->type == REDIR) {
        rcmd = (struct redircmd *)cmd;
        freecmd(rcmd->cmd);
        free(cmd);
    }
}

void freeheredoc(struct heredoc *heredoc)
{
    struct heredoc *tmp;
    while (heredoc) {
        tmp = heredoc;
        heredoc = heredoc->next;
        free(tmp->argv);
        free(tmp);
    }
}

void cleanup(t_main *main)
{
    if (main->cmd) {
        freecmd(main->cmd);
    }
    if (main->heredoc) {
        freeheredoc(main->heredoc);
    }
}
