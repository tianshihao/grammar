#include "grammer.h"

Grammer::Grammer()
{
}

Grammer::Grammer(std::string __strInputText)
{
    ParseText(__strInputText);
    MergeProduction();
    CalcFirstSet();
    CalcFollowSet();
    PrintGrammer();
    CalcLL1PrasingTable();
}

void Grammer::ParseText(std::string __strInputText)
{
    // 向输入文本串末尾加换行符.
    __strInputText = __strInputText + '\n';

    while (!__strInputText.empty())
    {
        // 已经分析的左部或右部长度.
        // 用于清除已经分析的内容.
        int nBodyLength = 0;

        // 临时候选式.
        Body bodyTemp;

        // 临时产生式.
        Production productionTemp;

        // 扫描产生式左部.
        for (int i = 0;; ++i)
        {
            // 如果文法后面有多余的换行符.
            if (__strInputText[i] == '\n')
            {
                __strInputText.clear();
                return;
            }

            // 检查箭头 "->".
            if (__strInputText[i] == '-')
            {
                // 防止越界.
                if ((i + 1) <= (int)__strInputText.length() - 1)
                {
                    if (__strInputText[i + 1] == '>')
                    {
                        nBodyLength = i + 1;
                        break;
                    }
                }
            }

            // 不是箭头.
            bodyTemp.SetExpression(__strInputText[i]);
        }

        // 添加分析得到的非终结符至产生式左部.
        productionTemp.SetLeftSide(bodyTemp);

        // 同时将该非终结符添加到非终结符号集.
        m_setVN.insert(bodyTemp.GetExpression());

        // 清空此非终结符容器.
        bodyTemp.Clear();

        // 将已经分析的内容删除.
        __strInputText.erase(0, nBodyLength + 1);

        // 扫描产生式右部.
        for (int i = 0; (__strInputText[i] != '\n') && (i < (int)__strInputText.length()); ++i, nBodyLength = i)
        {
            // 找到一个候选式.
            if (__strInputText[i] == '|')
            {
                // 添加分析得到的候选式至产生式右部.
                productionTemp.SetRightSide(bodyTemp);

                // 清空此候选式容器.
                bodyTemp.Clear();

                // 将已经分析的内容删除.
                __strInputText.erase(0, i + 1);

                // 重置索引.
                i = -1;

                continue;
            }

            // 如果找到终结符.
            if (!isupper(__strInputText[i]) && __strInputText[i] != 39)
            {
                // 空串长度为两个字节,
                // 特殊处理.
                if (__strInputText[i] == -50)
                {
                    std::string strVT = "ε";
                    m_setVT.insert(strVT);
                }
                else if (__strInputText[i] == -75)
                {
                }
                // 终结符.
                else
                {
                    std::string strVT;
                    strVT.push_back(__strInputText[i]);

                    // 将终结符添加到终结符号集中.
                    m_setVT.insert(strVT);
                }
            }

            bodyTemp.SetExpression(__strInputText[i]);
        }

        // 添加最后一个得到的候选式至产生式右部.
        productionTemp.SetRightSide(bodyTemp);

        // 将已经分析的内容删除.
        __strInputText.erase(0, nBodyLength + 1);

        // 得到一个完整的产生式.
        m_vecProductionSet.push_back(productionTemp);
    }

    // 确定开始符号.
    m_bodyStart = m_vecProductionSet[0].GetLeftSide();

    // 将句子左界符添加到终结符号集.
    m_setVT.insert(std::string("#"));

    return;
}

bool Grammer::MergeProduction()
{
    // 标志位.
    bool bMerged = false;

    for (int i = 0; i < (int)m_vecProductionSet.size(); ++i)
    {
        for (int j = i + 1; j < (int)m_vecProductionSet.size(); ++j)
        {
            // 第 j 个产生式的左部等于第 i 个产生式的左部, 合并.
            if (m_vecProductionSet[j].GetLeftSide().GetExpression() == m_vecProductionSet[i].GetLeftSide().GetExpression())
            {
                bMerged = true;
                for (int k = 0; k < (int)m_vecProductionSet[j].GetRightSide().size(); ++k)
                {
                    m_vecProductionSet[i].SetRightSide(m_vecProductionSet[j].GetRightSide()[k]);
                }

                // 合并之后删除产生式 m_prodectionSet[j].
                m_vecProductionSet.erase(m_vecProductionSet.begin() + j);
            }
        }
    }

    return bMerged;
}

