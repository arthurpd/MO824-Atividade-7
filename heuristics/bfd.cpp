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
		int choice = -1;
		int fit = 0x3f3f3f3f;
		for (int j = 0; j < ans; j++)
			if (b[j] + w[i] <= c)
			{
				if (c - (b[j] + w[i]) < fit)
				{
					fit = c - (b[j] + w[i]);
					choice = j;
				}
			}

		if (choice == -1)
			b[ans++] = w[i];
		else
		{
			b[choice] += w[i];
		}
	}

	cout << ans << endl;
}
