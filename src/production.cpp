#include "production.h"

// 获取产生式左部数据
Body Production::GetLeftSide()
{
    return m_leftSide;
}

// 设置产生式左部数据
void Production::SetLeftSide(Body leftSide)
{
    m_leftSide = leftSide;
}

// 获取产生式右部的候选式向量
std::vector<Body> Production::GetRightSide()
{
    return m_rightSide;
}

// 向产生式右部添加候选式
void Production::SetRightSide(Body rightSide)
{
    m_rightSide.push_back(rightSide);
}