void Grammer::CalcFirstSet()
{
    for (auto &production : m_vecProductionSet)
    {
        // 求解 FOLLOW 集时要用到产生式左部非终结符的 FIRST 集.
        {
            auto &body = production.GetLeftSide();
            DFS(body, body.GetFirstSymbol());
        }
        // 针对产生式右部的每一个候选式 body, 求 FIRST(body).
        for (auto &body : production.GetRightSide())
        {
            if (body.IsTerminal())
            {
                body.SetFirstSet(body.GetFirstSymbol());
                continue;
            }
            else
            {
                DFS(body, body.GetFirstSymbol());
            }
        }
    }

    return;
}

void Grammer::DFS(Body &__bodyOrigin, std::string __strFirstSymbol)
{
    // 遍历所有产生式, 得到非终结符 __strFirstSymbol 的 FIRST.
    for (auto &production : m_vecProductionSet)
    {
        if (production.GetLeftSide().GetFirstSymbol() == __strFirstSymbol)
        {
            for (auto &bodyCurrent : production.GetRightSide())
            {
                // 如果是非终结符, 添加到 FIRST 集.
                if (bodyCurrent.IsTerminal())
                {
                    __bodyOrigin.SetFirstSet(bodyCurrent.GetFirstSymbol());
                    bodyCurrent.SetFirstSet(bodyCurrent.GetFirstSymbol());
                }
                else
                {
                    DFS(__bodyOrigin, bodyCurrent.GetFirstSymbol());
                }
            }
        }
        else
        {
            continue;
        }
    }

    return;
}

