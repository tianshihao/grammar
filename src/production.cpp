#include "production.h"

void Production::SetLeftPart(Expression e)
{
    leftPart = e;
}

void Production::SetRightPart(Expression e)
{
    rightPart.push_back(e);
}