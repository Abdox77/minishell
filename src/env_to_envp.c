// // /* ************************************************************************** */
// // /*                                                                            */
// // /*                                                        :::      ::::::::   */
// // /*   env_to_envp.c                                      :+:      :+:    :+:   */
// // /*                                                    +:+ +:+         +:+     */
// // /*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
// // /*                                                +#+#+#+#+#+   +#+           */
// // /*   Created: 2024/04/14 20:08:31 by aabou-ib          #+#    #+#             */
// // /*   Updated: 2024/05/10 14:33:07 by aabou-ib         ###   ########.fr       */
// // /*                                                                            */
// // /* ************************************************************************** */

// #include "minishell.h"

// // // char	**env_to_envp(t_exec *exec)
// // // {
// // // 	t_env	*tmp;
// // // 	int		i;
// // // 	char	**arr;

// // //     i = 0;
// // // 	tmp = exec->env;
// // // 	while (tmp)
// // // 	{
// // // 		tmp = tmp->next;
// // // 		i++;
// // // 	}
// // // 	tmp = exec->env;
// // // 	arr = malloc((sizeof(char *) * (i + 1)));
// // // 	if (!arr)	
// // // 		return (NULL);
// // // 	i = 0;
// // // 	while (tmp)
// // // 	{
// // // 		arr[i] = ft_strjoin(tmp->key, "=");
// // // 		arr[i] = ft_strjoin(arr[i], tmp->value);
// // // 		tmp = tmp->next;
// // // 		i++;
// // // 	}
// // // 	arr[i] = NULL;
// // // 	return (arr);
// // // }

// // // char **env_to_envp(t_exec *exec)
// // // {
// // //     t_env *tmp;
// // //     int i;
// // //     char **arr;

// // //     i = 0;
// // //     tmp = exec->env;
// // //     while (tmp)
// // // 	{
// // //         tmp = tmp->next;
// // //         i++;
// // //     }
// // //     tmp = exec->env;
// // //     arr = (char **)malloc(sizeof(char *) * (i + 1));
// // //     if (!arr)
// // //         return NULL;
// // //     i = 0;
// // //     while (tmp)
// // // 	{
// // //         arr[i] = ft_strjoin(tmp->key, "=");
// // //         if (!arr[i])
// // // 		{
// // //             free_strs(arr);
// // //             return NULL;
// // //         }
// // //         char *joined_value = ft_strjoin(arr[i], tmp->value);
// // //         free(arr[i]);
// // //         if (!joined_value)
// // // 		{
// // //             free(arr[i]);
// // //             free_strs(arr);
// // // 			free(joined_value);
// // //             return NULL;
// // //         }
// // //         arr[i] = joined_value;
// // //         tmp = tmp->next;
// // //         i++;
// // //     }
// // //     arr[i] = NULL;
// // //     return (arr);
// // // }
// // char **env_to_envp(t_exec *exec) {
// //     t_env *tmp;
// //     int i;
// //     char **arr;
// //     char *tmp_str;

// //     i = 0;
// //     tmp = exec->env;
// //     while (tmp) {
// //         tmp = tmp->next;
// //         i++;
// //     }
// //     tmp = exec->env;
// //     arr = (char **)malloc(sizeof(char *) * (i + 1));
// //     if (!arr)
// //         return NULL;
// //     i = 0;
// //     while (tmp) {
// //         arr[i] = ft_strjoin(tmp->key, "=");
// //         if (!arr[i]) {
// //             free_strs(arr);
// //             return NULL;
// //         }
// //         tmp_str = ft_strjoin(arr[i], tmp->value);
// //         free(arr[i]);
// //         if (!tmp_str) {
// //             free_strs(arr);
// //             return NULL;
// //         }
// //         arr[i] = tmp_str;
// //         tmp = tmp->next;
// //         i++;
// //     }
// //     arr[i] = NULL;
// //     return arr;
// // }

// // char **env_to_envp(t_exec *exec) {
// //     t_env *tmp;
// //     int i;
// //     char **arr;
// //     char *tmp_str;