void Grammer::CalcFollowSet()
{
    // 1. 对文法的开始符号 S, 令 #∈FOLLOW(S).
    m_vecProductionSet[0].GetLeftSide().SetFollowSet(std::string("#"));

    // 2. 若文法 G 中有形如 B->αAβ 的规则, 且 β!=ε,
    // 则将 FIRST(β) 中的一切非 ε 符号加入 FOLLWOW(A).
    for (auto &production : m_vecProductionSet)
    {
        // 因为 ε 占两个字节, 所以需要特殊处理.
        // 候选式长度为 2 有以下可能:
        // 1) ε: 继续循环;
        // 2) VN1+VN2: 将 FIRST(VN2) 中的元素加入到 FOLLOW(VN1) 中;
        // 3) VN+VT: 将 Vt 加入到 FOLLOW(VN);
        // 4) VT+V: 继续循环.

        auto bodyLeft = production.GetLeftSide();

        for (auto vecRightSide : production.GetRightSide())
        {
            std::string strRightBody = vecRightSide.GetExpression();
            for (int i = 0; i < (int)strRightBody.length(); ++i)
            {
                int nVN1Length = MagicFunction(strRightBody, i);

                if (nVN1Length > 0)
                {

                    int nVN2Length = MagicFunction(strRightBody, i + nVN1Length);

                    // * 2) VN1+VN2: 将 FIRST(VN2) 中的元素加入到 FOLLOW(VN1) 中.
                    if (nVN2Length > 0)
                    {
                        // 在产生式左部中找到非终结符 VN1 所属的 Body&.
                        Body &bodyVN1 = FindLeftBody(std::string(strRightBody, i, nVN1Length), true);

                        // 在产生式左部中找到非终结符 VN2 所属的 Body.
                        Body bodyVN2 = FindLeftBody(std::string(strRightBody, i + nVN1Length, nVN2Length));

                        // 将 FIRST(VN2) 中的元素加入到 FOLLOW(VN1) 中.
                        bodyVN1.MergeFollowSet(bodyVN2.GetFirstSet());
                    }
                    // * 3) VN+VT: 将 Vt 加入到 FOLLOW(VN).
                    // 可以怎么理解, VN1 后面是终结符,
                    // 终结符的 "非终结符长度" 为 0.
                    else if (nVN2Length == 0)
                    {
                        // ε 绝不会出现在 FOLLOW 中,
                        // 除了 ε 以外的终结符长度均为 1.
                        // 在产生式左部中找到非终结符 VN1 所属的 Body&.
                        Body &bodyVN1 = FindLeftBody(std::string(strRightBody, i, nVN1Length), true);

                        // 将 VT 加入到 FOLLOW(VN1) 中.
                        std::string strVT;
                        strVT.push_back(strRightBody[i + nVN1Length]);
                        bodyVN1.GetFollowSet(true).insert(strVT);
                    }
                }
                // * 1) ε: 继续循环;
                // * 4) VT+V: 继续循环.
                else
                {
                    continue;
                }
            }
        }
    }

    // 3. 若文法 G 有形如 B->αA 或 B->αAβ 的规则, 且 ε∈FIRST(β),
    // 则 FOLLOW(B) 中的全部元素属于 FOLLOW(A).
    for (auto &production : m_vecProductionSet)
    {
        // 1) ε: 继续循环;
        // 2) VN1+VN2: 判断是否ε∈FIRST(VN2), 若是, 则将 FOLLOW(bodyLeft) 加入 FOLLOW(VN1);
        // 3) VN: 将 FOLLOW(bodyLeft) 加入 FOLLOW(VN);
        // 4) VN+VT: 继续循环;
        // 5) VT+(*): 继续循环.
        auto bodyLeft = production.GetLeftSide();

        for (auto vecRightSide : production.GetRightSide())
        {
            std::string strRightBody = vecRightSide.GetExpression();
            for (int i = 0; i < (int)strRightBody.length(); ++i)
            {
                int nVN1Length = MagicFunction(strRightBody, i);

                if (nVN1Length > 0)
                {
                    int nVN2Length = MagicFunction(strRightBody, i + nVN1Length);

                    if (nVN2Length > 0)
                    {
                        // 在产生式左部找到非终结符 VN2 所属的 Body.
                        Body bodyVN2 = FindLeftBody(std::string(strRightBody, i + nVN1Length, nVN2Length));

                        std::string strNullString = "ε";
                        std::set<std::string>::iterator it = bodyVN2.GetFirstSet().find(strNullString);

                        // * 2) VN1+VN2: 判断是否ε∈FIRST(VN2), 若是, 则将 FOLLOW(bodyLeft) 加入 FOLLOW(VN1).
                        // 如果 ε∈FIRST(VN2).
                        // ? 这样能判断出来吗?
                        if (it != bodyVN2.GetFirstSet().end())
                        {
                            // 在产生式左部中找到非终结符 VN1 所属的 Body&.
                            Body &bodyVN1 = FindLeftBody(std::string(strRightBody, i, nVN1Length), true);
                            // 将 FOLLOW(bodyLeft) 中的元素加入到 FOLLOW(VN1) 中.
                            bodyVN1.MergeFollowSet(bodyLeft.GetFollowSet());
                        }
                    }
                    // VN1 后可能是终结符, 也可能 VN1 没有后继, 是最后一个元素.
                    else
                    {
                        // 判断 VN1 是不是最后一个元素.
                        // * 3) VN: 将 FOLLOW(bodyLeft) 加入 FOLLOW(VN).
                        if ((i + nVN1Length) >= (int)strRightBody.length())
                        {
                            // 如果是, 则将 FOLLOW(bodyLeft) 加入到 FOLLOW(VN1).
                            // 在产生式左部中找到非终结符 VN1 所属的 Body&.
                            Body &bodyVN1 = FindLeftBody(std::string(strRightBody, i, nVN1Length), true);
                            // 将 FOLLOW(bodyLeft) 中的元素加入到 FOLLOW(VN1) 中.
                            bodyVN1.MergeFollowSet(bodyLeft.GetFollowSet());
                        }
                        // * 4) VN+VT: 继续循环.
                        else
                        {
                            continue;
                        }
                    }
                }
                // * 1) 空串ε: 继续循环;
                // * 5) VT+(*): 继续循环.
                else
                {
                    continue;
                }
            }
        }
    }

    // 之前的合并没有考虑空串 ε, 这里统一去掉 ε.
    for (auto &production : m_vecProductionSet)
    {
        std::string strNullString = "ε";

        production.GetLeftSide().GetFollowSet(true).erase(strNullString);
    }

    return;
}

