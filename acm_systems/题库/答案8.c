#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define N 10
char g[N][N];
static bool dfs(int x, int y)
{
	if (y == 9) return true;
	if (x == 9) return dfs(0, y + 1);
	if (g[x][y] != '.') return dfs(x + 1, y);
	bool st[N];
	memset(st, 0, sizeof(st));
	for (int i = 0; i < 9; ++i)
		if (g[i][y] >= '1' && g[i][y] <= '9')
			st[g[i][y] - '0'] = true;
	for (int i = 0; i < 9; ++i)
		if (g[x][i] >= '1' && g[x][i] <= '9')
			st[g[x][i] - '0'] = true;
	int sx = x / 3 * 3, sy = y / 3 * 3;
	for (int i = sx; i < sx + 3; ++i)
	for (int j = sy; j < sy + 3; ++j)
		if (g[i][j] >= '1' && g[i][j] <= '9')
			st[g[i][j] - '0'] = true;
	for (int i = 1; i <= 9; ++i)
		if (!st[i])
		{
			g[x][y] = '0' + i;
		if (dfs(x + 1, y)) return true;
		}
			g[x][y] = '.';
	return false;
}
	int main(void)
	{
	for (int i = 0; i < 9; ++i)
		scanf("%9s", g[i]);
		dfs(0, 0);
	for (int i = 0; i < 9; ++i, putchar('\n'))
	for (int j = 0; j < 9; ++j)
		putchar(g[i][j]);
	return 0;
	}	printf("%d ", a[i]);
		return 0;
	}

