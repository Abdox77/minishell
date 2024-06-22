// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   export.c                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: aminosar <aminosar@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/04/10 08:13:01 by amohdi            #+#    #+#             */
// /*   Updated: 2024/06/17 02:46:45 by aminosar         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../src/minishell.h"

// static void ft_lstadd_back_env(t_env **head, t_env *new_node)
// {
//     if (!new_node)
//         return;
//     if (*head == NULL)
//         *head = new_node;
//     else
//     {
//         struct s_env *current = *head;
//         while (current->next != NULL)
//             current = current->next;
//         current->next = new_node;
//     }
// }

// static char	*ft_strncpy(char *dest, char *src, unsigned int n)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (src[i] != '\0' && i < n)
// 	{
// 		dest[i] = src[i];
// 		++i;
// 	}
// 	while (i < n)
// 	{
// 		dest[i] = '\0';
// 		i++;
// 	}
// 	return (dest);
// }

// // Function to validate and extract the key from the input string
// // static char *check_and_get_key(const char *p)
// // {
// //     if (p == NULL)
// //         return NULL;
// //     int i = 1;
// //     // Traverse the input string to find '=' and check for valid characters
// //     if (!ft_isalpha(p[0]) && p[0] != '_')
// //     {
// //         printf("%s: not a valid identifier\n", p);
// //         return NULL;
// //     }
// //     while (p[i] && p[i] != '=')
// //     {
// //         if (!ft_isalnum(p[i]) && p[i] != '_')
// //         {
// //             printf("%s: not a valid identifier\n", p);
// //             return NULL;
// //         }
// //         i++;
// //     }

// //     // Allocate memory for the key and copy it
// //     char *key = (char *)malloc((i + 1) * sizeof(char));
// //     if (!key)
// //     {
// //         perror("malloc");
// //         return NULL;
// //     }

// //     ft_strncpy(key, (char *)p, i);
// //     key[i] = '\0'; // null terminate the key
// //     return key;
// // }


// // // Function to extract the value from the input string
// // static char *search_for_value(const char *p)
// // {
// //     if (p == NULL) {
// //         return NULL;
// //     }

// //     // Find the index of '='
// //     const char *equal_sign = ft_strchr(p, '=');
// //     if (!equal_sign) {
// //         // No '=' found, return NULL
// //         return NULL;
// //     }

// //     // Allocate memory for the value and copy it
// //     const char *value_start = equal_sign + 1;
// //     char *value = (char *)malloc((strlen(value_start) + 1) * sizeof(char));
// //     if (!value) {
// //         perror("malloc");
// //         return NULL;
// //     }

// //     strcpy(value, value_start);
// //     return value;
// // }

// // // Function to search for a key in the linked list
// // static int search_key(t_env *head, const char *key)
// // {
// //     if (key == NULL) {
// //         return -1;
// //     }

// //     // Traverse the linked list and check each node's key
// //     struct s_env *current = head;
// //     while (current != NULL) {
// //         if (strcmp(current->key, key) == 0) {
// //             return 1; // Key found
// //         }
// //         current = current->next;
// //     }
// //     return 0; // Key not found
// // }

// // static void	swap(t_env *tmp)
// // {
// // 	char	*temp_key;
// // 	char	*temp_value;

// // 	temp_key = tmp->key;
// // 	tmp->key = tmp->next->key;
// // 	tmp->next->key = temp_key;
// // 	temp_value = tmp->value;
// // 	tmp->value = tmp->next->value;
// // 	tmp->next->value = temp_value;
// // }

// // static t_env	*sort_export(t_env *env)
// // {
// // 	t_env	*tmp;
// // 	// char	*temp_key;
// // 	// char	*temp_value;
// // 	int		swapped;

// // 	swapped = 1;
// // 	while (swapped)
// // 	{
// // 		swapped = 0;
// // 		tmp = env;
// // 		while (tmp->next)
// // 		{
// // 			if (ft_strcmp(tmp->key, tmp->next->key) > 0)
// // 			{
// // 				swap(tmp);
// // 				swapped = 1;
// // 			}
// // 			tmp = tmp->next;
// // 		}
// // 	}
// // 	return (env);
// // }

