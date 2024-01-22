#ifndef FT_SPLIT
#define FT_SPLIT

int	number_of_tab(char const *s, char c);
void	fill_tab(char const *s, char c, char **p, int l);
void	ft_free(char **p, int j);
char	**numberofcharacter(char const *s, char c, char **p);
char	**ft_split(char const *s, char c);
char* copyString(const char* original);
#endif