//
// Created by yys on 2024-05-19.
//

#ifndef FEI_COMPILER_GRAMMARRULE_H
#define FEI_COMPILER_GRAMMARRULE_H
using namespace std;
class GrammarRule{
public:
    string left,right;
    int back;  // 用于标记是否已经放入dfa中，防止规则重复放入(0无，1有)
    int id;
    GrammarRule ( char s1[] , char s2[] , int x , int y )
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
    }
    GrammarRule ( const string& s1 , const string& s2 , int x , int y )
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
    }

};
#endif //FEI_COMPILER_GRAMMARRULE_H
