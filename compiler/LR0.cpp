//
// Created by yys on 2024-05-19.
//
#include <sstream>
# include "LR0.h"
# include "GrammarRule.h"
#include <algorithm>
#include <iostream>
using namespace std;
char CH = '.';
string get_symbol(string &s, size_t idx){
    string symbol;
    size_t find = idx,i=0;
    while(s[find]!=' ' && find<s.length())
    {
        symbol+=s[find];
        find++;
    }
    return symbol;
}

size_t get_symbol_idx(string &s){
    size_t idx = 0;
    while(s[idx]!=' ' && idx<s.length())idx++;
    return idx;
}
StateSet::StateSet(int idx)
{
    state_idx = idx;
    state_type = 's';  // 默认为移进
}

LeftRightZero::LeftRightZero(string  language, string gram_file) {
    target_language = language;
    grammar_file.open("../target_languages-info/"+gram_file, ios::in);
}

void LeftRightZero::get_grammars() {
    //如果lgg=='c-'
    //打开文件，如果开头是序号(如“1.”)指针移动三格
    //如果匹配到符号“->”,就把文法分为两部分，符号左边部分和符号右边部分
    //继续读符号右边部分，直到读到“|”或者换行符;
    //如果是“|”，新建GrammarRule对象，把符号左边部分放到left，“|”或者换行符前面的内容放到right;若是换行符则读下一行的文法，若是“|”
    //则继续这样的操作，直到读到换行符
    if(target_language == "c-")
    {
        string first_line;
        string line;
        getline(grammar_file,first_line);
        size_t pos_ = 0;
        if(isdigit(first_line[0]) && first_line[1]=='.')
        {
            pos_ += 3;
        }
        auto divide_ = first_line.find_first_of("->");
        string first_symbol = first_line.substr(pos_, divide_-4);
        string first_right = first_line.substr(divide_+3);
        GrammarRule first_rule(first_symbol,first_right,0,-1);
        start_symbol = first_symbol + "'";
        GrammarRule start_rule(start_symbol,first_symbol,0,-1);
        original_rule_set.push_back(start_rule);
        original_rule_set.push_back(first_rule);
        while(getline(grammar_file,line))
        {
            if (line.empty()) continue;
            size_t pos = 0;
            int i=0;
            while(isdigit(line[i]) || line[i]=='.')
            {
                i++;
                pos++;
            }
            pos++;
            if(pos < line.length())
            {
                auto divide = line.find_first_of("->");
                size_t begin = divide+3;
                string left = line.substr(pos, divide-pos-1);
                string right = line.substr(divide+3);
                size_t or_pos = right.find("|");
                while(or_pos != string::npos)
                {
                    string right0 = right.substr(0, or_pos-2);
                    GrammarRule rule(left,right0,0,-1);
                    original_rule_set.push_back(rule);
//                    begin = or_pos+1;
                    right = right.substr(or_pos+3);
                    begin = or_pos+2;
                    or_pos = right.find("|");
                    if(or_pos==string::npos){
                        GrammarRule rule0(left,right,0,-1);
                        original_rule_set.push_back(rule0);
                        break;
                    }
                }
                if(line.find("|") == string::npos)
                {
                    GrammarRule rule(left,right,0,-1);
                    original_rule_set.push_back(rule);
                }
            }

        }
    }
    //如果lgg=='c'，开始读取每一行，空行则跳过
    //如果匹配到符号“::=”,就把文法分为两部分，符号左边部分和符号右边部分,
    //新建GrammarRule对象，把符号左边部分放到left,右边部分放到right
    //换行，如果不是空行，则新建GrammarRule对象，把符号左边部分放到left,“|”右边部分放到right；重复，直到遇见空行
    if(target_language == "c")
    {
        string first_line;
        getline(grammar_file,first_line);
        size_t pos_ = 0;
        auto divide_ = first_line.find_first_of("::=");
        string first_symbol = first_line.substr(0, divide_);
        start_symbol = first_symbol + "'";
        GrammarRule start_rule(start_symbol,first_symbol,0,-1);
        original_rule_set.push_back(start_rule);
        string line;
        string left;
        while(getline(grammar_file,line))
        {
            if (line.empty()) continue;
            if(line.find('|')!=string::npos)
            {
                string right = line.substr(line.find('|')+1);
                GrammarRule rule(left,right,0,-1);
                original_rule_set.push_back(rule);
            }
            else
            {
                auto divide = line.find_first_of("::=");
                left = line.substr(0, divide);
                string right = line.substr(divide+4);
                GrammarRule rule(left,right,0,-1);
                original_rule_set.push_back(rule);
            }
        }
    }

}

