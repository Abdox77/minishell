#include "minishell.h"
#include <dirent.h>
#include <stdbool.h>

bool isMatch(const char *s, const char *p)
{
    const char *ws = 0;
    const char *wp = 0;

    while (*s) {
        if (*p == *s || *p == '?')
            s++, p++;
        else if (*p == '*')
            wp = ++p, ws = s;
        else if (wp)
            p = wp, s = ++ws;
        else
            return false;
    }

    while (*p)
    {
        if (*p != '*')
            return false;
        p++;
    }

    return true;
}

static size_t count_matches(const char *pattern)
{
    DIR *dir;
    struct dirent *entry;
    size_t count = 0;

    dir = opendir(".");
    if (dir == NULL)
    {
        perror("opendir failed");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (isMatch(entry->d_name, pattern))
            count++;
    }

    closedir(dir);
    return count;
}

static char **list_matching_files(const char *pattern)
{
    DIR *dir;
    struct dirent *entry;
    size_t count = count_matches(pattern);
    char **matches = (char **)malloc((count + 1) * sizeof(char *));
    size_t idx = 0;

    if (!matches)
    {
        perror("malloc failed");
        return NULL;
    }

    dir = opendir(".");
    if (dir == NULL)
    {
        perror("opendir failed");
        free(matches);
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (isMatch(entry->d_name, pattern))
            matches[idx++] = strdup(entry->d_name);
    }
    matches[idx] = NULL;
    closedir(dir);
    return matches;
}

static size_t calculate_total_size(char **args)
{
    size_t total_size = 0;
    int i = 0;

    while (args[i] != NULL)
    {
        if (strchr(args[i], '*') || strchr(args[i], '?'))
            total_size += count_matches(args[i]);
        else
            total_size++;
        i++;
    }

    return total_size;
}

char **expand_wildcards(char **args)
{
    size_t total_size = calculate_total_size(args);
    char **expanded_args = (char **)malloc((total_size + 1) * sizeof(char *));
    if (!expanded_args)
        return (perror("malloc failed"),NULL);
    int i = 0;
    int count = 0;

    while (args[i] != NULL)
    {
        if (strchr(args[i], '*') || strchr(args[i], '?'))
        {
            char **matches = list_matching_files(args[i]);
            if (matches)
            {
                int j = 0;
                while (matches[j] != NULL)
                {
                    expanded_args[count++] = strdup(matches[j]);
                    free(matches[j]);
                    j++;
                }
                free(matches);
            }
        }
        else
            expanded_args[count++] = strdup(args[i]);
        i++;
    }
    expanded_args[count] = NULL;
    return expanded_args;
}

void free_expanded_args(char **args)
{
    if (!args) return;
    int i = 0;
    while (args[i] != NULL)
    {
        free(args[i]);
        i++;
    }
    free(args);
}
