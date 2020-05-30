#include "production.h"

Expression Production::GetLeftPart()
{
    return leftPart;
}

std::vector<Expression> Production::GetRightPart()
{
    return rightPart;
}

void Production::SetLeftPart(Expression e)
{
    leftPart = e;
}

void Production::SetRightPart(Expression e)
{
    rightPart.push_back(e);
}