void LeftRightZero::show_rules() {
    for(auto rule:original_rule_set)
    {
        cout <<rule.left<<" "<<rule.right<<endl;
    }
}
void LeftRightZero::generate_lr0_item() {
    for(auto rule: original_rule_set)
    {
        string temp = rule.right; // 用于加‘.’
        string temp1 = rule.right; // 用于分隔
        size_t idx=0;
        int insert = 0;
        while(idx<=temp.length())
        {
            idx -= insert;
            string temp0 = rule.right;
            if(temp0[idx]==' ')
            {
                temp0[idx]=CH;
                temp1[idx+insert]=CH;
            }
            else
            {
                temp0.insert(idx,1, CH);
                temp1.insert(idx+insert,1, CH);
                insert+=1;
            }
            GrammarRule lr0=GrammarRule(rule.left,temp0,0,-1);
            rule_lr0_set.push_back(lr0);
            // 找到下一个符号的起始位置
            idx = get_symbol_idx(temp1);
            if(idx+1>temp.length())
            {
                string temp_ = rule.right;
                temp_ = temp_.insert(temp_.length(),1, CH);
                GrammarRule lr0_=GrammarRule(rule.left,temp_,0,-1);
                rule_lr0_set.push_back(lr0_);
                break;
            }
        }
    }
}

void LeftRightZero::get_none_end_symbol() {
    //先找非终结符
    for(auto rule:original_rule_set)
    {
        auto it = count(none_end_symbol.begin(), none_end_symbol.end(), rule.left);
        if(it==0)none_end_symbol.push_back(rule.left);
    }
}

void LeftRightZero::get_end_symbol(string &a) {
    // 再找终结符
    istringstream iss(a);  // 从字符串中读取数据
    string token;
    while (iss >> token) {
        // 检查token是否在非终结符中
        bool found = false;
        for (const string& str : none_end_symbol) {
            if (token == str) {
                found = true;
                break;
            }
        }
        // 如果没有在B中找到token，则将其添加到终结符中
        if (!found) {
            end_symbol.push_back(token);
        }
    }
}
void LeftRightZero::makeDfa() {
    int i = 0;
    for (auto &rule:rule_lr0_set)
    {
        StateSet state(i);
        if(rule.back==1)continue;
        state.rules.push_back(rule);
        rule.back=1;
        rule_processor(state,rule);
        DFA.push_back(state);
        i++;
    }
}

