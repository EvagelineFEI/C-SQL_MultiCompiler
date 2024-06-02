//
// Created by yys on 2024-04-22.
//
#ifndef FEI_COMPILER_WORDANALYSE_H
#define FEI_COMPILER_WORDANALYSE_H
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stack>
using namespace std;
class WordAnalyse{
public:
    WordAnalyse(string language, string source_file, string target_file="target_languages-info/c_key_words.txt");
    void words_analyse();
    void result(ofstream &file);

    void get_key_words(ifstream &target_file); // Get key-words of target language from file
    void show_key_words(ofstream &file);
    string get_token();
    map<pair<string, string>, pair<int, int>> error_list; //Like: ERRORLIST[{"SyntaxError", "Unexpected character"}] = {10, 15};
private:
    ifstream source_file;
    ifstream target_file;
    string target_language;
    vector<pair< pair< string, string >,pair<int,int> >> Words; //Words carry tokens, which is the result of word-analyse
    map<string, string> key_words; //store key words
    map<string, string> key_words_sql; //store key words

    set<string> key_occur; // key words that occured
    set< string > IDs; // store identifiers
    set< string > Signs;//
    char ch{};
    bool read = false;
    int temp{};
    string string_get;  // the string that has been read
    pair<int, int> position;   //retain the current reading position<line, column>
};
#endif //FEI_COMPILER_WORDANALYSE_H
