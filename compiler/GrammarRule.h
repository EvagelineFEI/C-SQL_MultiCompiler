//
// Created by yys on 2024-05-19.
//

#ifndef FEI_COMPILER_GRAMMARRULE_H
#define FEI_COMPILER_GRAMMARRULE_H
using namespace std;
class GrammarRule{
public:
    string left,right;
    int back;
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
