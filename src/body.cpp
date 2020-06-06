#include "body.h"

// Body::Body()
// {
// }

// // 复制构造函数
// Body::Body(const Body &body)
// {
//     std::cout << "copy constructor called\n";
//     m_expression = body.m_expression;
//     m_firstSet = body.m_firstSet;
// }

// // 移动构造函数
// Body::Body(Body &&body) : m_expression(std::move(body.m_expression)), m_firstSet(std::move(body.m_firstSet)), data(std::exchange(body.data, 0))
// {
//     std::cout << "move constructor called\n";

//     body.Clear();
// }

// // 运算符重载, 左值
// void Body::operator=(const Body &body)
// {
//     m_expression = body.m_expression;
//     m_firstSet = body.m_firstSet;
// }

// // 运算符重载, 右值
// void Body::operator=(Body &&body)
// {

//     body.Clear();
// }

// 获得候选式
std::string Body::GetExpression()
{
    return std::move(m_expression);
}

// 向候选式中添加数据
void Body::SetExpression(char c)
{
    m_expression.push_back(c);
}

// 获取 FirstSet
std::string Body::GetFirstSet()
{
    return m_firstSet;
}

// 向 FirstSet 中添加数据
void Body::SetFirstSet(char c)
{
    m_firstSet.push_back(c);
}

// 获取候选式首符
char Body::GetFirstCharacter()
{
    return m_expression[0];
}

// 候选式首符是否为终结符
bool Body::IsTerminal()
{
    return islower(m_expression[0]);
}

// 清空候选式
void Body::Clear()
{
    m_expression.clear();
    m_firstSet.clear();
}