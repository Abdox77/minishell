#include "minishell.h"

static char *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

char *expand_env_variable(char *input, t_env *env)
{
    char *var_name;
    char *var_value;
    if (input == NULL || input[0] != '$')
        return NULL;
    var_name = input + 1;
    var_value = get_env_value(env, var_name);
    return var_value;
}

