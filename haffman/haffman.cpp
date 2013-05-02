#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>


using namespace std;
const int MAX_INT = (int) 2e9;
const int N = 256;
vector<char> a;
vector<int> k;
string code[N];
vector<pair<int, int> > Tree;

void encode(int n, string s)
{
    if (n == -1)
        return;
	if (n < N)
	{
		code[n] = s;
		return;
	}
	encode(Tree[n].first, s + '0');
	encode(Tree[n].second, s + '1');
}

string decode(string s)
{
	string curr = "", ans = "";
	for (size_t i = 0; i < s.size(); i++)
	{
		curr += s[i];
		for (int i = 0; i < N; i++)
			if (curr == code[i])
			{
				ans += (char) i;
				curr = "";
				break;
			}
	}
	return ans;
}

void Huffman(vector<char> &a)
{
	int nk = N;
	for (size_t i = 0; i < a.size(); i++)
		k[(int) a[i]]++;
	while (1)
	{
		int MinX = MAX_INT, MinY = MAX_INT;
		int x, y = -1;
		for (int i = 0; i < nk; i++)
			if (k[i] && k[i] < MinX)
			{
				MinX = k[i];
				x = i;
			}
		for (int i = 0; i < nk; i++)
			if (i != x && k[i] && k[i] < MinY)
			{
				MinY = k[i];
				y = i;
			}
		if (y == -1)
			break;
		++nk;
		k[x] = k[y] = 0;
		k.push_back(MinX + MinY);
		Tree.push_back(make_pair(x, y));
	}
    if (nk == N)
    {
        int x;
        for (int i = 0; i < N; i++)
            if (k[i])
                x = i;
        ++nk;
        Tree.push_back(make_pair(x, -1));
    }
	encode(nk - 1, "");
	string encoding_string = "";
	for (size_t i = 0; i < a.size(); i++)
		encoding_string += code[(int) a[i]];
	cout << encoding_string << endl;

	cout << decode(encoding_string) << endl;
}

int main()
{
	Tree.resize(N);
	k.resize(N);
	
	srand((unsigned) time(0));
	int n = rand() % 30;
	for (int i = 0; i < n; i++)
		a.push_back(char(rand() % 26 + 65));
	Huffman(a);
	return 0;
}