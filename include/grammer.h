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
    // 非终结符号集, vocabulary of nonterminal
    std::set<char> VN;
    // 终结符号集, vocabulary of terminal
    std::set<char> VT;
    // 开始符号
    Body m_start;
    // 产生式集
    std::vector<Production> m_productionSet;
    // 文法类型
    int m_type;

public:
    // 构造函数
    Grammer();
    // 构造函数重载
    Grammer(std::string inputText);
    // 解析输入文本
    void ParseText(std::string inputText);
    // 计算 FIRST 集
    void CalcFirstSet();
    // DFS
    void DFS(Body &, char);
    // 计算 FOLLOW 集
    void CalcFollowSet();
    // 获取文法类型
    int GetType();
    // 设置文法类型
    void SetType(int type);
    // 打印文法
    void Print();
};

#endif