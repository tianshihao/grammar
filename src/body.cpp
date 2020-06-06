#include "body.h"

// 获得候选式
std::string Body::GetExpression()
{
    return m_expression;
}

// 向候选式中添加数据
void Body::SetExpression(char ch)
{
    m_expression.push_back(ch);
}

// 获取 FirstSet
std::set<std::string> Body::GetFirstSet()
{
    return m_firstSet;
}

// 向 FirstSet 中添加数据
void Body::SetFirstSet(std::string symbol)
{
    m_firstSet.insert(symbol);
}

// 获取候选式首符
std::string Body::GetFirstSymbol()
{
    // return m_expression[0];
    if (m_expression.length() >= 2)
    {
        if (m_expression[1] == 39)
        {
            return std::string(m_expression, 0, 2);
        }
        else
        {
            return std::string(m_expression, 0, 1);
        }
    }
    else
    {
        return std::string(m_expression, 0, 1);
    }

    return std::string();
}

// 候选式首符是否为终结符
bool Body::IsTerminal()
{
    // return islower(m_expression[0]);
    if (isupper(m_expression[0]))
    {
        std::cout << m_expression[0] << " is not terminal\n";
        return false;
    }
    else
    {
        std::cout << m_expression[0] << " is terminal\n";
        return true;
    }
}

// 清空候选式
void Body::Clear()
{
    m_expression.clear();
    m_firstSet.clear();
    m_followSet.clear();
}