// // static void	print_export(t_env *env)
// // {
// // 	t_env	*tmp;

// // 	tmp = sort_export(env);
// // 	while (tmp)
// // 	{
// // 		if (!tmp->value)
// // 			printf("declare -x %s\n", tmp->key);
// // 		else
// // 			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
// // 		tmp = tmp->next;
// // 	}
// // }

// // int ft_export(t_env *env, char **args)
// // {
// //     if (env == NULL || args == NULL)
// //         return -1; // Return an error code for NULL input
// //     else if (args[1] == NULL) // i need to change it to !args[1] but the 2d array now doesn't have the command as the first argument
// //     {
// //         print_export(env);
// //         return (1);
// //     }
// //     int i = 1;
// //     while (args[i])
// //     {
// //         const char *arg = args[i];

// //         char *key = check_and_get_key(arg);
// //         if (key == NULL)
// //         {
// //             i++;
// //             continue; // Skip to the next argument if key is NULL
// //         }
// //         int exists = search_key(env, key);

// //         if (exists == 0)
// //         {
// //             struct s_env *new_node = (struct s_env *)malloc(sizeof(struct s_env));
// //             if (new_node == NULL)
// //             {
// //                 perror("malloc");
// //                 free(key); 
// //                 return -1;
// //             }

// //             new_node->key = key;
// //             new_node->value = search_for_value(arg);
// //             new_node->next = NULL;
// //             ft_lstadd_back_env(&env, new_node);
// //         }
// //         else if (exists == 1)
// //         {
// //             struct s_env *current = env;
// //             while (current != NULL)
// //             {
// //                 if (strcmp(current->key, key) == 0)
// //                 {
// //                     free(current->value);
// //                     current->value = search_for_value(arg);
// //                     break;
// //                 }
// //                 current = current->next;
// //             }
// //             free(key); // free the allocated key as it is no longer needed
// //         }
// //         i++;
// //     }

// //     return 1; // Return success
// // }
// // static char *check_and_get_key(const char *p) {
// //     if (p == NULL)
// //         return NULL;
// //     int i = 0;
// //     // Traverse the input string to find '=' and check for valid characters
// //     if (!isalpha((unsigned char)p[i]) && p[i] != '_') {
// //         printf("%s: not a valid identifier\n", p);
// //         return NULL;
// //     }
// //     while (p[i] && p[i] != '=') {
// //         if (!isalnum((unsigned char)p[i]) && p[i] != '_') {
// //             printf("%s: not a valid identifier\n", p);
// //             return NULL;
// //         }
// //         i++;
// //     }

// //     // Allocate memory for the key and copy it
// //     char *key = (char *)malloc((i + 1) * sizeof(char));
// //     if (!key) {
// //         perror("malloc");
// //         return NULL;
// //     }

// //     ft_strncpy(key, (char *)p, i);
// //     key[i] = '\0'; // null terminate the key
// //     return key;
// // }

// // static char *search_for_value(const char *p) {
// //     if (p == NULL) {
// //         return NULL;
// //     }

// //     // Find the index of '='
// //     const char *equal_sign = strchr(p, '=');
// //     if (!equal_sign) {
// //         // No '=' found, return NULL
// //         return NULL;
// //     }

// //     // Allocate memory for the value and copy it
// //     const char *value_start = equal_sign + 1;
// //     char *value = (char *)malloc((strlen(value_start) + 1) * sizeof(char));
// //     if (!value) {
// //         perror("malloc");
// //         return NULL;
// //     }

// //     strcpy(value, value_start);
// //     return value;
// // }

// // static int search_key(t_env *head, const char *key) {
// //     if (key == NULL) {
// //         return -1;
// //     }

