#ifndef MYGRAPH_H
#define MYGRAPH_H
#include <vector>
#include <string>
#include <map>
using namespace std;

class myGraph
{
public:
    myGraph();
    myGraph(vector< string > data);

    vector<string > names;
    map<string, int> nicks;
    vector< vector<bool> > matrix;
};

#endif // MYGRAPH_H