void LeftRightZero::rule_processor(StateSet &state, GrammarRule &rule) {
    //如果‘.’在最后，跳过
    size_t idx = rule.right.find(CH);
    if(idx==rule.right.length()-1)
    {
        printf("%s",&rule.right);
        //说明是规约项
        state.state_type = 'r';
        return;
    }
    // 如果‘.’后面是非终结符;加入相关规则
    string str = rule.right;
    string symbol = get_symbol(str,idx+1);
    auto it = find(none_end_symbol.begin(), none_end_symbol.end(), symbol);
    if(it != none_end_symbol.end())
    {
        //在rule_lr0_set中找到该终结符的‘.’开头规则
        for(auto &item:rule_lr0_set)
        {
            if(item.back==0 && item.left==symbol)
            {
                if(item.right[0]==CH)
                {
                    state.rules.push_back(item);
                    item.back=1;
                    rule_processor(state, item);
                }
            }
        }
    }
    else return;
}
int LeftRightZero::find_state(GrammarRule rule) {
    string left = rule.left;
    string right = rule.right;
    for(auto state:DFA)
    {
        vector<GrammarRule> rule_in_state=state.rules;
        for(auto rule:rule_in_state)
        {
            if(rule.left!=left)continue;
            if(rule.right==right)return state.state_idx;
        }
    }
}
void LeftRightZero::make_goto()
{
    for(auto state :DFA)
    {
        vector<GrammarRule> rule_in_state=state.rules;
        for (auto  rule:rule_in_state)
        {
            string temp = rule.right;
            //如果’.‘后面还有符号，就移进或者规约
            if(temp.back()!=CH)
            {
                // 获取’.‘后面的符号
                size_t pos = temp.rfind(CH); // rfind返回最后一次出现的下标
                string symbol = get_symbol(temp,pos+1);
                // 找到移进之后所在的状态
                string new_right;
                if(pos==0)new_right = temp.erase(pos,1);
                else
                {
                    // 先删除pos位置上的字符
                    new_right = temp.erase(pos, 1);
                    // 然后在pos位置上插入一个空格
                    new_right.insert(pos, " ");
                    pos+=1;
                }
                size_t next_pos = pos + symbol.length();
                if (next_pos < new_right.size() && new_right[next_pos] == ' ') {
                    // 如果symbol后面是空格，替换为空格
                    new_right.replace(next_pos, 1, ".");
                } else {
                    // 如果symbol后面没有内容或不是空格，直接插入'.'
                    new_right.insert(next_pos, ".");
                }
                GrammarRule temp_rule(rule.left, new_right,0,-1);
                // goto
                int idx = find_state(temp_rule);
                go_to[make_pair(state.state_idx,symbol)] = idx;
            }
        }
    }
}
//void LeftRightZero::construct_table() {
//}
// 辅助函数：记录当前步骤
void LeftRightZero::record_step(ofstream &outfile, const string &operation) {
    // 记录分析栈
    stack<string> temp_stack(analyse_stack);
    vector<string> analyse_stack_vec;
    while (!temp_stack.empty()) {
        analyse_stack_vec.push_back(temp_stack.top());
        temp_stack.pop();
    }
    reverse(analyse_stack_vec.begin(), analyse_stack_vec.end());
    for (const auto &elem : analyse_stack_vec) {
        outfile << elem << " ";
    }
    outfile << "\t"<<"----------------------";

    // 记录输入栈
    temp_stack = input_stack;
    vector<string> input_stack_vec;
    while (!temp_stack.empty()) {
        input_stack_vec.push_back(temp_stack.top());
        temp_stack.pop();
    }
    reverse(input_stack_vec.begin(), input_stack_vec.end());
    for (const auto &elem : input_stack_vec) {
        outfile << elem << " ";
    }
    outfile << "\t"<<"----------------------";

    // 记录操作
    outfile << operation << "\n";
}
void LeftRightZero::gram_analyse(string token) {
    //
    get_grammars();
    get_none_end_symbol();
    generate_lr0_item();
    makeDfa();
    make_goto();

    ofstream outfile("../result/LR0_analyse_result.txt");

    istringstream iss(token);
    string word;
    stack<string> temp_input;
    while (iss >> word) {
        temp_input.push(word);
    }
    while(!temp_input.empty())
    {
        input_stack.push(temp_input.top());
        temp_input.pop();
    }
    analyse_stack.push("0");
    while (!input_stack.empty())
    {
        string symbol = input_stack.top();
        string now_state = analyse_stack.top();
        bool action_taken = false;
//        pair<int, string> go(state[0]-'0', symbol);
//        int next_state = go_to[go];
        StateSet state = DFA[stoi(now_state)];
        if(to_string(state.state_idx)==now_state)
        {
            if(state.state_type!='r') // 根据实际情况移进
            {
                pair<int, string> go(stoi(now_state), symbol);
                int next_state = go_to[go];
                analyse_stack.push(symbol);
                analyse_stack.push(to_string(next_state));
                input_stack.pop();

                record_step(outfile, "移进");
                action_taken = true;
            }
            else
            {
//                    for(auto rule:state.rules)
//                    {
//                        cout<<rule.left<<" "<<rule.right<<endl;
//                    }
                GrammarRule rule = state.rules[0];
                string symbol_in = rule.left;
                string symbol_out = rule.right;
                symbol_out.pop_back();
                // 把规约项symbol_out中的内容顺序放入一个临时栈；
                istringstream get_single(symbol_out);
                stack<string> temp;
                string temp0;
                while (get_single >> temp0) {
                    temp.push(temp0);
                }
                while(!temp.empty())
                {
                    if(analyse_stack.top()!=temp.top())analyse_stack.pop();
                    else
                    {
                        analyse_stack.pop();
                        temp.pop();
                    }
                }
                string state_top = analyse_stack.top();
                analyse_stack.push(symbol_in);
                pair<int, string> go(stoi(state_top), symbol_in);
                int next_state = go_to[go];
                analyse_stack.push(to_string(next_state));
                action_taken = true;
                record_step(outfile, "规约");

            }

        }

        if(!action_taken)
        {
            outfile<<"Something goes wrong!"<<endl;
            break;
        }
    }


    outfile.close();
}