#include "minishell.h"

// static char *find_env_value(const char *var_name, t_env *env_list) {
//     while (env_list != NULL) {
//         if (strcmp(var_name, env_list->key) == 0) {
//             return env_list->value;
//         }
//         env_list = env_list->next;
//     }
//     return NULL;
// }

// static size_t get_expanded_length(const char *str, t_env *env_list)
// {
//     size_t length = 0;
//     int in_single_quotes = 0;
//     int in_double_quotes = 0;

//     while (*str) {
//         if (*str == '\'') {
//             in_single_quotes = !in_single_quotes;
//             str++;
//         } else if (*str == '"') {
//             in_double_quotes = !in_double_quotes;
//             str++;
//         } else if (*str == '$' && !in_single_quotes) {
//             str++;
//             const char *var_start = str;
//             while (*str && (isalnum(*str) || *str == '_')) {
//                 str++;
//             }
//             size_t var_len = str - var_start;
//             char *var_name = strndup(var_start, var_len);
//             char *var_value = find_env_value(var_name, env_list);
//             free(var_name);
//             if (var_value) {
//                 length += strlen(var_value);
//             }
//         } else {
//             length++;
//             str++;
//         }
//     }
//     return length;
// }

// // Function to expand environment variables in the input string
// char *expand_string(const char *str, t_env *env_list) {
//     size_t result_size = get_expanded_length(str, env_list) + 1;
//     char *result = (char *)malloc(result_size);
//     if (!result) {
//         perror("malloc failed");
//         exit(EXIT_FAILURE);
//     }
//     result[0] = '\0';
//     char *result_ptr = result;
//     int in_single_quotes = 0;
//     int in_double_quotes = 0;

//     while (*str) {
//         if (*str == '\'') {
//             in_single_quotes = !in_single_quotes;
//             str++;
//         } else if (*str == '"') {
//             in_double_quotes = !in_double_quotes;
//             str++;
//         } else if (*str == '$' && !in_single_quotes) {
//             str++;
//             const char *var_start = str;
//             while (*str && (isalnum(*str) || *str == '_')) {
//                 str++;
//             }
//             size_t var_len = str - var_start;
//             char *var_name = strndup(var_start, var_len);
//             char *var_value = find_env_value(var_name, env_list);
//             free(var_name);
//             if (var_value) {
//                 strcpy(result_ptr, var_value);
//                 result_ptr += strlen(var_value);
//             }
//         } else {
//             *result_ptr++ = *str++;
//         }
//     }
//     *result_ptr = '\0';
//     return result;
// }

// Function to split a string by spaces into an array of strings
static char **split_by_spaces(const char *str) {
    int count = 0;
    const char *temp = str;

    // Count the number of words
    while (*temp) {
        while (isspace((unsigned char)*temp)) temp++;
        if (*temp) {
            count++;
            while (*temp && !isspace((unsigned char)*temp)) temp++;
        }
    }

    // Allocate memory for the array of strings
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Split the string into words
    int i = 0;
    while (*str) {
        while (isspace((unsigned char)*str)) str++;
        if (*str) {
            const char *start = str;
            while (*str && !isspace((unsigned char)*str)) str++;
            size_t len = str - start;
            result[i] = (char *)malloc(len + 1);
            if (result[i] == NULL) {
                perror("malloc failed");
                exit(EXIT_FAILURE);
            }
            strncpy(result[i], start, len);
            result[i][len] = '\0';
            i++;
        }
    }
    result[i] = NULL;
    return result;
}

// Function to check if a variable is inside quotes in a string
static int is_inside_quotes(const char *str, const char *pos) {
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    while (str < pos) {
        if (*str == '\'' && !in_double_quotes) {
            in_single_quotes = !in_single_quotes;
        } else if (*str == '"' && !in_single_quotes) {
            in_double_quotes = !in_double_quotes;
        }
        str++;
    }
    return in_double_quotes;
}


