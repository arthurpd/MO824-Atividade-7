#include <bits/stdc++.h>
using namespace std;

#define MAXN 11234

int n, c;
int w[MAXN];

int main(void)
{
	cin >> n >> c;
	for (int i = 0; i < n; i++)
		cin >> w[i];

	int ans = 0;
	int cur = c;
	for (int i = 0; i < n; i++)
		if (cur + w[i] <= c)
			cur += w[i];
		else
		{
			ans++;
			cur = w[i];
		}
	cout << ans << endl;
}
