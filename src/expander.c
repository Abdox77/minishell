// #include "minishell.h"

// static char *find_env_value(const char *var_name, t_env *env_list)
// {
//     while (env_list != NULL)
//     {
//         if (strcmp(var_name, env_list->key) == 0)
//             return env_list->value;
//         env_list = env_list->next;
//     }
//     return NULL;
// }

// static char *ft_strndup(const char *s1, size_t n)
// {
//     size_t i;
//     char *p;

//     i = 0;
//     size_t len = 0;
//     while (len < n && s1[len] != '\0')
//         len++;
//     p = (char *)malloc(sizeof(char) * (len + 1));
//     if (!p)
//         return (NULL);

//     while (i < len)
//     {
//         p[i] = s1[i];
//         i++;
//     }
//     p[i] = '\0';
//     return (p);
// }

// // static char	*ft_strcat(char *s1, const char *p, size_t len)
// // {
// // 	size_t	i;
// // 	char	*s2;

// // 	i = 0;
// // 	s2 = (char *)p;
// // 	while (s2[i])
// // 	{
// // 		s1[i] = s2[i];
// // 		i++;
// // 	}
// // 	s1[i] = '\0';
// // 	return (s1);
// // }

// //length of the expanded string
// static size_t get_expanded_length(const char *str, t_env *env_list)
// {
//     size_t length = 0;
//     while (*str)
//     {
//         if (*str == '$')
//         {
//             str++;
//             const char *var_start = str;
//             while (*str && (ft_isalnum(*str) || *str == '_'))
//                 str++;
//             size_t var_len = str - var_start;
//             char *var_name = ft_strndup(var_start, var_len);
//             char *var_value = find_env_value(var_name, env_list);
//             free(var_name);
//             length += ft_strlen(var_value);
//         }
//         else if (*str == '"')
//         {
//             str++;
//             while (*str && *str != '"')
//             {
//                 if (*str == '$')
//                 {
//                     str++;
//                     const char *var_start = str;
//                     while (*str && (ft_isalnum(*str) || *str == '_'))
//                         str++;
//                     size_t var_len = str - var_start;
//                     char *var_name = ft_strndup(var_start, var_len);
//                     char *var_value = find_env_value(var_name, env_list);
//                     free(var_name);
//                     length += ft_strlen(var_value);
//                 }
//                 else
//                 {
//                     length++;
//                     str++;
//                 }
//             }
//             if (*str == '"')
//                 str++;
//         }
//         else
//         {
//             length++;
//             str++;
//         }
//     }
//     return length;
// }

// //expand variables in a string
// char *expand_string(const char *str, t_env *env_list)
// {
//     size_t result_size = get_expanded_length(str, env_list) + 1;
//     char *result = (char *)malloc(result_size);
//     if (!result)
//     {
//         perror("malloc failed");
//         exit(EXIT_FAILURE);
//     }
//     result[0] = '\0';
//     char *result_ptr = result;

//     while (*str)
//     {
//         if (*str == '$')
//         {
//             str++;
//             const char *var_start = str;
//             while (*str && (ft_isalnum(*str) || *str == '_'))
//                 str++;
//             size_t var_len = str - var_start;
//             char *var_name = ft_strndup(var_start, var_len);
//             char *var_value = find_env_value(var_name, env_list);
//             free(var_name);
//             strcpy(result_ptr, var_value);
//             result_ptr += ft_strlen(var_value);
//         }
//         else if (*str == '"')
//         {
//             str++;
//             while (*str && *str != '"')
//             {
//                 if (*str == '$')
//                 {
//                     str++;
//                     const char *var_start = str;
//                     while (*str && (ft_isalnum(*str) || *str == '_'))
//                         str++;
//                     size_t var_len = str - var_start;
//                     char *var_name = ft_strndup(var_start, var_len);
//                     char *var_value = find_env_value(var_name, env_list);
//                     free(var_name);
//                     strcpy(result_ptr, var_value);
//                     result_ptr += ft_strlen(var_value);
//                 }
//                 else
//                     *result_ptr++ = *str++;
//             }
//             if (*str == '"')
//                 str++;
//         }
//         else
//             *result_ptr++ = *str++;
//     }
//     *result_ptr = '\0';
//     return result;
// }

// char *expand_arg_if_needed(char *arg, char *og_arg, t_env *env_list)
// {
//     if (!arg)
//         return NULL;
//     if (ft_strchr(arg, '$'))
//         return expand_string(og_arg, env_list); // Need to do this with og_args
//     return strdup(arg);
// }

// char **process_args(char **args, char **og_args, char *cmd, t_env *env_list)
// {
//     int count = 0;
//     while (args[count] != NULL)
//         count++;
//     char **processed_args = (char **)malloc((count + 1) * sizeof(char *));
//     if (processed_args == NULL)
//     {
//         perror("malloc failed");
//         exit(EXIT_FAILURE);
//     }

//     int i = 0;

//     processed_args[i++] = expand_string(cmd, env_list);

//     while (args[i] != NULL)
//     {
//         processed_args[i] = expand_arg_if_needed(args[i], og_args[i - 1], env_list); // need to do og_args[i-1];
//         i++;
//     }
//     processed_args[i] = NULL;

//     return processed_args;
// }