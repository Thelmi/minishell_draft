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

void free_double_pointer(char **str)
{
    int i;
	
    i = 0;
    while (str && str[i])
    {
        free(str[i]);
        str[i] = NULL;
        i++;
    }
    if (str)
        free(str);
    str = NULL;
}

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
        // //env->ev
        // if (check == 0)
        // {
        //     while (env && env->ev && env->ev[i])
        //     {
        //         free(env->ev[i]);
        //         env->ev[i] = NULL;
        //         i++;
        //     }
        //     if (env && env->ev)
        //         free(env->ev);
        //     env->ev = NULL;
        // }
		free(env);
        env = NULL;
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

void free_exp_node(t_export *node)
{
    if (node)
    {
        free(node->variable);
        free(node->value);
        // if (node->ev)
        //     free_double_pointer(node->ev);
        // node->ev = NULL;
        free(node);
    }
}

void free_env_node(t_env *node)
{
    if (node)
    {
        free(node->variable);
        free(node->value);
        // if (node->ev)
        //     free_double_pointer(node->ev);
        node->ev = NULL;
        free(node);
    }
}

char	**free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
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
            {
                free(ecmd->argv[i]);
            }
            ecmd->argv[i] = NULL;
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
        cmd = NULL;
    } else if (cmd->type == REDIR) {
        rcmd = (struct redircmd *)cmd;
        freecmd(rcmd->cmd, x);
        free(cmd);
        cmd = NULL;
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