// //     t_env *current = head;
// //     while (current != NULL) {
// //         if (strcmp(current->key, key) == 0) {
// //             return 1; // Key found
// //         }
// //         current = current->next;
// //     }
// //     return 0; // Key not found
// // }

// // static void swap(t_env *tmp) {
// //     char *temp_key;
// //     char *temp_value;

// //     temp_key = tmp->key;
// //     tmp->key = tmp->next->key;
// //     tmp->next->key = temp_key;
// //     temp_value = tmp->value;
// //     tmp->value = tmp->next->value;
// //     tmp->next->value = temp_value;
// // }

// // static t_env *sort_export(t_env *env) {
// //     t_env *tmp;
// //     int swapped;

// //     swapped = 1;
// //     while (swapped) {
// //         swapped = 0;
// //         tmp = env;
// //         while (tmp->next) {
// //             if (strcmp(tmp->key, tmp->next->key) > 0) {
// //                 swap(tmp);
// //                 swapped = 1;
// //             }
// //             tmp = tmp->next;
// //         }
// //     }
// //     return env;
// // }

// // static void print_export(t_env *env) {
// //     t_env *tmp;

// //     tmp = sort_export(env);
// //     while (tmp) {
// //         if (!tmp->value)
// //             printf("declare -x %s\n", tmp->key);
// //         else
// //             printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
// //         tmp = tmp->next;
// //     }
// // }

// // int ft_export(t_env *env, char **args) {
// //     if (env == NULL || args == NULL)
// //         return -1; // Return an error code for NULL input
// //     else if (args[1] == NULL) {
// //         print_export(env);
// //         return 1;
// //     }
// //     int i = 1;
// //     while (args[i]) {
// //         const char *arg = args[i];

// //         char *key = check_and_get_key(arg);
// //         if (key == NULL) {
// //             i++;
// //             continue; // Skip to the next argument if key is NULL
// //         }
// //         int exists = search_key(env, key);

// //         if (exists == 0) {
// //             t_env *new_node = (t_env *)malloc(sizeof(t_env));
// //             if (new_node == NULL) {
// //                 perror("malloc");
// //                 free(key);
// //                 return -1;
// //             }

// //             new_node->key = key;
// //             new_node->value = search_for_value(arg);
// //             new_node->next = NULL;
// //             ft_lstadd_back_env(&env, new_node);
// //         } else if (exists == 1) {
// //             t_env *current = env;
// //             while (current != NULL) {
// //                 if (strcmp(current->key, key) == 0) {
// //                     char *new_value = search_for_value(arg);
// //                     if (new_value) {
// //                         free(current->value);
// //                         current->value = new_value;
// //                     }
// //                     break;
// //                 }
// //                 current = current->next;
// //             }
// //             free(key); // free the allocated key as it is no longer needed
// //         }
// //         i++;
// //     }

// //     return 1; // Return success
// // }
// static char *check_and_get_key(const char *p)
// {
//     if (p == NULL)
//         return NULL;
//     int i = 0;
//     if (!isalpha((unsigned char)p[i]) && p[i] != '_')
//         return (printf("%s: not a valid identifier\n", p), NULL);
//     while (p[i] && p[i] != '=' && !(p[i] == '+' && p[i + 1] == '='))
//     {
//         if (!isalnum((unsigned char)p[i]) && p[i] != '_')
//         {
//             printf("%s: not a valid identifier\n", p);
//             return NULL;
//         }
//         i++;
//     }

//     char *key = (char *)malloc((i + 1) * sizeof(char));
//     if (!key)
//     {
//         perror("malloc");
//         return NULL;
//     }

//     ft_strncpy(key, (char *)p, i);
//     key[i] = '\0';
//     return key;
// }

// static char *search_for_value(const char *p)
// {
//     const char *equal_sign;
    
//     if (p == NULL)
//         return NULL;
//     equal_sign = strchr(p, '=');
//     if (!equal_sign)
//         return NULL;

