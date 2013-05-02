#include "mygraph.h"
#include <iostream>
using namespace std;

myGraph::myGraph() { }
myGraph::myGraph(vector<string> data) {
    nicks.clear();
    matrix.resize(data.size());
    for (size_t i = 0; i < data.size(); i++)
        matrix[i].resize(data.size(), false);

    for (size_t i = 0; i < data.size(); i++) {
        string nick = "";
        for (size_t j = 0; j < (int) data[i].size() && data[i][j] != ' '; j++)
            nick += data[i][j];

        nicks[nick] = i;
        names.push_back(nick);
        data[i].erase(0, nick.size() + 1);
    }
    for (size_t i = 0; i < data.size(); i++) {
        bool f = false;
        string next_nick = "";
        data[i] += " ";
        for (int j = i; j < (int) data[i].size(); j++) {
            if (data[i][j] == ' ') {
                if (f) {
                    matrix[i][nicks[next_nick]] = matrix[nicks[next_nick]][i] = true;
                    next_nick = "";
                }
                f ^= 1;
                continue;
            }
            if (!f)
                continue;
            next_nick += data[i][j];
        }
    }
}
