#ifndef BODY_H
#define BODY_H

#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

// 候选式

class Body
{
private:
    // 候选式
    std::string m_expression;
    // 候选式 FIRST 集
    // std::string m_firstSet;
    std::set<std::string> m_firstSet;

public:
    // 获得候选式
    std::string GetExpression();
    // 向候选式中添加数据
    void SetExpression(char c);
    // 获取 FirstSet
    std::set<std::string> GetFirstSet();
    // 向 FirstSet 中添加数据
    void SetFirstSet(std::string c);
    // 获取候选式首符
    std::string GetFirstSymbol();
    // 候选式首符是否为终结符
    bool IsTerminal();
    // 清空候选式
    void Clear();
};

#endif // BODY_H