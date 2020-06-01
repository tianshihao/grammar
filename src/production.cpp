#include "production.h"

Body Production::GetLeftSide()
{
    return m_leftSide;
}

std::vector<Body> Production::GetRightSide()
{
    return m_rightSide;
}

void Production::SetLeftPart(Body e)
{
    m_leftSide = e;
}

void Production::SetRightPart(Body e)
{
    m_rightSide.push_back(e);
}