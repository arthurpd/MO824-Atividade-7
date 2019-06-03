#include <bits/stdc++.h>
using namespace std;

#define MAXN 11234

int n, c;
int w[MAXN];
int b[MAXN];

int main(void)
{
	cin >> n >> c;
	for (int i = 0; i < n; i++)
		cin >> w[i];

	int ans = 0;
	for (int i = 0; i < n; i++)
	{
		bool ok = false;
		for (int j = 0; j < ans; j++)
			if (b[j] + w[i] <= c)
			{
				b[j] += w[i];
				ok = true;
				break;
			}

		if (!ok)
			b[ans++] = w[i];
	}

	cout << ans << endl;
}
