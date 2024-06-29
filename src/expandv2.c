
#include "minishell.h"

static int is_inside_quotes(const char *str, const char *pos)
{
    int in_single_quotes;
    int in_double_quotes;

    in_single_quotes = 0;
    in_double_quotes = 0;
    while (str < pos)
    {
        if (*str == '\'' && !in_double_quotes)
            in_single_quotes = !in_single_quotes;
        else if (*str == '"' && !in_single_quotes)
            in_double_quotes = !in_double_quotes;
        str++;
    }
    return in_double_quotes;
}

static void *malloc_with_error(size_t size)
{
    void *ptr;

    ptr = malloc(size + 2);
    if (ptr == NULL)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
static char **single_arg(char *arg)
{
    char **single_arg;

    single_arg = malloc_with_error(2 * sizeof(char *));
    single_arg[0] = ft_strdup(arg);
    single_arg[1] = NULL;
    return single_arg;
}
char **expand_arg_if_needed(char *arg, char *og_arg, t_env *env_list)
{
    char *expanded;
    char **split_expanded;

    if (arg)
    {
        if (ft_strchr(arg, '$'))
        {
            expanded = expand_string(og_arg, env_list);
            if (is_inside_quotes(og_arg, strchr(og_arg, '$')))
            {
                split_expanded = malloc_with_error(2 * sizeof(char *));
                split_expanded[0] = expanded;
                split_expanded[1] = NULL;
            }
            else
            {
                split_expanded = ft_split(expanded, ' ');
                free(expanded);
            }
            return split_expanded;
        }
        return (single_arg(arg));
    }
    return NULL;
}

static int get_args_length(char **args)
{
    int length = 0;
    while (args[length] != NULL)
        length++;
    return length;
}

static char **concat_args(char **args1, char **args2)
{
    int len1;
    int len2;
    char **result;
    int j;
    int i;

    len1 = get_args_length(args1);
    len2 = get_args_length(args2);
    result = malloc_with_error((len1 + len2 + 1) * sizeof(char *));
    i = 0;
    while (i < len1)
    {
        result[i] = args1[i];
        i++;
    }
    j = 0;
    while (j < len2)
    {
        result[i + j] = args2[j];
        j++;
    }
    result[i + j] = NULL;
    return result;
}

char	**initialize_processed_args(char *cmd, char *og_cmd, t_env *env_list)
{
	char	**for_cmd;
	char	**processed_args;
	int		i;

	for_cmd = expand_arg_if_needed(cmd, og_cmd, env_list);
	i = 0;
	while (for_cmd && for_cmd[i])
		i++;
	processed_args = malloc_with_error(sizeof(char *) * (i + 1));
	i = 0;
	while (for_cmd && for_cmd[i])
	{
		processed_args[i] = strdup(for_cmd[i]);
		i++;
	}
	processed_args[i] = NULL;
	free_strs(for_cmd);
	return (processed_args);
}

char	**expand_and_concat_args(char **args, char **og_args, t_env *env_list, char **processed_args)
{
	char	**expanded_args;
	char	**new_processed_args;
	int		i;

	i = 1;
	while (args[i] != NULL)
	{
		expanded_args = expand_arg_if_needed(args[i], og_args[i - 1], env_list);
		new_processed_args = concat_args(processed_args, expanded_args);
		free(processed_args);
		free(expanded_args);
		processed_args = new_processed_args;
		i++;
	}
	return (processed_args);
}

char	**process_args(char **args, char **og_args, char *og_cmd, char *cmd, t_env *env_list)
{
	char	**processed_args;

	processed_args = initialize_processed_args(cmd, og_cmd, env_list);
	processed_args = expand_and_concat_args(args, og_args, env_list, processed_args);
	return (processed_args);
}

static char *find_env_value(const char *var_name, t_env *env_list)
{
    if (ft_strcmp(var_name, "?") == 0)
        return ft_itoa_no_malloc(stat_handler(0, 0));
    while (env_list != NULL)
    {
        if (strcmp(var_name, env_list->key) == 0)
            return env_list->value;
        env_list = env_list->next;
    }
    return NULL;
}

static size_t handle_quotes_and_length(const char **str, int *in_single_quotes, int *in_double_quotes)
{
    if (**str == '\'' && !*in_double_quotes)
    {
        *in_single_quotes = !*in_single_quotes;
        (*str)++;
        return 0;   
    }
    else if (**str == '"' && !*in_single_quotes)
    {
        *in_double_quotes = !*in_double_quotes;
        (*str)++;
        return 0;
    }
    return 1;
}

static void handle_special_variable(const char **str, t_env *env_list, size_t *length)
{
    const char *var_value = find_env_value("?", env_list);
    *length += ft_strlen(var_value);
    (*str)++;
}

char *get_var_value(int var_len, const char *var_start, t_env *env_list)
{
    char *var_name;
    char *var_value;

    var_name = ft_strndup(var_start, var_len);
    var_value = find_env_value(var_name, env_list);
    free(var_name);
    return var_value;
}
static size_t handle_dollar_sign(const char **str, t_env *env_list, size_t length)
{
    const char *var_start;
    char *var_value;
    size_t var_len;

    (*str)++;
    var_start = *str;
    if (**str == '?')
        handle_special_variable(str, env_list, &length);
    else
    {
        while (**str && (isalnum((unsigned char)**str) || **str == '_'))
            (*str)++;
        if (*str == var_start)  // No valid variable name found
            length++;  // Count the dollar sign as a literal character
        else
        {
            var_len = *str - var_start;
            var_value = get_var_value(var_len, var_start, env_list);
            if (var_value)
                length += strlen(var_value);
        }
    }
    return length;
}


static size_t get_expanded_length(const char *str, t_env *env_list)
{
    size_t length = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    while (*str)
    {
        if (!handle_quotes_and_length(&str, &in_single_quotes, &in_double_quotes))
            continue;
        else if (*str == '$' && !in_single_quotes)
            length = handle_dollar_sign(&str, env_list, length);
        else
        {
            length++;
            str++;
        }
    }
    return length;
}

static void copy_variable_value(const char *var_value, char **result_ptr)
{
    if (var_value)
    {
        strcpy(*result_ptr, var_value);
        *result_ptr += strlen(var_value);
    }
}

static void handle_special_variable2(const char **str, char **result_ptr, t_env *env_list)
{
    const char *var_value = find_env_value("?", env_list);
    copy_variable_value(var_value, result_ptr);
    (*str)++;
}

static void extract_variable_name(const char **str, char **var_name_ptr)
{
    const char *var_start = *str;
    while (**str && (isalnum((unsigned char)**str) || **str == '_'))
        (*str)++;
    size_t var_len = *str - var_start;
    if (var_len > 0)
        *var_name_ptr = ft_strndup(var_start, var_len);
}

static void handle_variable_expansion(const char **str, char **result_ptr, t_env *env_list)
{
    char *var_name;
    const char *var_value;

    var_name = NULL;
    extract_variable_name(str, &var_name);
    if (var_name)
    {
        var_value = find_env_value(var_name, env_list);
        copy_variable_value(var_value, result_ptr);
        free(var_name);
    }
    else
        *(*result_ptr)++ = '$';
}

void expand_variable(const char **str, char **result_ptr, t_env *env_list, int *in_single_quotes)
{
    if (**str == '?')
        handle_special_variable2(str, result_ptr, env_list);
    else if (!*in_single_quotes)
        handle_variable_expansion(str, result_ptr, env_list);
    else
        *(*result_ptr)++ = '$';  // Just copy '$' if in single quotes
}

static void handle_single_quotes(const char **str, int *in_single_quotes)
{
    *in_single_quotes = !(*in_single_quotes);
    (*str)++;
}

static void handle_double_quotes(const char **str, int *in_double_quotes)
{
    *in_double_quotes = !(*in_double_quotes);
    (*str)++;
}

static void handle_dollar_sign2(const char **str, char **result_ptr, t_env *env_list, int *in_single_quotes)
{
    (*str)++;
    expand_variable(str, result_ptr, env_list, in_single_quotes);
}

static void expand_char(char **result_ptr, const char **str)
{
    **result_ptr = **str;
    (*result_ptr)++;
    (*str)++;
}

char *expand_string(const char *str, t_env *env_list)
{
    size_t result_size;
    char *result;
    char *result_ptr;
    int in_single_quotes;
    int in_double_quotes;

    in_single_quotes = 0;
    in_double_quotes = 0;
    result_size = get_expanded_length(str, env_list);
    result = malloc_with_error(result_size);
    result_ptr = result;
    while (*str)
    {
        if (*str == '\'' && !in_double_quotes)
            handle_single_quotes(&str, &in_single_quotes);
        else if (*str == '"' && !in_single_quotes)
            handle_double_quotes(&str, &in_double_quotes);
        else if (*str == '$' && !in_single_quotes)
            handle_dollar_sign2(&str, &result_ptr, env_list, &in_single_quotes);
        else
            expand_char(&result_ptr, &str);
    }
    *result_ptr = '\0';
    return result;
}
