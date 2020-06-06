#ifndef PRODUCITON_H
#define PRODUCTION_H

#include "body.h"
#include <vector>

// 产生式
class Production
{
private:
    // 产生式左部
    Body m_leftSide;
    // 产生式右部
    std::vector<Body> m_rightSide;

public:
    // 获取产生式左部数据
    Body GetLeftSide();
    // 设置产生式左部数据
    void SetLeftSide(Body leftSide);
    // 获取产生式右部的候选式向量
    std::vector<Body> &GetRightSide();
    // 向产生式右部添加候选式
    void SetRightSide(Body rightSide);
};

#endif // PRODUCTION_H