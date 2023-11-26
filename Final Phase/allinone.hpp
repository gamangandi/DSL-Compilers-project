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

void shift(node& box, vector<vector<string>>& G, vector<string> terminals, vector<string> nonterminals, map<baserules, nodeptr>& existing_bases, int &states){

    map<string, vector<pair<vector<string>, int>>> shiftfor;

    for(int i = 0; i < box.rules.size(); i++){

        int j = 0;

        while(box.rules[i].first[j] != "dot"){

            j++;
        }

        if(j == box.rules[i].first.size() - 1) continue;

        else{

            string pushfor = box.rules[i].first[j+1];

            if(pushfor == "ep") continue;

            pair<vector<string>, int> temp;

            temp = box.rules[i];

            temp.first[j] = pushfor;
            temp.first[j+1] = "dot";

            if(shiftfor.find(pushfor) != shiftfor.end()){

                shiftfor[pushfor].push_back(temp);
            }

            else shiftfor.insert({pushfor, {temp}});        
        }
    }

    for(auto x: shiftfor){

        string pushfor = x.first;
        sort(x.second.begin(), x.second.end());

        if(existing_bases.find(x.second) != existing_bases.end()){

            box.towards.push_back({pushfor, existing_bases[x.second]});            
            continue;            
        }

        nodeptr newnode = new node();
        newnode->state = states;
        states++;

        newnode->base_rules = x.second;

        existing_bases.insert({x.second, newnode});
        box.towards.push_back({x.first, newnode});
    }
}


void build_automaton(vector<vector<string>>& G, vector<string> terminals, vector<string> nonterminals, map<string, set<int>> first, map<baserules, nodeptr>& existing_bases, int& states, set<int>& visited){

    vector<string> main_rule = G[0];
    string main_sym = main_rule[0];
    string main_sym_dash = main_sym + "dash";
    int n = terminals.size();

    nodeptr firstnode = new node();
    firstnode -> base_rules = {{{main_sym_dash, "pointer", "dot", main_sym}, n+1}};
    firstnode->state = states;
    states++;

    existing_bases.insert({firstnode->base_rules, firstnode});

    bool checker = true;

    while(checker){

        checker = false;

        int automaton_size = existing_bases.size();

        for(auto x: existing_bases){

            int state = x.second->state;

            if(visited.find(state) != visited.end()){

                continue;
            }
            visited.insert(state);

            closure(*(x.second), G, terminals, nonterminals, first);
            shift(*(x.second), G, terminals, nonterminals, existing_bases, states);
        }

        if(automaton_size != existing_bases.size()){

            checker = true;            
        }
    }
}

void print_box(nodeptr box, vector<string> terminals){

    terminals.push_back("ep");
    terminals.push_back("dollar");

    baserules rules = box->rules;

    vector<pair<string, node*>> towards = box->towards;

    int state = box->state;

    cout<<"-----------------------------------"<<endl;

    cout<<"STATE: "<<state<<endl<<"RULES: "<<endl;

    for(auto x : rules){

        cout<<terminals[x.second]<<": ";

        for(auto y : x.first){

            cout<<y<<" ";
        }
        cout<<endl; 
    }

    cout<<"TOWARDS"<<endl;

    for(auto x: towards){

        cout<<x.first<<": ";
        cout<<x.second->state<<endl;
    }

    cout<<"-----------------------------------"<<endl<<endl;

}

void create_parsing_table(vector<vector<string>> G, vector<string> terminals, vector<string> nonterminals, map<int, map<string, vector<string>>> & table, map<int, nodeptr>& state_ptr, map<vector<string>, int>& production_map){

    terminals.push_back("ep");
    terminals.push_back("dollar");

    for(auto x: state_ptr){

        int state = x.first;

        vector<pair<string, node*>> towards = x.second->towards;

        map<string, int> reduce_to;

        for(auto y: x.second->rules){

            if(y.first[y.first.size() - 1] != "dot") continue;

            int follow_sym_index = y.second;
            string follow_sym = terminals[follow_sym_index];

            int prod_num = -1;

            if(production_map.find(y.first) != production_map.end()){

                prod_num = production_map[y.first];
            }

            string corr_red = "R" + to_string(prod_num);

            if(table[state].find(follow_sym) == table[state].end()){

                table[state].insert({follow_sym, {corr_red}});
            }

            else table[state][follow_sym].push_back(corr_red);            
        }

        for(auto y: towards){

            string temp = y.first;
            int to = y.second->state;
            string corr_shift = "S" + to_string(to);

            if(table[state].find(temp) == table[state].end()){

                table[state].insert({temp, {corr_shift}});
            }

            else table[state][temp].push_back(corr_shift);           
        }
    }
}

bool parse(vector<vector<string>> G, vector<string> input, map<int, map<string, vector<string>>>& table, map<vector<string>, int>& production_map){

    stack<string> inputs;
    inputs.push("dollar");

    for(int i = input.size()-1; i >= 0; i--){

        inputs.push(input[i]);
    }

    stack<string> state_stack;
    state_stack.push("0");

    bool checker = true;

    while(checker){

        string stacksym = state_stack.top();
        int state = stoi(stacksym);

        string lookahead = inputs.top();

        cout<<"input lookahead:  "<<lookahead<<endl;
        cout<<"state on top: "<<state<<endl<<endl;

        if(table[state].find(lookahead) == table[state].end()){

            cout<<"unused symbol is being used"<<endl;
            return 0;
        }

        string what_to_do = table[state][lookahead][0];

        if(what_to_do[0] == 'S'){

            string to_state_str = what_to_do.substr(1,what_to_do.size() - 1);

            state_stack.push(lookahead);
            state_stack.push(to_state_str);

            inputs.pop();
        }

        else if(what_to_do[0] == 'R'){

            if(what_to_do[1] == '-' && lookahead == "dollar"){

                cout<<"parse successful, string accepted."<<endl<<endl;
                return 1;
            }

            string reduce_using_rule = what_to_do.substr(1,what_to_do.size() -1);
            int rule_number = stoi(reduce_using_rule);

            vector<string> rule;

            int h = 0;

            for(auto x: production_map){

                if(x.second == rule_number){
                    rule = x.first;
                    h = 1;
                    break;
                }
            }

            if(h == 0){

                cout<<"error in code."<<endl;
                return 0;
            }

            int removals = rule.size() - 3;
            string insert_sym = rule[0];

            for(int i = 0; i < removals*2; i++){

                state_stack.pop();
            }

            string newstacktopstr = state_stack.top();
            int newstacktop = stoi(newstacktopstr);


            if(table[newstacktop].find(insert_sym) == table[newstacktop].end()){
                cout<<"string is not in the grammar."<<endl;
                return 0;
            }

            string newtop = table[newstacktop][insert_sym][0];
            newtop = newtop.substr(1,newtop.size()-1);

            state_stack.push(insert_sym);
            state_stack.push(newtop);
        }
    }

    return false;
}

void print_parsing_table(map<int, map<string, vector<string>>>& table){

    //cout<<table.size()<<endl<<endl;

    ofstream out;

    out.open("out.txt");

    for(auto x: table){

        out<<"state "<<x.first<<": "<<endl;

        for(auto y: x.second){

            out<<"on "<<y.first<<endl<<endl;

            for(auto z: y.second){

                out<<z<<endl;
            }

            out<<endl;
        }

        out<<endl;
    }
}
