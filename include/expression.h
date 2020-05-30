#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

// 表达式
class Expression
{
private:
    std::string e;

public:
    std::string GetExpression();
    void PushBack(char);
    void Clear();
};

#endif // EXPRESSION