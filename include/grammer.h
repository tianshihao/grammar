#ifndef GRAMMER_H
#define GRAMMER_H

#include "production.h"
#include "table.h"
#include <QtCore>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <tuple>
#include <vector>

// 文法
class Grammer
{
private:
    // 非终结符号集, vocabulary of nonterminal
    std::set<std::string> m_VN;
    // 终结符号集, vocabulary of terminal
    std::set<std::string> m_VT;
    // 开始符号
    Body m_start;
    // 产生式集
    std::vector<Production> m_productionSet;
    // 文法类型
    int m_type;
    // 分析表
    Table m_pharsingTable;

public:
    // 构造函数
    Grammer();
    // 构造函数重载
    Grammer(std::string inputText);
    // 解析输入文本
    void ParseText(std::string inputText);
    // 合并相同左部的产生式
    bool MergeProduction();
    // 计算 FIRST 集
    void CalcFirstSet();
    // DFS
    void DFS(Body &originBody, std::string firstSymbol);
    // 计算 FOLLOW 集
    void CalcFollowSet();

    /**
     *  @brief  我有一个神奇的函数, 输入变量, 解决问题
     *  @param str  源字符串
     *  @param index  开始判断的位置索引
     *  @return  返回符号的类型
     */
    int MagicFunction(std::string str, int index);
    // 返回给定的非终结符在产生式左部中所属的 Body
    Body FindLeftBody(std::string body);
    // 返回给定的非终结符在产生式左部中所属的 Body &
    Body &FindLeftBody(std::string body, bool bRefs);

    /**
     *  @brief  计算 LL(1) 文法分析表
     */
    void CalcLLPrasingTable();
    // 获取文法类型
    int GetType();
    // 设置文法类型
    void SetType(int type);
    // 打印文法
    void Print();
};

#endif