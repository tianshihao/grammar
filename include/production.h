#ifndef PRODUCITON_H
#define PRODUCTION_H

#include "body.h"
#include <vector>

// 产生式
class Production
{
private:
    Body m_leftSide;
    std::vector<Body> m_rightSide;

public:
    Body GetLeftSide();
    std::vector<Body> GetRightSide();

    void SetLeftPart(Body e);
    void SetRightPart(Body e);
};

#endif // PRODUCTION_H