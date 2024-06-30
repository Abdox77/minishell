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