//     const char *value_start = equal_sign + 1;
//     char *value = (char *)malloc((strlen(value_start) + 1) * sizeof(char));
//     if (!value)
//     {
//         perror("malloc");
//         return NULL;
//     }
//     strcpy(value, value_start);
//     return value;
// }

// static t_env *search_key(t_env *head, const char *key)
// {
//     t_env *current;
//     if (key == NULL)
//         return NULL;
//     current = head;
//     while (current != NULL)
//     {
//         if (ft_strcmp(current->key, key) == 0)
//         {
//             return current; // Return the node itself
//         }
//         current = current->next;
//     }
//     return NULL; // Key not found
// }

// static void swap(t_env *tmp)
// {
//     char *temp_key;
//     char *temp_value;

//     temp_key = tmp->key;
//     tmp->key = tmp->next->key;
//     tmp->next->key = temp_key;
//     temp_value = tmp->value;
//     tmp->value = tmp->next->value;
//     tmp->next->value = temp_value;
// }

// static t_env *sort_export(t_env *env)
// {
//     t_env *tmp;
//     int swapped;

//     swapped = 1;
//     while (swapped)
//     {
//         swapped = 0;
//         tmp = env;
//         while (tmp->next)
//         {
//             if (strcmp(tmp->key, tmp->next->key) > 0)
//             {
//                 swap(tmp);
//                 swapped = 1;
//             }
//             tmp = tmp->next;
//         }
//     }
//     return env;
// }

// static void print_export(t_env *env)
// {
//     t_env *tmp;

//     tmp = sort_export(env);
//     while (tmp)
//     {
//         if (!tmp->value)
//             printf("declare -x %s\n", tmp->key);
//         else
//             printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
//         tmp = tmp->next;
//     }
// }

// int ft_export(t_env *env, char **args)
// {
//     if (env == NULL || args == NULL)
//         return -1;
//     else if (args[1] == NULL)
//     {
//         print_export(env);
//         return 1;
//     }

//     int i = 1;
//     while (args[i])
//     {
//         const char *arg = args[i];
//         char *key = check_and_get_key(arg);
//         if (key == NULL)
//         {
//             i++;
//             continue;
//         }

//         t_env *existing_node = search_key(env, key);
//         if (existing_node == NULL)
//         {
//             t_env *new_node = (t_env *)malloc(sizeof(t_env));
//             if (new_node == NULL)
//             {
//                 perror("malloc");
//                 free(key);
//                 return -1;
//             }

//             new_node->key = key;
//             new_node->value = search_for_value(arg);
//             new_node->next = NULL;
//             ft_lstadd_back_env(&env, new_node);
//         }
//         else
//         {
//             const char *append_sign = strstr(arg, "+=");
//             if (append_sign)
//             {
//                 char *new_value = search_for_value(arg);
//                 if (new_value)
//                 {
//                     char *concatenated_value = ft_strjoin(existing_node->value, new_value);
//                     free(existing_node->value);
//                     free(new_value);
//                     existing_node->value = concatenated_value;
//                 }
//             }
//             else
//             {
//                 char *new_value = search_for_value(arg);
//                 if (new_value)
//                 {
//                     free(existing_node->value);
//                     existing_node->value = new_value;
//                 }
//             }
//             free(key);
//         }
//         i++;
//     }

//     return 1;
// }

#include "../src/minishell.h"

