#include<bits/stdc++.h>
using namespace std;

class node{

    public:
        int state;
        vector<pair< vector<string>, int>> base_rules;
        vector<pair<string, node*>> towards;
        vector<pair< vector<string>, int> > rules;
};

typedef node* nodeptr;

typedef vector<pair<vector<string>,int>> baserules;