// //     i = 0;
// //     tmp = exec->env;
// //     while (tmp) {
// //         tmp = tmp->next;
// //         i++;
// //     }
// //     tmp = exec->env;
// //     arr = (char **)malloc(sizeof(char *) * (i + 1));
// //     if (!arr)
// //         return NULL;
// //     i = 0;
// //     while (tmp) {
// //         arr[i] = ft_strjoin(tmp->key, "=");
// //         if (!arr[i]) {
// //             free_strs(arr);
// //             return NULL;
// //         }
// //         tmp_str = ft_strjoin(arr[i], tmp->value);
// //         free(arr[i]);
// //         if (!tmp_str) {
// //             free_strs(arr);
// //             return NULL;
// //         }
// //         arr[i] = tmp_str;
// //         tmp = tmp->next;
// //         i++;
// //     }
// //     arr[i] = NULL;
// //     return arr;
// // }

// // char **env_to_envp(t_exec *exec) {
// //     t_env *tmp;
// //     int i;
// //     char **arr;
// //     char *tmp_str;

// //     i = 0;
// //     tmp = exec->env;
// //     while (tmp) {
// //         tmp = tmp->next;
// //         i++;
// //     }

// //     arr = (char **)malloc(sizeof(char *) * (i + 1));
// //     if (!arr)
// //         return NULL;

// //     i = 0;
// //     tmp = exec->env;
// //     while (tmp)
// //     {
// //         arr[i] = ft_strjoin(tmp->key, "=");
// //         if (!arr[i]) {
// //             free_strs(arr);
// //             return NULL;
// //         }
// //         tmp_str = ft_strjoin(arr[i], tmp->value);
// //         free(arr[i]);  // Free the previous allocation to avoid memory leak
// //         if (!tmp_str) {
// //             free_strs(arr);
// //             return NULL;
// //         }
// //         arr[i] = strdup(tmp_str);
// //         free(tmp_str);
// //         tmp = tmp->next;
// //         i++;
// //     }
// //     arr[i] = NULL;
// //     return arr;
// // }

// char **env_to_envp(t_exec *exec) {
//     t_env *tmp;
//     int i;
//     char **arr;
//     char *tmp_str;

//     i = 0;
//     tmp = exec->env;
//     while (tmp) {
//         tmp = tmp->next;
//         i++;
//     }
//     tmp = exec->env;
//     arr = (char **)malloc(sizeof(char *) * (i + 1));
//     if (!arr)
//         return NULL;
//     i = 0;
//     while (tmp) {
//         arr[i] = ft_strjoin(tmp->key, "=");
//         if (!arr[i]) {
//             free_strs(arr);
//             return NULL;
//         }
//         tmp_str = ft_strjoin(arr[i], tmp->value);
//         free(arr[i]);
//         if (!tmp_str) {
//             free_strs(arr);
//             return NULL;
//         }
//         arr[i] = tmp_str;
//         tmp = tmp->next;
//         i++;
//     }
//     arr[i] = NULL;
//     return arr;
// }

#include "minishell.h"

static int count_env_vars(t_exec *exec)
{
    t_env *tmp;
    int count;

    count = 0;
    tmp = exec->env;
    while (tmp)
    {
        tmp = tmp->next;
        count++;
    }
    return (count);
}

static char *create_env_var_string(t_env *env)
{
    char *env_var;
    char *tmp_str;

    env_var = ft_strjoin(env->key, "=");
    if (!env_var)
        return (NULL);
    tmp_str = ft_strjoin(env_var, env->value);
    free(env_var);
    return (tmp_str);
}

char **env_to_envp(t_exec *exec)
{
    t_env *tmp;
    int i;
    char **arr;

    arr = (char **)malloc(sizeof(char *) * (count_env_vars(exec) + 1));
    if (!arr)
        return (NULL);
    i = 0;
    tmp = exec->env;
    while (tmp)
    {
        arr[i] = create_env_var_string(tmp);
        if (!arr[i])
        {
            free_strs(arr);
            return (NULL);
        }
        tmp = tmp->next;
        i++;
    }
    arr[i] = NULL;
    return (arr);
}
