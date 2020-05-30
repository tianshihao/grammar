#include "expression.h"

std::string Expression::GetExpression()
{
    return e;
}

void Expression::PushBack(char c)
{
    e.push_back(c);
}

void Expression::Clear()
{
    e.clear();
}