#ifndef BODY_H
#define BODY_H

#include <string>
#include <vector>

// 表达式
class Body
{
private:
    std::string m_string;
    std::string m_firstSet;

public:
    std::string GetString();
    std::string GetFirstSet();
    void SetFirstSet(char symbol);
    void PushBack(char);
    void Clear();
    bool IsTerminal();
};

#endif // BODY