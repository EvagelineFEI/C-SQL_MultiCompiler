//
// Created by yys on 2024-05-19.
//

#ifndef FEI_COMPILER_LR0_H
#define FEI_COMPILER_LR0_H
#define MAX 800
#include <fstream>
# include <vector>
# include "GrammarRule.h"
using namespace std;
class LR0Action{

};

class StateSet{

};
class LeftRightZero{
public:
    // get grammar rules from file
    LeftRightZero(ifstream &grammar_file);
    void get_grammars(ifstream &target_file);
    void generate_lr0_item();
    void construct_table();
    void MakeDFA();
    void gram_analyse();

private:
    // Store the grammar rules;
    vector<GrammarRule> original_rule_set;
    // Process the grammar rules and get lr0 items
    vector<GrammarRule> rule_lr0_set;
    // store the actions under lr0 rules
    vector<LR0Action> action[MAX];
    vector<StateSet> DFA;
};
#endif //FEI_COMPILER_LR0_H
