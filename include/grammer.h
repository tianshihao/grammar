#ifndef GRAMMER_H
#define GRAMMER_H

#include "production.h"
#include <QtCore>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

// 文法
class Grammer
{
private:
    // 非终结符号集
    std::set<char> VN;
    // 终结符号集
    std::set<char> VT;
    // 开始符号
    Body m_start;
    // 产生式集
    std::vector<Production> m_productionSet;

    int m_type;

    //  vocabulary of terminal，vocabulary of nonterminal
public:
    Grammer();
    Grammer(std::string inputText);

    void SetType(int type);
    int GetType();

    void GetFirstSet();
    void DFSTraverse(Body &, char);
    void GetFollowSet();

    void ParseText(std::string inputText);
    void Print();
};

#endif