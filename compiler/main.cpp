#include <iostream>
#include "WordAnalyse.h"
#include "LR0.h"
int main() {
    string language;
    cout << "Please input the language you want to parse请输入目标解析语言" << endl;
    cin >> language;
    string file_src;
    cout << "Please input the source file name请输入测试文件名称" << endl;
    cin >> file_src;
    string file_tar;
    cout << "Please input the key-words file of the target language请输入解析的目标语言关键字文件" << endl;
    cin >> file_tar;
    //词法分析
    WordAnalyse word( language, file_src,file_tar);
    word.words_analyse();
    ofstream fout;
    fout.open("../result/" + file_src + "_Word_result.txt", ios::out);
    word.result(fout);
//    if (!word.error_list.empty())
//    {
//        cout << "Word analyse ERROR!词法分析有错误，请检查代码对应位置"<<endl;
//    }
    string tokens = word.get_token();
    cout<< "tokens--------------"<<endl;
    cout<<tokens;

//    ofstream fout_d;
//    fout_d.open("../result/" + file_src + "_Word_divid.txt", ios::out);
//    word.show_key_words(fout_d);

//    printf("%s",&tokens);
    LeftRightZero grammar_analyse("c-","CMinusGrammar.txt");
    grammar_analyse.gram_analyse(tokens);
    return 0;
}
