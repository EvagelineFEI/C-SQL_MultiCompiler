#include <iostream>
#include "WordAnalyse.h"
#include "LR0.h"
int main() {
    string language;
    cout << "Please input the language you want to parse������Ŀ���������" << endl;
    cin >> language;
    string file_src;
    cout << "Please input the source file name����������ļ�����" << endl;
    cin >> file_src;
    string file_tar;
    cout << "Please input the key-words file of the target language�����������Ŀ�����Թؼ����ļ�" << endl;
    cin >> file_tar;
    //�ʷ�����
    WordAnalyse word( language, file_src,file_tar);
    word.words_analyse();
    ofstream fout;
    fout.open("../result/" + file_src + "_Word_result.txt", ios::out);
    word.result(fout);
//    if (!word.error_list.empty())
//    {
//        cout << "Word analyse ERROR!�ʷ������д�����������Ӧλ��"<<endl;
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
