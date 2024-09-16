/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_with_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:10:03 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/11 20:45:30 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int num_strncmp(char *s1, char* s2)
{
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return s1[i] - s2[i];
}

int is_valid_identifier(const char *str)
{
    int i;

	i = 0;
    if (!str || str[i] == '\0')
        return 0;
    if (!(isalpha(str[i]) || str[i] == '_')) // libft function
        return 0;
    i++;
    while (str[i] != '\0')
    {
        if (!(isalnum(str[i]) || str[i] == '_')) // libft function
            return 0;
        i++;
    }

    return 1;
}

void update_env(t_env **env, char *variable, char *value, int *last_exit_status)
{
    t_env *tmp;
    t_env *new_node;

    tmp = *env;
    while (tmp != NULL)
    {
        if (num_strncmp(tmp->variable, variable) == 0)
        {
            free(tmp->value);
            if (value != NULL)
            {
                tmp->value = value;
            }
            else
            {
                tmp->value = malloc(1);
                if (tmp->value == NULL)
                {
                    perror("Error allocating memory for empty value");
                    *last_exit_status = 1;
                    return;
                }
                tmp->value[0] = '\0';
            }
            free(variable);
            return;
        }
        tmp = tmp->next;
    }
    if (value == NULL)
    {
        value = strdup("");
        if (value == NULL)
        {
            perror("Error allocating memory for empty value");
            free(variable);
            *last_exit_status = 1;
            return;
        }
    }
    new_node = create_env_nodes(variable, value);
    if (new_node != NULL)
    {
        new_node->next = *env;
        *env = new_node;
    }
    else
    {
        perror("Error allocating memory for new environment variable");
        free(variable);
        free(value);
        *last_exit_status = 1;
    }
}

void update_export(t_export **export, char *variable, int *last_exit_status)
{
    t_export *tmp;
    t_export *new_node;

    tmp = *export;
    while (tmp != NULL)
    {
        if (num_strncmp(tmp->variable, variable) == 0)
        {
            free(variable);
            return;
        }
        tmp = tmp->next;
    }

    new_node = create_export_nodes(variable, NULL);
    if (new_node != NULL)
    {
        new_node->next = *export;
        *export = new_node;
    }
    else
    {
        perror("Error allocating memory for new export variable");
        free(variable);
        *last_exit_status = 1;
    }
}

void export_with_args(t_env **env, t_export **export, int ac, char **av, int *last_exit_status)
{
    int i = 1;
    char *variable;
    char *value;
	char *export_var;

    while (i < ac)
    {
        if (strchr(av[i], '=') == NULL)
        {
            if (!is_valid_identifier(av[i]))
            {
				write(2, "export: ", 9);
				write(2, av[i], strlen(av[i]));
				write(2, ": not a valid identifier\n", 26);
                *last_exit_status = 1;
                i++;
                continue;
            }
            variable = malloc(strlen(av[i]) + 1);
            if (!variable)
            {
                perror("Error allocating memory for variable");
                *last_exit_status = 1;
                return;
            }
            strcpy(variable, av[i]); // libft function
            update_export(export, variable, last_exit_status);
        }
        else
        {
            variable = substr_before_char(av[i], '=');
            value = substr_after_char(av[i], '=');
            if (variable == NULL || !*variable || !is_valid_identifier(variable))
            {
                //printf("export: `%s': not a valid identifier\n", av[i]);
				write(2, "export: ", 8);
				write(2, av[i], strlen(av[i]));
				write(2, ": not a valid identifier\n", 26);
                free(variable);
                free(value);
				*last_exit_status = 1;
            }
            else
            {
                update_env(env, variable, value, last_exit_status);
                export_var = malloc(strlen(variable) + 1);
                if (!export_var)
                {
                  perror("Error allocating memory for export variable");
                  *last_exit_status = 1;
				  return;
                }
                strcpy(export_var, variable); // libft function
                update_export(export, export_var, last_exit_status);
            }
        }
        i++;
    }
}
