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
		tmp = env->next;
		free(env->variable);
		free(env->value);
		free(env);
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

void freecmd(struct cmd *cmd, int x)
{
    int i;
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if (cmd == NULL)
        return;
    // printf("Yes\n");
    if (cmd->type == EXEC) {
        // printf("No\n");
        ecmd = (struct execcmd *)cmd;
        i = 0;
        while (x == 0 && ecmd->argv[i] != NULL) 
        { 
            // printf("%s\n", ecmd->argv[i]);
            if (x == 0) //i == 1, means there was an error in the tree
                free(ecmd->argv[i]);
            i++;
        }
        if (cmd)
            free(cmd);
        cmd = NULL;
    } else if (cmd->type == PIPE) {
        pcmd = (struct pipecmd *)cmd;
        freecmd(pcmd->left, x);
        freecmd(pcmd->right, x);
        free(cmd);
    } else if (cmd->type == REDIR) {
        rcmd = (struct redircmd *)cmd;
        freecmd(rcmd->cmd, x);
        free(cmd);
        // printf("******\n");
    }
}

void freeheredoc(struct heredoc *heredoc)
{
    struct heredoc *tmp;
    while (heredoc) {
        tmp = heredoc;
        heredoc = heredoc->next;
        // free(tmp->argv);
        // tmp->argv = NULL;
        free(tmp);
        tmp = NULL;
    }
}

void cleanup(t_main *main)
{
    if (main->cmd) {
        freecmd(main->cmd, 0);// 0 here, i assumed this is called when no errors in building the tree
    }
    if (main->heredoc) {
        freeheredoc(main->heredoc);
    }
}
