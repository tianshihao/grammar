#ifndef PRODUCITON_H
#define PRODUCTION_H

#include "body.h"
#include <vector>

// 产生式
class Production
{
private:
    // 产生式左部.
    Body m_bodyLeft;
    // 产生式右部.
    std::vector<Body> m_vecRightSide;

public:
    /**
     *  @return  返回产生式左部的引用.
    */
    Body &GetLeftSide();

    /**
     *  @brief  设置产生式左部数据.
     *  @param  将产生式左部设置为 __bodyLeft.
     */
    void SetLeftSide(Body __bodyLeft);

    /**
     *  @return  返回产生式右部的候选式向量的引用.
    */
    std::vector<Body> &GetRightSide();

    /**
     *  @brief  向产生式右部添加候选式.
     *  @param  将产生式右部设置为 __bodyRight.
     */
    void SetRightSide(Body __bodyRight);
};

#endif // PRODUCTION_H