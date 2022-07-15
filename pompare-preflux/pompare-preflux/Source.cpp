#include <iostream> 
#include <vector>
#include <algorithm>
#include <fstream>

using std::vector;
using namespace std;

struct Muchie
{
	int flux, capacitate, u, v;

	Muchie(int flux, int capacitate, int u, int v)
	{
		this->flux = flux;
		this->capacitate = capacitate;
		this->u = u;
		this->v = v;
	}
};


struct Varf
{
	int h, e_flow;
	Varf(int h, int e_flow)
	{
		this->h = h;
		this->e_flow = e_flow;
	}
};

class Graf
{
	int V;
	vector<Varf> varfuri;
	vector<Muchie> muchii;
	bool pompare(int u);
	void actualizare(int u);
	void init(int s);
	void updateReverseFlow(int i, int flow);

public:
	Graf(int V);
	void adaugaMuchie(int u, int v, int w);
	int flux_max(int s, int t);
};

Graf::Graf(int V)
{
	this->V = V;
	for (int i = 0; i < V; i++)
		varfuri.push_back(Varf(0, 0));
}

void Graf::adaugaMuchie(int u, int v, int capacity)
{
	muchii.push_back(Muchie(0, capacity, u, v));
}

void Graf::init(int s)
{

	varfuri[s].h = varfuri.size();
	for (int i = 0; i < muchii.size(); i++)
	{
		if (muchii[i].u == s)
		{
			muchii[i].flux = muchii[i].capacitate;
			varfuri[muchii[i].v].e_flow += muchii[i].flux;
			muchii.push_back(Muchie(-muchii[i].flux, 0, muchii[i].v, s));
		}
	}
}

int verif_overflow(vector<Varf>& ver)
{
	for (int i = 1; i < ver.size() - 1; i++)
		if (ver[i].e_flow > 0)
			return i;

	return -1;
}

void Graf::updateReverseFlow(int i, int flow)
{
	int u = muchii[i].v, v = muchii[i].u;

	for (int j = 0; j < muchii.size(); j++)
	{
		if (muchii[j].v == v && muchii[j].u == u)
		{
			muchii[j].flux -= flow;
			return;
		}
	}

	Muchie e = Muchie(0, flow, u, v);
	muchii.push_back(e);
}

bool Graf::pompare(int u)
{

	for (int i = 0; i < muchii.size(); i++)
	{

		if (muchii[i].u == u)
		{
			if (muchii[i].flux == muchii[i].capacitate)
				continue;
			if (varfuri[u].h > varfuri[muchii[i].v].h)
			{
				int flow = min(muchii[i].capacitate - muchii[i].flux, varfuri[u].e_flow);
				varfuri[u].e_flow -= flow;
				varfuri[muchii[i].v].e_flow += flow;
				muchii[i].flux += flow;
				updateReverseFlow(i, flow);
				return true;
			}
		}
	}
	return false;
}


void Graf::actualizare(int u)
{

	int mh = INT_MAX;
	for (int i = 0; i < muchii.size(); i++)
	{
		if (muchii[i].u == u)
		{
			if (muchii[i].flux == muchii[i].capacitate)
				continue;
			if (varfuri[muchii[i].v].h < mh)
			{
				mh = varfuri[muchii[i].v].h;
				varfuri[u].h = mh + 1;
			}
		}
	}
}

int Graf::flux_max(int s, int t)
{
	init(s);
	while (verif_overflow(varfuri) != -1)
	{
		int u = verif_overflow(varfuri);
		if (!pompare(u))
			actualizare(u);
	}

	return varfuri.back().e_flow;
}

int main(int argc, char* argv[])
{
	std::ifstream fin("in.txt");
	std::ofstream fout("out.txt");
	int V, E, x, y, w;
	fin >> V >> E;
	Graf g(V);
	for (int i = 0; i < E; i++) {
		fin >> x >> y >> w;
		g.adaugaMuchie(x, y, w);

	}

	int s = 0, t = V - 1;

	fout << g.flux_max(s, t);

	fin.close();
	fout.close();
	return 0;
}
