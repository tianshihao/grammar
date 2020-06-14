#ifndef GRAMMER_H
#define GRAMMER_H

#include "production.h"
#include "table.h"
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
    std::set<std::string> m_setVN;
    // 终结符号集, vocabulary of terminal
    std::set<std::string> m_setVT;
    // 开始符号
    Body m_bodyStart;
    // 产生式集
    std::vector<Production> m_vecProductionSet;
    // 文法类型
    int m_nType;
    // 分析表
    Table m_tablePharsingTable;

public:
    /**
     *  构造函数.
     */
    Grammer();

    /**
     *  @brief  构造函数重载.
     */
    Grammer(std::string __strInputText);

    /**
     *  @brief  解析输入文本.
     */
    void ParseText(std::string __strInputText);

    /**
     *  合并相同左部的产生式.
     */
    bool MergeProduction();

    /**
     *  计算 FIRST 集.
     */
    void CalcFirstSet();

    /**
     *  @brief  DFS.
     */
    void DFS(Body &__bodyOrigin, std::string __strFirstSymbol);

    /**
     *  计算 FOLLOW 集.
     */
    void CalcFollowSet();

    /**
     *  @brief  我有一个神奇的函数, 输入变量, 解决问题.
     *  @param __str  源字符串.
     *  @param __nIndex  开始判断的位置索引.
     *  @return  返回符号的类型.
     */
    int MagicFunction(std::string __str, int __nIndex);

    /**
     *  @brief  返回给定的非终结符在产生式左部中所属的 Body.
     */
    Body FindLeftBody(std::string __strBody);

    /**
     *  @brief  返回给定的非终结符在产生式左部中所属的 Body &.
     */
    Body &FindLeftBody(std::string __strBody, bool __bRefs);

    /**
     *  计算 LL(1) 文法分析表.
     */
    void CalcLLPrasingTable();

    /**
     *  获取文法类型.
     */
    int GetType();

    /**
     *  设置文法类型.
     */
    void SetType(int __nType);

    /**
     *  打印文法.
     */
    void PrintGrammer();
};

#endif