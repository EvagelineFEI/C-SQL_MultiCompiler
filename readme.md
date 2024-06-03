# Fei-Compiler: An Attempt to Build a Multi-language Lexical and Syntax Analyzer

## Project Concept

The class task is to build a lexical and syntax compiler for C-minus. After doing some research, I found that C-minus is compatible with C, so it naturally led me to consider whether it would be possible to implement C directly, thus covering two languages at once.

After reviewing the source code of the TINY compiler, I noticed that it hardcodes all the lexical keywords directly into the code (although this seems to be the approach of most mainstream compilers). This led me to wonder if it would be possible to store the lexical keywords and grammar rules in a file instead of hardcoding them, allowing users to compile whichever language they want by specifying the corresponding files.

Therefore, I added a few interfaces: the language type in the analysis class is specified by the user; the keyword and grammar files are also specified by the user; functions are provided to read keywords and grammar rules from these files.

## Implementation Status

Lexical analysis is relatively straightforward, and it currently supports tokenization for C-minus, C, and SQL.

The syntax analysis adopts the LR0 method, and the implementation approach is akin to solving problems: constructing LR0 items, constructing the DFA, and constructing the DFA's connections. (If any experts happen to come across this and have better implementation ideas, please feel free to contact me.)

At present, the syntax analysis does not yet support C and SQL. However, to add support, it would only require adding some judgments in the lexical analysis part according to the keyword categories appearing in the grammar rules of C and SQL (for example, numbers in C-minus grammar belong to the "NUM" category, while in C grammar they belong to the "integer-constant" or "floating-constant" categories).



# Fei-Compiler试图构建一个支持多语言的词法-语法分析器

## 项目构思

课堂任务是构建C-minus的词法和语法编译器，查资料后会发现C-minus是被C兼容的，所以很自然地会想能不能直接实现C的，这样同时就覆盖了两种语言。

看了TINY编译器的源码之后，发现它是把所有词法关键字写死了，堆在代码里（虽然目前主流编译器似乎也都是这样做的）；但是就让人又自然地想，如果把词法关键字和文法规则放在文件里而不是写死，用户就可以想编译什么语言就用什么语言了。

于是增加了几个接口：分析类中的语言类别由用户指定；关键字和文法文件也由用户指定；从文件中读取关键字和文法的函数。

## 实现情况

词法分析比较简单，目前是支持对C-，C和SQL的分词；

语法分析采用的LR0方法，实现思路就是像做题一样的：构造LR0项，构造DFA，构造DFA的连接情况；（如果大佬路过有更好的实现思路，欢迎滴滴我）。

目前语法分析还不支持C和SQL，但是想要支持的话只需要在词法分析部分，对照C和SQL的文法规则中出现的关键字类别新增一些判断即可（比如数字在C-minus的文法中属于“NUM”类别，而在C的文法中属于“integer-constant”或者“floating-constant”类别）。

