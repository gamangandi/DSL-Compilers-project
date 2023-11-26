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

void generate_grammar(vector<string>& stringified_grammar, vector<string>& nonterminals, vector<string>& terminals, vector<vector<string>>& grammar){

    // ifstream ipfile;

    // ipfile.open(infile);

    // vector<string> input;
    // string b;

    // while(ipfile>>b){

    //     input.push_back(b);
    // }

	int i = 0;

    while(i < stringified_grammar.size()){

        string lhstemp = stringified_grammar[i];
        nonterminals.push_back(lhstemp);

        i += 2;

        vector<vector<string>> rhs;
        vector<string> rhstemp;

        while(stringified_grammar[i] != "semic"){

            if(stringified_grammar[i] != "orsym"){

                rhstemp.push_back(stringified_grammar[i]);
                i++;

            }

            else{

                rhs.push_back(rhstemp);
                rhstemp.clear();
                i++;
            }               
        }

        if(stringified_grammar[i] == "semic"){
        
            if(rhstemp.size() != 0){
            
            	rhs.push_back(rhstemp);
            	rhstemp.clear();            
            }
            
            else{
            
            	rhstemp.push_back("ep");
            	rhs.push_back(rhstemp);
            	rhstemp.clear();            
            }

            for(int j = 0; j < rhs.size(); j++){

                vector<string> temprule;
                temprule.push_back(lhstemp);
                temprule.push_back("pointer");

                for(int k = 0; k < rhs[j].size(); k++){

                    temprule.push_back(rhs[j][k]);
                }

                grammar.push_back(temprule);                
            }

            i++;
        }
    }

    unordered_set<string> nont(nonterminals.begin(), nonterminals.end());
    set<string> t;

    for(auto x: stringified_grammar){

        if(x != "orsym" && x != "semic" && x != "pointer" && nont.find(x) == nont.end()){

            t.insert(x);
        }
    }

    terminals.assign(t.begin(), t.end());
}