static void	ft_lstadd_back_env(t_env **head, t_env *new_node)
{
	t_env *current;

	if (!new_node)
		return;
	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

static char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

static char	*check_and_get_key(const char *p)
{
	int		i;
	char	*key;

	if (p == NULL)
		return (NULL);
	i = 0;
	if (!ft_isalpha((unsigned char)p[i]) && p[i] != '_')
		return (printf("%s: not a valid identifier\n", p), NULL);
	while (p[i] && p[i] != '=' && !(p[i] == '+' && p[i + 1] == '='))
	{
		if (!ft_isalnum((unsigned char)p[i]) && p[i] != '_')
		{
			printf("%s: not a valid identifier\n", p);
			return (NULL);
		}
		i++;
	}
	key = (char *)malloc((i + 1) * sizeof(char));
	if (!key)
		return (perror("malloc"), NULL);
	ft_strncpy(key, p, i);
	key[i] = '\0';
	return (key);
}

static char	*search_for_value(const char *p)
{
	const char	*equal_sign;
	const char	*value_start;
	char		*value;

	if (p == NULL)
		return (NULL);
	equal_sign = strchr(p, '=');
	if (!equal_sign)
		return (NULL);
	value_start = equal_sign + 1;
	value = (char *)malloc((strlen(value_start) + 1) * sizeof(char));
	if (!value)
	{
		perror("malloc");
		return (NULL);
	}
	strcpy(value, value_start);
	return (value);
}

static t_env	*search_key(t_env *head, const char *key)
{
	t_env	*current;

	if (key == NULL)
		return (NULL);
	current = head;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
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
	int		swapped;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		tmp = env;
		while (tmp->next)
		{
			if (strcmp(tmp->key, tmp->next->key) > 0)
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

static char	*ft_j_strcat(char *s1, const char *s2, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*p;
	size_t	i;
	size_t	l1;
	size_t	l2;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		return (ft_strdup(s2));
	else if (!s2 && s1)
		return (ft_strdup(s1));
	i = 0;
	l1 = ft_strlen((char *)s1);
	l2 = ft_strlen((char *)s2);
	p = (char *)malloc(sizeof(char) * (l1 + l2 + 1));
	if (!p)
		return (NULL);
	while (i < l1)
	{
		p[i] = ((char *)s1)[i];
		i++;
	}
	ft_j_strcat((p + i), s2, l2);
	return (p);
}

static int	handle_new_variable(t_env **env, const char *arg, char *key)
{
	t_env *new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node == NULL)
	{
		perror("malloc");
		free(key);
		return (-1);
	}
	new_node->key = key;
	new_node->value = search_for_value(arg);
	new_node->next = NULL;
	ft_lstadd_back_env(env, new_node);
	return (1);
}

static void	handle_existing_variable(t_env *existing_node, const char *arg)
{
	char	*new_value;
	char	*concatenated_value;

	if (strstr(arg, "+="))
	{
		new_value = search_for_value(arg);
		if (new_value)
		{
			concatenated_value = ft_strjoin(existing_node->value, new_value);
			free(existing_node->value);
			free(new_value);
			existing_node->value = concatenated_value;
		}
	}
	else
	{
		new_value = search_for_value(arg);
		if (new_value)
		{
			free(existing_node->value);
			existing_node->value = new_value;
		}
	}
}

// int	ft_export(t_env *env, char **args)
// {
// 	int		i;
// 	char	*key;
// 	t_env	*existing_node;

// 	if (env == NULL || args == NULL)
// 		return (-1);
// 	else if (args[1] == NULL)
// 		return (print_export(env), 1);
// 	i = 1;
// 	while (args[i])
// 	{
// 		key = check_and_get_key(args[i]);
// 		if (key == NULL)
// 		{
// 			i++;
// 			continue;
// 		}
// 		existing_node = search_key(env, key);
// 		if (existing_node == NULL)
// 			handle_new_variable(&env, args[i], key);
// 		else
// 		{
// 			handle_existing_variable(existing_node, args[i]);
// 			free(key);
// 		}
// 		i++;
// 	}
// 	return (1);
// }
int	ft_export(t_env *env, char **args)
{
	int		i;
	char	*key;
	t_env	*existing_node;

	if (!env || !args)
		return (-1);
	if (!args[1])
		return (print_export(env), 1);
	i = 1;
	while (args[i])
	{
		key = check_and_get_key(args[i]);
		if (!key)
			continue;
		existing_node = search_key(env, key);
		if (!existing_node)
			handle_new_variable(&env, args[i], key);
		else
		{
			handle_existing_variable(existing_node, args[i]);
			free(key);
		}
		i++;
	}
	return (1);
}
