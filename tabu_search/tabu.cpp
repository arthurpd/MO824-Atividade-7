#include <bits/stdc++.h>
using namespace std;

#define MAXN 11234

typedef pair<int,int> pii;

using namespace std::chrono;
class code_timer
{
	high_resolution_clock::time_point begin;

public:
	code_timer()
	{
		begin = high_resolution_clock::now();
	}

	double seconds()
	{
		return duration_cast<duration<double>>(high_resolution_clock::now() - begin).count();
	}
};


int n, c;
int w[MAXN];
vector<vector<int>> b;
vector<int> bw;
double val;
int nb;
double infd = 1e9;
vector<int> tabu_tab;
list<pair<int,int>> tabu_list;
int iteration;
int best_nb;
bool feasible;
double infeasible_penalty = 1e9;
bool allow_infeasible;

void remove_old_tabu(int tabu_tenure)
{
	while (!tabu_list.empty() && iteration - tabu_list.front().first > tabu_tenure)
	{
		tabu_tab[tabu_list.front().second]--;
		tabu_list.pop_front();
	}
}

void insert_tabu(int x)
{
	tabu_tab[x]++;
	tabu_list.push_back(pii(iteration, x));
}

inline double sq(const double &x)
{
	return x*x;
}

pair<double, int> eval_move_val(int i, int j, int k)
{
	int next_size = nb;
	if (k == nb)
		next_size = nb + 1;
	else if (i == nb - 1 && b[i].size() == 1)
		next_size = nb - 1;
	
	double next_val = (val * nb);

	next_val -= sq(bw[i] / (double) c) + sq(bw[k] / (double) c);

	next_val += sq((bw[i] - w[b[i][j]]) / (double) c);
	next_val += sq((bw[k] + w[b[i][j]]) / (double) c);
	next_val /= next_size;

	return {next_val, next_size};
}

double evaluate_relocation_move(int i, int j, int k)
{
	assert(i != k);

	// if (bw[k] + w[b[i][j]] > c)
	// 	return -infd;

	double pen = 0;
	if (bw[k] + w[b[i][j]] > c)
		pen += infeasible_penalty * (bw[k] + w[b[i][j]] - max(w[b[i][j]], c));

	if (bw[i] > c)
		pen -= infeasible_penalty * min(w[b[i][j]], bw[i] - c);

	double next_val = eval_move_val(i, j, k).first;

	return next_val - val - pen;
}

void set_val()
{
	val = 0.0;
	for (int i = 0; i < nb; i++)
		val += sq(bw[i] / (double) c);

	val /= nb;

	feasible = true;
	for (int i = 0; i < nb; i++)
		if (bw[i] > c)
			feasible = false;
}

void apply_relocation_move(int i, int j, int k)
{
	insert_tabu(b[i][j]);

	auto x = eval_move_val(i, j, k);
	val = x.first;
	nb = x.second;

	bw[k] += w[b[i][j]];
	bw[i] -= w[b[i][j]];
	b[k].push_back(b[i][j]);
	b[i].erase(b[i].begin() + j);

	set_val();
}

bool neighborhood_move()
{
	vector<tuple<int, int, int>> neighborhood;
	tuple<int, int, int> best_move = {-1, -1, -1};
	double best_move_delta = -infd;
		
	for (int i = 0; i < nb; i++)
		if (b[i].size() > 1 || i + 1 == nb) // Only make last one empty.
			for (int j = 0; j < b[i].size(); j++)
				if (2*w[b[i][j]] <= c) // Don't move items too heavy.
					if (tabu_tab[b[i][j]] == 0)
						for (int k = 0; k < nb + 1; k++)
							if (i != k)
							{
								if (b[i].size() == 1 && k == nb)
									continue; // Don't make a bucket empty to transfer to a new bucket.

								if (!allow_infeasible)
									if (bw[k] + w[b[i][j]] > c)
										continue;

								neighborhood.push_back({i, j, k});
							}

	random_shuffle(neighborhood.begin(), neighborhood.end());
	for (auto move : neighborhood)
	{
		double move_delta = evaluate_relocation_move(get<0>(move), get<1>(move), get<2>(move));
		// Relocate to any currently used or one after last used.
		if (move_delta > best_move_delta)
		{
			best_move = move;
			best_move_delta = move_delta;
		}
	}

	if (best_move_delta > -infd)
	{
		apply_relocation_move(get<0>(best_move), get<1>(best_move), get<2>(best_move));
		return true;
	}
	else
		return false;
}

void init_run()
{
	bw.clear();
	b.clear();

	bw.resize(n + 1, 0);
	b.resize(n + 1);

	// nb = 0;
	// for (int i = 0; i < n; i++)
	// {
	// 	bool ok = false;
	// 	for (int j = 0; j < nb; j++)
	// 		if (bw[j] + w[i] <= c)
	// 		{
	// 			b[j].push_back(i);
	// 			bw[j] += w[i];
	// 			ok = true;
	// 			break;
	// 		}

	// 	if (!ok)
	// 	{
	// 		bw[nb] += w[i];
	// 		b[nb].push_back(i);
	// 		nb++;
	// 	}
	// }

	for (int i = 0; i < n; i++)
	{
		b[i].push_back(i);
		bw[i] += w[i];
	}

	nb = n;

	feasible = true;
	set_val();

	best_nb = nb;

	tabu_tab = vector<int>(n, 0);
	tabu_list = list<pii>();

	iteration = 0;
}

int tabu_search(double time_limit, int tabu_tenure)
{
	init_run();
	code_timer exec_time;
	cout << val <<endl;

	while (exec_time.seconds() < time_limit)
	{
		remove_old_tabu(tabu_tenure);
		neighborhood_move();

		if (feasible && nb < best_nb)
		{
			best_nb = nb;
			cout << iteration << "," << best_nb << "," << exec_time.seconds() << endl;
		}

		if (feasible)
			infeasible_penalty *= 0.95;
		else
			infeasible_penalty *= 1.05;

		iteration++;
		// cout << "iter " << iteration << " best_nb " << best_nb << " val " << val << " nb " << nb << " feasible " << feasible << " penalty " << infeasible_penalty << endl;

		// for (int i = 0; i < nb; i++, printf("\n"))
		// 	for (int j = 0; j < b[i].size(); j++)
		// 		cout << w[b[i][j]] << " ";

		// printf("\n****\n");
	}

	return best_nb;
}


int main(int argc, char **argv)
{
	assert(argc == 3);
	cin >> n >> c;
	for (int i = 0; i < n; i++)
		cin >> w[i];

	allow_infeasible = atol(argv[2]);
	tabu_search(atol(argv[1]), ceil(n/4.0));

	int sum = 0;
	for (int i = 0; i < n; i++)
		sum += w[i];
	cout << "Lower bound " << (sum+c-1)/c << endl;
}