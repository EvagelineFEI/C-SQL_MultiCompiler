//
// Created by yys on 2024-05-19.
//

#ifndef FEI_COMPILER_LR0_H
#define FEI_COMPILER_LR0_H
#define MAX 800
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <stack>
# include "GrammarRule.h"
using namespace std;
// 从字符串s第idx的地方开始找一个完整的字符串（读到空格为止），返回这个字符串
class LR0Action{

};

class StateSet{
public:
    StateSet(int idx);
    int state_idx;
    char state_type;
    vector<GrammarRule> rules;
};
class LeftRightZero{
public:
    // get grammar rules from file
    LeftRightZero(string language, string gram_file);
    void get_grammars();

    void generate_lr0_item();
//    void construct_table();
    void makeDfa();
    void rule_processor(StateSet &state, GrammarRule &rule);
    void make_goto();
    int find_state(GrammarRule rule);
    void gram_analyse(string token);
    void show_rules();
    vector<string> end_symbol;
    vector<string> none_end_symbol;
    map<pair<int , string >, int> go_to;
    stack<string> input_stack;
    stack<string> analyse_stack;
    void record_step(ofstream &outfile, const string &operation);
private:
    string target_language;
    string start_symbol;
    ifstream grammar_file;
    // Store the grammar rules;
    vector<GrammarRule> original_rule_set;
    // Process the grammar rules and get lr0 items
    vector<GrammarRule> rule_lr0_set;
    // store the actions under lr0 rules
    vector<LR0Action> action[MAX];
    vector<StateSet> DFA;
    void get_end_symbol(string &a);
    void get_none_end_symbol();
};
#endif //FEI_COMPILER_LR0_H
