#ifndef BODY_H
#define BODY_H

#include <iostream>
#include <string>
#include <vector>

// 候选式
class Body
{
private:
    // 候选式
    std::string m_expression;
    // 候选式产生式集
    std::string m_firstSet;

public:
    // 向候选式中添加数据
    void SetExpression(char c);
    // 获得候选式
    std::string GetExpression();
    // 向 FirstSet 中添加数据
    void SetFirstSet(char c);
    // 获取 FirstSet
    std::string GetFirstSet();
    // 清空候选式
    void Clear();
    // 候选式首符是否为终结符
    bool IsTerminal();
};

#endif // BODY