// Function to expand a single argument if it contains a $
char **expand_arg_if_needed(char *arg, char *og_arg, t_env *env_list) {
        if (strchr(arg, '$')) {
            char *expanded = expand_string(og_arg, env_list);
            char **split_expanded;

            if (is_inside_quotes(og_arg, strchr(og_arg, '$'))) {
                // If the variable is inside quotes, do not split
                split_expanded = (char **)malloc(2 * sizeof(char *));
                if (split_expanded == NULL) {
                    perror("malloc failed");
                    exit(EXIT_FAILURE);
                }
                split_expanded[0] = expanded;
                split_expanded[1] = NULL;
            } else {
                // Otherwise, split by spaces
                split_expanded = split_by_spaces(expanded);
                free(expanded);
            }
            return split_expanded;
        }

        char **single_arg = (char **)malloc(2 * sizeof(char *));
        if (single_arg == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        single_arg[0] = strdup(arg);
        single_arg[1] = NULL;
        return single_arg;
}

// Function to concatenate two arrays of strings
static char **concat_args(char **args1, char **args2) {
    int len1 = 0;
    int len2 = 0;

    // Calculate the length of the first array
    while (args1[len1] != NULL)
        len1++;
    // Calculate the length of the second array
    while (args2[len2] != NULL)
        len2++;
    // Allocate memory for the concatenated array
    char **result = (char **)malloc((len1 + len2 + 1) * sizeof(char *));
    if (result == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Copy the elements of the first array
    int i = 0;
    while (i < len1) {
        result[i] = args1[i];
        i++;
    }
    // Copy the elements of the second array
    int j = 0;
    while (j < len2) {
        result[i + j] = args2[j];
        j++;
    }

    result[i + j] = NULL;
    return result;
}
// Function to process all arguments
char **process_args(char **args, char **og_args, char *og_cmd, char *cmd, t_env *env_list) {
    (void)cmd;
    // Allocate initial memory for the processed arguments array
    char **for_cmd = expand_arg_if_needed(cmd, og_cmd, env_list);
    int i =0;
    while(for_cmd[i])
        i++;
    char **processed_args = (char **)malloc(sizeof(char *) * (i + 1));
    if (processed_args == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    i = 0;
    while(for_cmd[i])
    {
        processed_args[i] = for_cmd[i];
        i++;
    }
    // processed_args[0] = 
    processed_args[i] = NULL;

    // Process each argument
    i = 1;
    while (args[i] != NULL) {
        char **expanded_args = expand_arg_if_needed(args[i], og_args[i - 1], env_list);
        char **new_processed_args = concat_args(processed_args, expanded_args);
        free(processed_args);
        free(expanded_args); // Free the expanded args after use
        processed_args = new_processed_args;
        i++;
    }

    return processed_args;
}

// this i smy expander
// Function to get a token in between quotes
// static char *get_token_in_between_quotes1(char **line, char quote, int *og_len) {
//     int i;
//     int len;
//     char *token;

//     i = -1;
//     len = 0;
//     if (!*line || !**line)
//         return NULL;
//     ++(*line);
//     ++(*og_len);
//     while ((*line)[len] && (*line)[len] != quote)
//         len++;
//     if (!(*line)[len]) {
//         if (0 == len && (*line)[len] == quote) {
//             ++(*line);
//             ++(*og_len);
//         }
//         return NULL;
//     }
//     token = (char *)malloc(sizeof(char) * (len + 1));
//     if (!token)
//         return NULL;
//     while (++i < len)
//         token[i] = (*line)[i];
//     token[i] = '\0';
//     *line += len;
//     *og_len += len;
//     return token;
// }

// // Function to get a token with quotes
// char *get_token_with_quotes1(char **line, int len, int *og_len) {
//     char quote;
//     char *arg;
//     char *buff;

//     arg = ft_substr(*line, 0, len);
//     (*line) += len;
//     (*og_len) += len;
//     if (!arg)
//         printf("problemo LEO\n"); // to edit later
//     buff = NULL;
//     while (**line && **line != '&' && **line != '(' && **line != ')' && **line != '|' && is_space(**line) == FALSE && is_redirection_char(**line) == FALSE) {
//         if (is_quote(**line) == FALSE && **line != '(' && **line != ')' && is_special_char(**line) == FALSE) {
//             len = 0;
//             while ((*line)[len] && is_special_char((*line)[len]) == FALSE)
//                 len++;
//             arg = ft_strjoin(arg, ft_substr(*line, 0, len));
//             *og_len += len;
//             *line += len;
//         } else if (is_quote(**line) == TRUE) {
//             quote = **line;
//             buff = get_token_in_between_quotes1(line, quote, og_len);
//             arg = ft_strjoin(arg, buff);
//             if (**line != quote)
//                 ft_print_error("Syntax error unclosed quotes\n", line, SAVE);
//             else {
//                 ++(*line);
//                 ++(*og_len);
//             }
//         }
//     }
//     return arg;
// }
// this is my expander but there's a problem there's an env called ? which when i do $? it should be expanded but at the same ? can't be in the name of any other variable so if var=hello and i do $var? it should return hello? (this works now o just add a special check for the first one)


static char *find_env_value(const char *var_name, t_env *env_list) {
    if (ft_strcmp(var_name, "?") == 0) {;
        return (ft_itoa(stat(0,0)));
    }

    while (env_list != NULL) {
        if (strcmp(var_name, env_list->key) == 0) {
            return env_list->value;
        }
        env_list = env_list->next;
    }
    return NULL;
}

static size_t get_expanded_length(const char *str, t_env *env_list) {
    size_t length = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    while (*str) {
        if (*str == '\'') {
            in_single_quotes = !in_single_quotes;
            str++;
        } else if (*str == '"') {
            in_double_quotes = !in_double_quotes;
            str++;
        } else if (*str == '$' && !in_single_quotes) {
            str++;
            const char *var_start = str;
            if (*str == '?') {
                length += strlen(find_env_value("?", env_list));
                str++;
            } else {
                while (*str && (isalnum(*str) || *str == '_')) {
                    str++;
                }
                size_t var_len = str - var_start;
                char *var_name = strndup(var_start, var_len);
                char *var_value = find_env_value(var_name, env_list);
                free(var_name);
                if (var_value) {
                    length += strlen(var_value);
                }
            }
        } else {
            length++;
            str++;
        }
    }
    return length;
}

// Function to expand environment variables in the input string
char *expand_string(const char *str, t_env *env_list) {
    size_t result_size = get_expanded_length(str, env_list) + 1;
    char *result = (char *)malloc(result_size);
    if (!result) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';
    char *result_ptr = result;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    while (*str) {
        if (*str == '\'') {
            in_single_quotes = !in_single_quotes;
            str++;
        } else if (*str == '"') {
            in_double_quotes = !in_double_quotes;
            str++;
        } else if (*str == '$' && !in_single_quotes) {
            str++;
            const char *var_start = str;
            if (*str == '?') {
                char *var_value = find_env_value("?", env_list);
                strcpy(result_ptr, var_value);
                result_ptr += strlen(var_value);
                str++;
            } else {
                while (*str && (isalnum(*str) || *str == '_')) {
                    str++;
                }
                size_t var_len = str - var_start;
                char *var_name = strndup(var_start, var_len);
                char *var_value = find_env_value(var_name, env_list);
                free(var_name);
                if (var_value) {
                    strcpy(result_ptr, var_value);
                    result_ptr += strlen(var_value);
                }
            }
        } else {
            *result_ptr++ = *str++;
        }
    }
    *result_ptr = '\0';
    return result;
}
