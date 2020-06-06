#ifndef BODY_H
#define BODY_H

#include <iostream>
#include <set>
#include <string>
#include <vector>

// 候选式
class Body
{
private:
    // 候选式
    std::string m_expression;
    // 候选式 FIRST 集
    std::set<char> m_firstSet;
    // 非终结符 FOLLOW 集
    std::set<char> m_followSet;

public:
    // 获得候选式
    std::string GetExpression();

    // 设置候选式
    void SetExpression(char ch);

    // 获取候选式的 First 集
    std::set<char> GetFirstSet();

    // 向候选式的 First 集中添加终结符
    void SetFirstSet(char c);

    // 获得非终结符的 FOLLOW 集
    std::set<char> GetFollowSet();

    // 向非终结符的 FOLLOW 集中添加终结符
    void SetFollowSet(char);

    // 获取候选式首符
    std::string GetFirstSymbol();

    // 候选式首符是否为终结符
    bool IsTerminal();

    // 清空候选式
    void Clear();
};

#endif // BODY_H