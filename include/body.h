#ifndef BODY_H
#define BODY_H

#include <iostream>
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
    std::string m_firstSet;

public:
    char GetFirstCharacter();
    // 候选式首符是否为终结符
    bool IsTerminal();
    // 清空候选式
    void Clear();
};

#endif // BODY_H