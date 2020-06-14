#include "production.h"

Body &Production::GetLeftSide()
{
    return m_bodyLeft;
}

void Production::SetLeftSide(Body __bodyLeft)
{
    m_bodyLeft = __bodyLeft;
}

std::vector<Body> &Production::GetRightSide()
{
    return m_vecRightSide;
}

void Production::SetRightSide(Body __bodyRight)
{
    m_vecRightSide.push_back(__bodyRight);
}
