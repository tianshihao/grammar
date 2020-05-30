#ifndef PRODUCITON_H
#define PRODUCTION_H

#include "expression.h"
#include <vector>

// 产生式
class Production
{
private:
    Expression leftPart;
    std::vector<Expression> rightPart;

public:
    Expression GetLeftPart();
    Expression GetRightPart();

    void SetLeftPart(Expression e);
    void SetRightPart(Expression e);
};

#endif // PRODUCTION_H