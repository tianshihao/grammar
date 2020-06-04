#include "body.h"

// 获得候选式
std::string Body::GetExpression()
{
    return this->m_expression;
}

// 向候选式中添加数据
void Body::SetExpression(char c)
{
    m_expression.push_back(c);
}

// 获取 FirstSet
std::string Body::GetFirstSet()
{
    return m_firstSet;
}

// 向 FirstSet 中添加数据
void Body::SetFirstSet(char c)
{
    m_firstSet.push_back(c);
}

// 获取候选式首符
char Body::GetFirstCharacter()
{
    return m_expression[0];
}

// 候选式首符是否为终结符
bool Body::IsTerminal()
{
    return islower(m_expression[0]);
}

// 清空候选式
void Body::Clear()
{
    m_expression.clear();
}