#include "minishell.h"

int	load_envv(void)
{
	int			i;
	char		**env;
	extern char	**environ;
	
	env = environ;

	i = 0;
	while (env[i])
	{
		printf("%d: %s\n", i, env[i]);
		i++;
	}
	// first search shell vars then if not found
	// use getenv to retreive environment variables
	
	printf("\ngetenv: %s\n", getenv("PATH"));

	return (0);
}

// modify variable expansion to take token, use as key and replace with
// value