int Grammer::MagicFunction(std::string __str, int __nIndex)
{
    // A' 式非终结符, 返回 2;
    // A 式非终结符, 返回 1;
    // 终结符(包括 ε), 返回 0;
    // 索引越界, 返回 -1.
    if (__nIndex < (int)__str.length())
    {
        // index 至少是非终结符.
        if (isupper(__str[__nIndex]))
        {
            if ((__nIndex + 1) < (int)__str.length())
            {
                if (__str[__nIndex + 1] == 39)
                {
                    return 2;
                }
            }

            return 1;
        }
        // index 是终结符.
        else
        {
            return 0;
        }
    }

    // index 索引越界.
    return -1;
}

Body Grammer::FindLeftBody(std::string __strBody)
{
    for (auto production : m_vecProductionSet)
    {
        // 产生式左部的表达式只有一个非终结符.
        if (production.GetLeftSide().GetExpression() == __strBody)
        {
            return production.GetLeftSide();
        }
    }

    return Body();
}

Body &Grammer::FindLeftBody(std::string __strBody, bool __bRefs)
{
    for (auto &production : m_vecProductionSet)
    {
        // 产生式左部的表达式只有一个非终结符.
        if (production.GetLeftSide().GetExpression() == __strBody)
        {
            return production.GetLeftSide();
        }
    }
}

void Grammer::CalcLL1PrasingTable()
{
    // 分析表中没必要出现空串.
    std::string strNullString = "ε";
    m_setVT.erase(strNullString);

    // 初始化分析表.
    m_tablePharsingTable.InitTable(m_setVN, m_setVT);

    // for 文法 G 的每个产生式 A->γ1|γ2|...|γm.
    for (auto production : m_vecProductionSet)
    {
        Body bodyLeft = production.GetLeftSide();

        // for 产生式 P 的每个候选式 γi.
        for (Body bodyRight : production.GetRightSide())
        {
            // 行坐标.
            std::string strRow = bodyLeft.GetExpression();

            // 规则 1.
            // if a∈FIRST(γi) 置 M[A,a] = "A->γi";
            if (bodyRight.GetExpression() != strNullString)
            {
                // 列坐标.
                for (auto strColumn : bodyRight.GetFirstSet())
                {
                    m_tablePharsingTable.SetCellData(strRow, strColumn, bodyRight.GetExpression());
                }
            }
            // 规则 2.
            // if ε∈FIRST(γi)
            //         for 任何 a∈FOLLOW(A)
            //         {
            //             置 M[A,a] = "A->γi"
            //         }
            else
            {
                // 列坐标.
                for (auto strColumn : bodyLeft.GetFollowSet())
                {
                    m_tablePharsingTable.SetCellData(strRow, strColumn, bodyRight.GetExpression());
                }
            }
        }
    }

    m_tablePharsingTable.Print();

    return;
}

int Grammer::GetType()
{
    return m_nType;
}

void Grammer::SetType(int type)
{
    m_nType = type;
}

void Grammer::PrintGrammer()
{
    std::ofstream outFile;
    outFile.open("result.txt");

    outFile << "The Grammer" << std::endl;
    outFile << "start symbol: " << m_bodyStart.GetExpression() << std::endl;
    outFile << "Production" << std::endl;

    for (auto production : m_vecProductionSet)
    {
        outFile << "For "
                << production.GetLeftSide().GetExpression()
                << ": ";

        bool bPrintVN = true;

        // 是否输出终结符的 FIRST 集.
        if (bPrintVN)
        {
            auto body = production.GetLeftSide();
            outFile << "FIRST("
                    << body.GetExpression()
                    << ")={";
            for (auto symbol : body.GetFirstSet())
            {
                outFile << " " << symbol << " ";
            }

            outFile << "}\t";

            outFile << "FOLLOW("
                    << body.GetExpression()
                    << ")={";
            for (auto symbol : body.GetFollowSet())
            {
                outFile << " " << symbol << " ";
            }

            outFile << "}\t";
        }

        for (auto body : production.GetRightSide())
        {
            outFile << "FIRST("
                    << body.GetExpression()
                    << ")={";

            for (auto symbol : body.GetFirstSet())
            {
                outFile << " " << symbol << " ";
            }

            outFile << "}\t\t";
        }

        outFile << std::endl;
    }

    outFile.close();

    return;
}