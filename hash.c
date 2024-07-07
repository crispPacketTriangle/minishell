#include "minishell.h"

int	init_arr(t_data *data, int n)
{
	int	i;
	data->uev = malloc(n * sizeof(t_var_tb *));
	if (!data->uev)
		return (1);
	i = 0;
	while (i < n)
	{
		data->uev[i] = NULL;
		i++;
	}
	return (0);
}

int	poly_r_hash(char *key, int n)
{
	unsigned long long	base;
	unsigned long long	pow;
	unsigned long long	mod;
	unsigned long long	index;
	int					i;
	
	base = 31;
	pow = 1;
	mod = 1000000009;
	i = 0;
	index = 0;
	while (key[i])
	{
		index = (index + ((unsigned long long)key[i] - 'a' + 1) * pow) % mod;
		pow = (pow * base) % mod;
		i++;
	}
	printf("idx: %d\n", (int)index % n);
	return ((int)index % n);
}

// int	main()
// {
// 	printf("%d\n", poly_r_hash("hello", 100));
// 	printf("%d\n", poly_r_hash("this", 100));
// 	printf("%d\n", poly_r_hash("is", 100));
// 	printf("%d\n", poly_r_hash("a", 100));
// 	printf("%d\n", poly_r_hash("polynomial", 100));
// 	printf("%d\n", poly_r_hash("rolling", 100));
// 	printf("%d\n", poly_r_hash("hash", 100));
// 	printf("%d\n", poly_r_hash("algorithm", 100));
// 	printf("%d\n", poly_r_hash("for", 100));
// 	printf("%d\n", poly_r_hash("unbash", 100));
// }
