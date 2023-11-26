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

void print_follow(map<string, set<int>> follow, vector<string> terminals){

    terminals.push_back("ep");
    terminals.push_back("dollar");

    for(auto x: follow){

        cout<<x.first<<": ";

        for (auto y: x.second){

            cout<<terminals[y]<<" ";
        }   

        cout<<endl;     
    }
}

void print_first(map<string, set<int>> first, vector<string> terminals){

    terminals.push_back("ep");

    for(auto x: first){

        cout<<x.first<<": ";

        for (auto y: x.second){

            cout<<terminals[y]<<" ";
        }    

        cout<<endl;    
    }


}

void uni(string A,string B, map<string, set<int>>& first, map<string, set<int>>& follow, int p = 0, int n = 0){
  if(p==0) { for(auto j : first[B]) first[A].insert(j);}
  else if(p==1){
    for(auto j : first[B]) follow[A].insert(j);
    follow[A].erase(n-1);
  }
  else if(p==2){
    for(auto j : follow[B]) follow[A].insert(j);
  }
}

void f(vector<vector<string>> G,vector<string> terminals, vector<string> non_terminals, map<string, set<int>>& first, map<string, set<int>>& follow){

  terminals.push_back("ep");
  int n = terminals.size();
  unordered_map<string,int> d;
  for(int i=0;i<n;i++) d[terminals[i]]=i;

  vector<bool> v(G.size(),true);
  unordered_set<string> NT(non_terminals.begin(),non_terminals.end());
  unordered_set<string> T(terminals.begin(),terminals.end());

  for(int i = 0;i<G.size();i++){
    if(T.find(G[i][2]) != T.end()){
      first[G[i][0]].insert(d[G[i][2]]);
      v[i]=false;
    }
  }
  bool temp = true;

  while(temp){
    temp = false;
    for(int i=0;i<G.size();i++){

        vector<string> fo(G[i].begin(),G[i].end());
        string A = fo[0];
        int r = first[A].size();
        if(A == fo[2]) {
            cout<<"eliminate left factoring for Non-terminal :"<<A<<endl;
            exit(0);
        }

        if(!v[i]) continue; 
        else{
            int k = 2;
            while(k < fo.size()){
                
                string B = fo[k];

                if(A==B){k++; continue;}

                if(T.find(B)!=T.end()){
                    first[A].insert(d[B]);
                    break;
                }
                else if(first[B].find(n-1)==first[B].end()){
                    uni(A,B, first, follow);
                    break;
                }
                else{
                    bool hero =  (first[A].find(n-1)==first[A].end());
                    uni(A,B, first, follow);
                    if(hero && first[A].find(n-1)!=first[A].end()) first[A].erase(n-1);
                }
                k++;
            }

            if(k==fo.size()) first[A].insert(n-1);
            if( first[A].size() != r ) temp = true;
        }
    }

  }
}

void foll(vector<vector<string>> G,vector<string> terminals, vector<string> non_terminals, map<string, set<int>>& first, map<string, set<int>>& follow){
  
  terminals.push_back("ep");
  int n = terminals.size();
  terminals.push_back("dollar");
  follow[G[0][0]].insert(n);
  unordered_map<string,int> d;
  for(int i=0;i<n;i++) d[terminals[i]]=i;

  vector<bool> v(G.size(),true);
  unordered_set<string> NT(non_terminals.begin(),non_terminals.end());
  unordered_set<string> T(terminals.begin(),terminals.end());

  for(int i = 0;i<G.size();i++){
    if(T.find(G[i][2]) != T.end() && G[i].size()==3){
      v[i]=false;
      continue;
    }
    
    for(int k = 2;k<G[i].size()-1;k++){
      if(NT.find(G[i][k]) != NT.end() && T.find(G[i][k+1]) != T.end()){
        follow[G[i][k]].insert(d[G[i][k+1]]);
      }
      else if(NT.find(G[i][k]) != NT.end() && NT.find(G[i][k+1]) != NT.end()){
        uni(G[i][k],G[i][k+1],first, follow, 1,n);
      }
   
    }
  }
  
  bool temp = true;


  while(temp){
    temp = false;
    for(int i = 0; i < G.size(); i++){
      
      vector<string> fo(G[i].begin()+2, G[i].end());
      string A = G[i][0];
      int len = fo.size()-1;

      if(!v[i]) continue;
      else{
        
        if(NT.find(fo[len]) != NT.end()) uni(fo[len],A,first, follow, 2);
        string rest = A;
        for(int j = len; j > 0; j--){
          
          string B = fo[j-1];
          string C = fo[j];
          if(T.find(B)!=T.end()) continue;
          int rox = follow[B].size();

          if(T.find(C)!=T.end()) follow[B].insert(d[C]);

          else if(first[C].find(n-1)!=first[C].end()){
            
            uni(B,C,first, follow, 1,n);
            uni(B,rest,first, follow, 2);
            rest = B;
          }
          else{
          
            uni(B,C,first, follow, 1,n);
            rest = B;
          }

          if(follow[B].size() != rox) temp = true;
        }
  
      }

    }
  }
}

void closure(node& box, vector<vector<string>>& G, vector<string> terminals, vector<string> nonterminals, map<string, set<int>> first){

    set<pair<int, string>> expandfor;

    terminals.push_back("ep");
    terminals.push_back("dollar");

    map<string, int> corrospondence;

    for(int i = 0; i < terminals.size(); i++){

        corrospondence[terminals[i]] = i;
    }

    set<string> T(terminals.begin(), terminals.end());
    set<string> NT(nonterminals.begin(), nonterminals.end());

    for(auto rule : box.base_rules){

        box.rules.push_back(rule);
    }

    bool change = true;

    while(change){

        change = false;

        for(int i = 0; i < box.rules.size(); i++){

            int j;

            for(j = 0; j < box.rules[i].first.size(); j++){

                if(box.rules[i].first[j] == "dot"){

                    break;
                }
            }

            if(j == box.rules[i].first.size() - 1) continue;

            else if(T.find(box.rules[i].first[j+1]) != T.end()) continue;

            else{

                string cornonterminal = box.rules[i].first[j+1];
                int lookahead;

                if(j + 1 == box.rules[i].first.size() - 1){

                    lookahead = box.rules[i].second;
                    expandfor.insert({lookahead, cornonterminal});
                    continue;
                }

                else{

                    if(T.find(box.rules[i].first[j+2]) != T.end()){

                        lookahead = corrospondence[box.rules[i].first[j+2]];
                        expandfor.insert({lookahead, cornonterminal});

                    }

                    else{

                        for(auto a : first[box.rules[i].first[j+2]]){

                            if(terminals[a] == "ep") continue;

                            expandfor.insert({a, cornonterminal});
                        }
                    }
                }
            }
        }

        for(auto ex : expandfor){

            string X = ex.second;
            int lookahead = ex.first;

            for(auto rule : G){

                if(rule[0] == X){

                    vector<string> temprule = rule;

                    temprule.insert(temprule.begin() + 2, "dot");

                    pair<vector<string>, int> contender = {temprule, lookahead};

                    if(find(box.rules.begin(), box.rules.end(), contender) == box.rules.end()){

                        box.rules.push_back(contender);
                        change = true;
                    }
                }
            }        
        }
    }
}
