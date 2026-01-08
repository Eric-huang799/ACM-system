#include <stdio.h>
	int a[1001];
	void quick_sort(int l, int r)
	{
		if (l >= r) return;
		int i = l, j = r, pivot = a[(l + r) >> 1];
			while (i <= j)
		{
			while (a[i] < pivot) ++i;
			while (a[j] > pivot) --j;
		if (i <= j)
	{
		int t = a[i]; a[i] = a[j]; a[j] = t;
		++i; --j;
	}
		}
		quick_sort(l, j);
		quick_sort(i, r);
	}
	int main(void)
	{
		int n;
		if (scanf("%d", &n) != 1) return 0;
		int i;
		for (i = 1; i <= n; ++i) scanf("%d", &a[i]);
		quick_sort(1, n);
		for (i = 1; i <= n; ++i)
		if (i == 1 || a[i] != a[i - 1])
		printf("%d ", a[i]);
		return 0;
	}
