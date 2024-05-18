//
// Created by yys on 2024-04-22.
//
#include "WordAnalyse.h"
#include <cctype>
#include <iomanip>

#define GET ch = source_file.get()  // get a char from the file
#define PEEK ch = source_file.peek()  // read the next char from the file
#define ADD(c)  string_get.append(1, c) // add a char at the end of a string
#define PUSH(s1,s2) newitem =  make_pair(s1,s2);\
    wordcheck = make_pair(newitem,position);\
    Words.push_back(wordcheck);

WordAnalyse::WordAnalyse(string language, string src_file, string tar_file):target_language(language),position(1,1) {
    source_file.open("../test/" + src_file, ios::in);
    target_file.open("../target_languages-info/" +tar_file, ios::in);

    if (!source_file.is_open()) {
        cerr << "Failed to open Source Code file_tar: " << src_file << std::endl;
        return;  //
    }
    if (!target_file.is_open()) {
        cerr << "Failed to open Target Language file_tar: " << tar_file << std::endl;
        return;  //
    }

    get_key_words(target_file);

}

void WordAnalyse::get_key_words(ifstream &target_file){
    // get key words from target file
    string line;
    string lgg;
    lgg.reserve(target_language.size());
    for (char c : target_language) {
        lgg += tolower(c);
    }
    if (lgg == "c")
    {
        while (getline(target_file, line)) {
            string line_upper;
            line_upper.reserve(line.size());
            for (char c : line) {
                line_upper += toupper(c);
            }
            key_words.insert({line, line_upper});
        }
    }
    if (lgg == "sql")
    {
        while (getline(target_file, line)) {   // 支持小写
            string line_lower;
            line_lower.reserve(line.size());
            for (char c : line) {
                line_lower += tolower(c);
            }
            key_words_sql.insert({line_lower, line}); // 支持大写
        }
        while (getline(target_file, line)) {
            key_words.insert({line, line});
        }
    }

}

