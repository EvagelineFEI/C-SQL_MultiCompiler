//
// Created by yys on 2024-05-19.
//
# include "LR0.h"
# include "GrammarRule.h"
using namespace std;
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
        string line;
        while(getline(grammar_file,line))
        {
            if (line.empty()) continue;
            size_t pos = 0;
            if(isdigit(line[0]) && line[1]=='.')
            {
                pos += 3;
            }
            if(pos < line.length())
            {
                auto divide = line.find_first_of("->");
                size_t begin = divide+2;
                string left = line.substr(0, divide);
                string right = line.substr(divide+2);
                size_t or_pos = line.find("|");
                while(or_pos != string::npos)
                {
                    string right0 = line.substr(begin, or_pos);
                    GrammarRule rule(left,right0,-1,-1);
                    original_rule_set.push_back(rule);
                    begin = or_pos+1;
                    right = right.substr(or_pos+1);
                    or_pos = right.find("|");
                }
                if(line.find("|") == string::npos)
                {
                    GrammarRule rule(left,right,-1,-1);
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
        string line;
        string left;
        while(getline(grammar_file,line))
        {
            if (line.empty()) continue;
            if(line.find('|')!=string::npos)
            {
                string right = line.substr(line.find('|')+1);
                GrammarRule rule(left,right,-1,-1);
                original_rule_set.push_back(rule);
            }
            else
            {
                auto divide = line.find_first_of("::=");
                left = line.substr(0, divide);
                string right = line.substr(divide+3);
                GrammarRule rule(left,right,-1,-1);
                original_rule_set.push_back(rule);
            }
        }
    }

}

void LeftRightZero::generate_lr0_item() {

}

void LeftRightZero::MakeDFA() {

}

void LeftRightZero::construct_table() {

}

void LeftRightZero::gram_analyse() {

}