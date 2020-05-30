#include "expression.h"

void Expression::PushBack(char c)
{
    e.push_back(c);
}

void Expression::Clear()
{
    e.clear();
}