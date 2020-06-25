#include "production.h"

Body &Production::GetLeftSide()
{
    return m_bodyLeftSide;
}

void Production::SetLeftSide(Body __bodyLeft)
{
    m_bodyLeftSide = __bodyLeft;
}

std::vector<Body> &Production::GetRightSide()
{
    return m_vecRightSide;
}

void Production::SetRightSide(Body __bodyRight)
{
    m_vecRightSide.push_back(__bodyRight);
}
