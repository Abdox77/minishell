#include "../src/minishell.h"

static int check_key(t_env *env, char *key)
{
    t_env *tmp;

    tmp = env;
    while (tmp)
    {
        if (!ft_strcmp(tmp->key, key))
            return (1);
        tmp = tmp->next;
    }
    return (0);
}

// Helper function to check the key in the environment list and return its value
static char *get_value(t_env *env, const char *key)
{
    t_env *tmp = env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

// Helper function to update the value of a key in the environment list
static void set_value(t_env *env, const char *key, const char *value)
{
    t_env *tmp = env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            free(tmp->value); // Free the old value
            tmp->value = ft_strdup(value); // Set the new value
            return;
        }
        tmp = tmp->next;
    }
    // If the key does not exist and it's "OLDPWD", add a new node to the list
    if (!ft_strcmp(key, "OLDPWD"))
    {
        t_env *new_node = (t_env *)malloc(sizeof(t_env));
        if (!new_node)
        {
            perror("malloc");
            return;
        }
        new_node->key = ft_strdup(key);
        new_node->value = ft_strdup(value);
        new_node->next = env->next;
        env->next = new_node;
    }
    return;
}

// Function to update PWD and OLDPWD after changing the directory
static void update_pwd_and_oldpwd(t_exec *exec, char *old_pwd)
{
    if (!exec || !exec->env)
        return;
    char new_pwd[1024];
    if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
    {
        perror("getcwd");
        stat_handler(1, 1);
        return;
    }
    set_value(exec->env, "PWD", new_pwd);
    set_value(exec->env, "OLDPWD", old_pwd);
}

int cd(char **arg, t_exec *exec)
{
    char old_pwd[1024];
    getcwd(old_pwd, sizeof(old_pwd));

    if (arg[1] == NULL)
    {
        if (!check_key(exec->env, "HOME"))
            return (stat_handler(1, 1), perror("cd: HOME not set"), 0);
        else
        {
            if (chdir(get_value(exec->env, "HOME")) == -1)
                return (stat_handler(1, 1), perror("cd"), 0);
            else
                update_pwd_and_oldpwd(exec, old_pwd);
        }
    }
    else
    {
        if (chdir(arg[1]) == -1)
            return (stat_handler(1, 1), perror("cd"), 0);
        update_pwd_and_oldpwd(exec, old_pwd);
    }
    stat_handler(0, 1);
    return (1);
}