void WordAnalyse::words_analyse() {
    pair <string, string> newitem;
    pair < pair <string, string>, pair<int, int> > wordcheck;

    GET;  // begin to read
    while (ch != EOF)  // Get rid of blank space
    {
        while(isspace(ch))
        {
            if(ch == ' ')position.second++;
            else if (ch == '\n')
            {
                position.first++;
                position.second=1;
            }
            GET;
        }
        if (isdigit(ch))  // 读取完整的数字
        {
           temp = ch - '0';
           for(GET; isdigit(ch); GET)
           {
              temp = temp*10 + ch - '0';
              position.second ++;
           }
            PUSH(to_string(temp),"NUM");
        }

        else if (isalpha(ch))   // 读取完整的字符串
        {
           string_get.clear();
           ADD(ch);
           for (GET; isalnum(ch);GET)
           {
               ADD(ch);
               position.second++;
           }

            auto is_in = key_words.find(string_get);

            if (target_language == "sql")
            {
                auto is_in_lower = key_words_sql.find(string_get);
                if (is_in == key_words.end() && is_in_lower == key_words_sql.end())  // not found in key words,then it's an identifier
                {
                    PUSH(string_get,"ID");
                    IDs.insert(string_get);
                }
                else
                {
                    if (is_in != key_words.end())
                    {
                        PUSH(is_in->first, is_in->second);
                        key_occur.insert(string_get);
                    }
                    else
                    {
                        if (is_in_lower != key_words_sql.end())
                        {
                            PUSH(is_in_lower->first, is_in_lower->second);
                            key_occur.insert(string_get);
                        }
                    }
                }
            }
            else
            {
                if (is_in == key_words.end())  // not found in key words,then it's an identifier
                {
                    PUSH(string_get,"ID");
                    IDs.insert(string_get);
                }
                else
                {
                    PUSH(is_in->first, is_in->second);
                    key_occur.insert(string_get);
                }
            }

        }

        else
        {
            switch (ch)
            {
                case '&':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '&')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "&&");
                        Signs.insert("&&");
                        position.second++;
                        break;
                    }   // &&
                    else
                    {
                        PUSH(string_get, "&");
                        Signs.insert("&");
                        break;
                    }
                case '|':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '|')   //  ||
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "||");
                        Signs.insert("||");
                        position.second++;
                        break;
                    }
                    else
                    {
                        PUSH(string_get, "|");
                        Signs.insert("|");
                        break;
                    }
                case '(':
                PUSH("(", "(");
                    Signs.insert("(");
                    break;
                case ')':
                PUSH(")", ")");
                    Signs.insert(")");
                    break;
                case ';':
                PUSH(";", ";");
                    Signs.insert(";");
                    break;
                case '+':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '=')   // +=
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "+=");
                        Signs.insert("+=");
                        position.second++;
                        break;
                    }
                    else if (ch == '+')   // ++
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "++");
                        Signs.insert("++");
                        position.second++;
                        break;
                    }
                    else
                    {
                        PUSH(string_get, "+");
                        Signs.insert("+");
                        break;
                    }
                case '-':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '=')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "-=");
                        Signs.insert("-=");
                        position.second++;
                        break;
                    }
                    else if (isdigit(ch))   // a minus number
                    {
                        GET;
                        temp = ch - '0';            // change char type into integer type
                        for (GET; isdigit(ch); GET)
                        {
                            temp = temp * 10 + ch - '0';
                            position.second++;
                        }
                        PUSH( "-" + to_string(temp), "NUM");
                        read = true;
                        break;
                    }
                    else if (ch == '-')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "--");
                        Signs.insert("--");
                        position.second++;
                        break;
                    }
                    else
                    {
                        PUSH(string_get, "-");
                        Signs.insert("-");
                        break;
                    }
                case '*':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '=')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "*=");
                        Signs.insert("*=");
                        position.second++;
                        break;
                    }
                    else
                    {
                        PUSH(string_get, "*");
                        Signs.insert("*");
                        break;
                    }
                case '/':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '=')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "/=");
                        Signs.insert("/=");
                        position.second++;
                        break;
                    }
                    else
                    {
                        PUSH(string_get, "/");
                        Signs.insert("/");
                        break;
                    }
                case '=':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '=')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "==");
                        Signs.insert("==");
                        position.second++;
                        break;
                    }
                    else
                    {
                        PUSH(string_get, "=");
                        Signs.insert("=");
                        break;
                    }
                case '<':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    switch (ch)
                    {
                        case '=':
                            ADD(ch);
                            GET;
                            PUSH(string_get, "<=");
                            Signs.insert("<=");
                            position.second++;
                            break;
                        case '<':
                            ADD(ch);
                            GET;
                            PUSH(string_get, "<<");
                            Signs.insert("<<");
                            position.second++;
                            break;
                        default:
                        PUSH(string_get, "<");
                            Signs.insert("<");
                            break;
                    }
                    break;
                case '#':
                    string_get.clear();
                    ADD(ch);
                    PUSH(string_get, "BEGIN");
                    Signs.insert("#");
                    break;
                case '>':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '=')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, ">=");
                        Signs.insert(">=");
                        position.second++;
                        break;
                    }
                    else if (ch == '>')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, ">>");
                        Signs.insert(">>");
                        position.second++;
                        break;
                    }
                    else
                    {
                        PUSH(string_get, ">");
                        Signs.insert(">");
                        break;
                    }
                case '{':
                {
                    PUSH("{", "{");
                    Signs.insert("{");
                    break;
                }
                case '}':
                {
                    PUSH("}", "}");
                    Signs.insert("}");
                    break;
                }
                case '!':
                    string_get.clear();
                    ADD(ch);
                    PEEK;
                    if (ch == '=')
                    {
                        ADD(ch);
                        GET;
                        PUSH(string_get, "!=");
                        Signs.insert("!=");
                        position.second++;
                        break;
                    }
                    else
                    {
                        newitem = make_pair(string_get, "不合法字符");
                        error_list.insert({ newitem,position });
                        break;
                    }

                default:
                    string_get.clear();
                    ADD(ch);
                    newitem = make_pair(string_get, "不合法字符");
                    error_list.insert({ newitem,position });
            }
            position.second++;
            if (!read)
            {
                GET;  // 此处需要再 get 一个新的字符备用
            }
            else
            {
                read = false;
            }
        }
    }
    PUSH("$", "FINISH");

}


void WordAnalyse::result(ofstream &file) {
    cout << "Showing the result of Word Analyse词法分析结果" << endl  << endl;
    for (auto & Word : Words)
    {
       file << "Token字符值"<<setiosflags(ios::left)<< setw(6)<< Word.first.first<<"  "
       <<"TokenType 符号类别"<<setw(6)<< Word.first.second<<"  "<< "位置:" << Word.second.first<< ","<< Word.second.second << endl;
    }
    if(!error_list.empty())
    {
        cout<< "存在下列的错误输入"<<endl;
        for (auto & i : error_list)
        {
            cout << i.first.second  << setw(6) << i.first.first << "    ";
            cout << "位置：" << i.second.first << "行" << i.second.second <<"列" << endl;
        }
    }
}

void WordAnalyse::show_key_words(ofstream &file) {
    file << "-----------------Key words Table-----------------"<<endl;
    for (auto &key_word :key_occur)
    {
        file << "Token "<< setiosflags(ios::left)
                       << setw(6) << key_word << "Type: Key-words" << endl;
    }

    file << "-----------------Identifiers Table---------------"<<endl;
    for (auto &id :IDs)
    {
        file << "Token "<< setiosflags(ios::left)
             << setw(6) << id << "Type: ID" << endl;
    }

    file << "------------------Signs Table--------------------"<<endl;
    for (auto &sign :Signs)
    {
        file << "sign "<< setiosflags(ios::left)
             << setw(6) << sign << "Type: sign" << endl;
    }

}

