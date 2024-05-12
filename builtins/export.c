/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:13:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/12 13:55:00 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

static void ft_lstadd_back_env(t_env **head, t_env *new_node)
{
    if (!new_node) {
        return;
    }

    // If the list is empty, the new node becomes the head
    if (*head == NULL) {
        *head = new_node;
    } else {
        // Otherwise, traverse to the end of the list and add the new node
        struct s_env *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Function to validate and extract the key from the input string
static char *check_and_get_key(const char *p)
{
    if (p == NULL) {
        return NULL;
    }

    int i = 1;
    // Traverse the input string to find '=' and check for valid characters
    if (!ft_isalpha(p[0]) || p[0] != '_')
    {
        printf("%s: not a valid identifier\n", p);
        return NULL;
    }
    while (p[i] && p[i] != '=')
    {
        if (!ft_isalnum(p[i]) && p[i] != '_')
        {
            printf("%s: not a valid identifier\n", p);
            return NULL;
        }
        i++;
    }

    // Allocate memory for the key and copy it
    char *key = (char *)malloc((i + 1) * sizeof(char));
    if (!key) {
        perror("malloc");
        return NULL;
    }

    strncpy(key, p, i);
    key[i] = '\0'; // Null-terminate the key
    return key;
}


// Function to extract the value from the input string
static char *search_for_value(const char *p)
{
    if (p == NULL) {
        return NULL;
    }

    // Find the index of '='
    const char *equal_sign = strchr(p, '=');
    if (!equal_sign) {
        // No '=' found, return NULL
        return NULL;
    }

    // Allocate memory for the value and copy it
    const char *value_start = equal_sign + 1;
    char *value = (char *)malloc((strlen(value_start) + 1) * sizeof(char));
    if (!value) {
        perror("malloc");
        return NULL;
    }

    strcpy(value, value_start);
    return value;
}

// Function to search for a key in the linked list
static int search_key(t_env *head, const char *key)
{
    if (key == NULL) {
        return -1;
    }

    // Traverse the linked list and check each node's key
    struct s_env *current = head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return 1; // Key found
        }
        current = current->next;
    }
    return 0; // Key not found
}

static void	swap(t_env *tmp)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = tmp->key;
	tmp->key = tmp->next->key;
	tmp->next->key = temp_key;
	temp_value = tmp->value;
	tmp->value = tmp->next->value;
	tmp->next->value = temp_value;
}

static t_env	*sort_export(t_env *env)
{
	t_env	*tmp;
	// char	*temp_key;
	// char	*temp_value;
	int		swapped;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		tmp = env;
		while (tmp->next)
		{
			if (ft_strcmp(tmp->key, tmp->next->key) > 0)
			{
				swap(tmp);
				swapped = 1;
			}
			tmp = tmp->next;
		}
	}
	return (env);
}

static void	print_export(t_env *env)
{
	t_env	*tmp;

	tmp = sort_export(env);
	while (tmp)
	{
		if (!tmp->value)
			printf("declare -x %s\n", tmp->key);
		else
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

int ft_export(t_env *env, char **args)
{
    if (env == NULL || args == NULL)
        return -1; // Return an error code for NULL input
    else if (args[0] == NULL) // i need to change it to !args[1] but the 2d array now doesn't have the command as the first argument
    {
        print_export(env);
        return (1);
    }
    int i = 0;
    while (args[i])
    {
        const char *arg = args[i];

        char *key = check_and_get_key(arg);
        if (key == NULL)
        {
            i++;
            continue; // Skip to the next argument if key is NULL
        }
        int exists = search_key(env, key);

        if (exists == 0)
        {
            struct s_env *new_node = (struct s_env *)malloc(sizeof(struct s_env));
            if (new_node == NULL)
            {
                perror("malloc");
                free(key); 
                return -1;
            }

            new_node->key = key;
            new_node->value = search_for_value(arg);
            new_node->next = NULL;
            ft_lstadd_back_env(&env, new_node);
        }
        else if (exists == 1)
        {
            struct s_env *current = env;
            while (current != NULL)
            {
                if (strcmp(current->key, key) == 0)
                {
                    free(current->value);
                    current->value = search_for_value(arg);
                    break;
                }
                current = current->next;
            }
            free(key); // free the allocated key as it is no longer needed
        }
        i++;
    }

    return 1; // Return success
}
