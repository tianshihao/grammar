#include "body.h"

std::string Body::GetExpression()
{
    return m_strExpression;
}

void Body::SetExpression(char __ch)
{
    m_strExpression.push_back(__ch);
}

std::set<std::string> Body::GetFirstSet()
{
    return m_setFirstSet;
}

void Body::SetFirstSet(std::string __symbol)
{
    m_setFirstSet.insert(__symbol);
}

std::set<std::string> Body::GetFollowSet()
{
    return m_setFollowSet;
}

std::set<std::string> &Body::GetFollowSet(bool __bRefs)
{
    return m_setFollowSet;
}

void Body::SetFollowSet(const std::string &__symbol)
{
    m_setFollowSet.insert(__symbol);
}

void Body::MergeFollowSet(std::set<std::string> __set)
{
    for (auto str : __set)
    {
        m_setFollowSet.insert(str);
    }
}

std::string Body::GetFirstSymbol()
{
    // return m_strExpression[0];
    if (m_strExpression.length() >= 2)
    {
        // 如果首符是 A' 式非终结符
        if (m_strExpression[1] == 39)
        {
            return std::string(m_strExpression, 0, 2);
        }
        // 如果首符是 ε, 长度为两个字节
        else if ((m_strExpression[0] == -50) && (m_strExpression[1] == -75))
        {
            return std::string(m_strExpression, 0, 2);
        }
        // 首符可以用 ASCII 表示, 长度为一个字节 else
        {
            return std::string(m_strExpression, 0, 1);
        }
    }
    else
    {
        return std::string(m_strExpression, 0, 1);
    }

    return std::string();
}

bool Body::IsTerminal()
{
    // return islower(m_strExpression[0]);
    if (isupper(m_strExpression[0]))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Body::Clear()
{
    m_strExpression.clear();
    m_setFirstSet.clear();
    m_setFollowSet.clear();
}