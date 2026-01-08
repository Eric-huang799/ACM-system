int main(void)
{
    int N, V;
    if (scanf("%d %d", &N, &V) != 2) return 0;

    int vol[MAXN];
    int val[MAXN];
    for (int i = 0; i < N; ++i) {
        scanf("%d %d", &vol[i], &val[i]);
    }

    int dp[MAXV + 1] = {0};

    for (int i = 0; i < N; ++i) {
        int w = vol[i];
        int c = val[i];
        for (int j = V; j >= w; --j) {
            if (dp[j - w] + c > dp[j])
                dp[j] = dp[j - w] + c;
        }
    }

    printf("%d\n", dp[V]);
    return 0;
}

