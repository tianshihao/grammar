#include "body.h"

std::string Body::GetString()
{
    return m_string;
}

void Body::SetFirstSet(char symbol)
{
    m_firstSet.push_back(symbol);
}

void Body::PushBack(char c)
{
    m_string.push_back(c);
}

void Body::Clear()
{
    m_string.clear();
}