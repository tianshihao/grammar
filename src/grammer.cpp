#include "grammer.h"

// 构造函数
Grammer::Grammer()
{
    std::cout << "This is a grammer" << std::endl;
}

// 构造函数重载
Grammer::Grammer(std::string inputText)
{
    ParseText(inputText);
    MergeProduction();
    CalcFirstSet();
    CalcFollowSet();
    Print();
    CalcLLPrasingTable();
}

// 解析输入文本
void Grammer::ParseText(std::string inputText)
{
    // 向输入文本串末尾加换行符
    inputText = inputText + '\n';

    while (!inputText.empty())
    {
        // 已经分析的左部或右部长度
        // 用于清除已经分析的内容
        int bodyLength = 0;

        // 临时候选式
        Body tempBody;

        // 临时产生式
        Production tempProduction;

        // 扫描产生式左部
        for (int i = 0;; ++i)
        {
            // 检查箭头 "->"
            if (inputText[i] == '-')
            {
                // 防止越界
                if ((i + 1) <= (int)inputText.length() - 1)
                {
                    if (inputText[i + 1] == '>')
                    {
                        bodyLength = i + 1;
                        break;
                    }
                }
            }

            // 不是箭头
            tempBody.SetExpression(inputText[i]);
        }

        // 添加分析得到的非终结符至产生式左部
        tempProduction.SetLeftSide(tempBody);

        // 同时将该非终结符添加到非终结符号集
        m_VN.insert(tempBody.GetExpression());

        // 清空此非终结符容器
        tempBody.Clear();

        // 将已经分析的内容删除
        inputText.erase(0, bodyLength + 1);

        // 扫描产生式右部
        for (int i = 0; (inputText[i] != '\n') && (i < (int)inputText.length()); ++i, bodyLength = i)
        {
            // 找到一个候选式
            if (inputText[i] == '|')
            {
                // 添加分析得到的候选式至产生式右部
                tempProduction.SetRightSide(tempBody);

                // 清空此候选式容器
                tempBody.Clear();

                // 将已经分析的内容删除
                inputText.erase(0, i + 1);

                // 重置索引
                i = -1;

                continue;
            }

            // 如果找到终结符
            if (!isupper(inputText[i]) && inputText[i] != 39)
            {
                // 空串长度为两个字节
                // 特殊处理
                if (inputText[i] == -50)
                {
                    std::string VT = "ε";
                    m_VT.insert(VT);
                }
                else if (inputText[i] == -75)
                {
                }
                // 终结符
                else
                {
                    std::string VT;
                    VT.push_back(inputText[i]);

                    // 将终结符添加到终结符号集中
                    m_VT.insert(VT);
                }
            }

            tempBody.SetExpression(inputText[i]);
        }

        // 添加最后一个得到的候选式至产生式右部
        tempProduction.SetRightSide(tempBody);

        // 将已经分析的内容删除
        inputText.erase(0, bodyLength + 1);

        // 得到一个完整的产生式
        m_productionSet.push_back(tempProduction);
    }

    // 确定开始符号
    m_start = m_productionSet[0].GetLeftSide();

    // 将句子左界符添加到终结符号集
    m_VT.insert(std::string("#"));

    return;
}

bool Grammer::MergeProduction()
{
    // 标志位
    bool bMerged = false;

    for (int i = 0; i < (int)m_productionSet.size(); ++i)
    {
        for (int j = i + 1; j < (int)m_productionSet.size(); ++j)
        {
            // 第 j 个产生式的左部等于第 i 个产生式的左部, 合并
            if (m_productionSet[j].GetLeftSide().GetExpression() == m_productionSet[i].GetLeftSide().GetExpression())
            {
                bMerged = true;
                for (int k = 0; k < (int)m_productionSet[j].GetRightSide().size(); ++k)
                {
                    m_productionSet[i].SetRightSide(m_productionSet[j].GetRightSide()[k]);
                }

                // 合并之后删除产生式 m_prodectionSet[j]
                m_productionSet.erase(m_productionSet.begin() + j);
            }
        }
    }

    return bMerged;
}

// 计算 FIRST 集
void Grammer::CalcFirstSet()
{
    for (auto &production : m_productionSet)
    {
        // 求解 FOLLOW 集时要用到产生式左部非终结符的 FIRST 集
        {
            auto &body = production.GetLeftSide();
            DFS(body, body.GetFirstSymbol());
        }
        // 针对产生式右部的每一个候选式 body, 求 FIRST(body)
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

// DFS
void Grammer::DFS(Body &originBody, std::string firstSymbol)
{
    // 遍历所有产生式, 得到非终结符 firstSymbol 的 FIRST
    for (auto &production : m_productionSet)
    {
        if (production.GetLeftSide().GetFirstSymbol() == firstSymbol)
        {
            for (auto &nowBody : production.GetRightSide())
            {
                // 如果是非终结符, 添加到 FIRST 集
                if (nowBody.IsTerminal())
                {
                    originBody.SetFirstSet(nowBody.GetFirstSymbol());
                    nowBody.SetFirstSet(nowBody.GetFirstSymbol());
                }
                else
                {
                    DFS(originBody, nowBody.GetFirstSymbol());
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
    // 1. 对文法的开始符号 S, 令 #∈FOLLOW(S)
    // std::string tempString = "#";
    m_productionSet[0].GetLeftSide().SetFollowSet(std::string("#"));

    // 2. 若文法 G 中有形如 B->αAβ 的规则, 且 β!=ε, 则将 FIRST(β) 中的一切非 ε 符号
    // 加入 FOLLWOW(A)
    for (auto &production : m_productionSet)
    {
        // 因为 ε 占两个字节, 所以需要特殊处理
        // 候选式长度为 2 有以下可能:
        // 1) ε: 继续循环
        // 2) VN1+VN2: 将 FIRST(VN2) 中的元素加入到 FOLLOW(VN1) 中
        // 3) VN+VT: 将 Vt 加入到 FOLLOW(VN)
        // 4) VT+V: 继续循环

        auto leftBody = production.GetLeftSide();

        for (auto rightSide : production.GetRightSide())
        {
            std::string rightBody = rightSide.GetExpression();
            for (int i = 0; i < (int)rightBody.length(); ++i)
            {
                int VN1Length = MagicFunction(rightBody, i);

                if (VN1Length > 0)
                {

                    int VN2Length = MagicFunction(rightBody, i + VN1Length);

                    // * 2) VN1+VN2: 将 FIRST(VN2) 中的元素加入到 FOLLOW(VN1) 中
                    if (VN2Length > 0)
                    {
                        // 在产生式左部中找到非终结符 VN1 所属的 Body&
                        Body &VN1Body = FindLeftBody(std::string(rightBody, i, VN1Length), true);

                        // 在产生式左部中找到非终结符 VN2 所属的 Body
                        Body VN2Body = FindLeftBody(std::string(rightBody, i + VN1Length, VN2Length));

                        // 将 FIRST(VN2) 中的元素加入到 FOLLOW(VN1) 中
                        VN1Body.MergeFollowSet(VN2Body.GetFirstSet());
                    }
                    // * 3) VN+VT: 将 Vt 加入到 FOLLOW(VN)
                    // 可以怎么理解, VN1 后面是终结符
                    // 终结符的 "非终结符长度" 为 0
                    else if (VN2Length == 0)
                    {
                        // ε 绝不会出现在 FOLLOW 中
                        // 除了 ε 以外的终结符长度均为 1
                        // 在产生式左部中找到非终结符 VN1 所属的 Body&
                        Body &VN1Body = FindLeftBody(std::string(rightBody, i, VN1Length), true);

                        // 将 VT 加入到 FOLLOW(VN1) 中
                        std::string VT;
                        VT.push_back(rightBody[i + VN1Length]);
                        // VN1Body.MergeFollowSet(VT);
                        VN1Body.GetFollowSet(true).insert(VT);
                    }
                }
                // * 1) ε: 继续循环
                // * 4) VT+V: 继续循环
                else
                {
                    continue;
                }
            }
        }
    }

    // 3. 若文法 G 有形如 B->αA 或 B->αAβ 的规则, 且 ε∈FIRST(β),
    // 则 FOLLOW(B) 中的全部元素属于 FOLLOW(A)
    for (auto &production : m_productionSet)
    {
        // 1) ε: 继续循环
        // 2) VN1+VN2: 判断是否ε∈FIRST(VN2), 若是, 则将 FOLLOW(leftBody) 加入 FOLLOW(VN1)
        // 3) VN: 将 FOLLOW(leftBody) 加入 FOLLOW(VN)
        // 4) VN+VT: 继续循环
        // 5) VT+(*): 继续循环
        auto leftBody = production.GetLeftSide();

        for (auto rightSide : production.GetRightSide())
        {
            std::string rightBody = rightSide.GetExpression();
            for (int i = 0; i < (int)rightBody.length(); ++i)
            {
                int VN1Length = MagicFunction(rightBody, i);

                if (VN1Length > 0)
                {
                    int VN2Length = MagicFunction(rightBody, i + VN1Length);

                    if (VN2Length > 0)
                    {
                        // 在产生式左部找到非终结符 VN2 所属的 Body
                        Body VN2Body = FindLeftBody(std::string(rightBody, i + VN1Length, VN2Length));

                        std::string nullString = "ε";
                        std::set<std::string>::iterator it = VN2Body.GetFirstSet().find(nullString);

                        // * 2) VN1+VN2: 判断是否ε∈FIRST(VN2), 若是, 则将 FOLLOW(leftBody) 加入 FOLLOW(VN1)
                        // 如果 ε∈FIRST(VN2)
                        // ? 这样能判断出来吗?
                        if (it != VN2Body.GetFirstSet().end())
                        {
                            // 在产生式左部中找到非终结符 VN1 所属的 Body&
                            Body &VN1Body = FindLeftBody(std::string(rightBody, i, VN1Length), true);
                            // 将 FOLLOW(leftBody) 中的元素加入到 FOLLOW(VN1) 中
                            VN1Body.MergeFollowSet(leftBody.GetFollowSet());
                        }
                    }
                    // VN1 后可能是终结符, 也可能 VN1 没有后继, 是最后一个元素
                    else
                    {
                        // 判断 VN1 是不是最后一个元素
                        // * 3) VN: 将 FOLLOW(leftBody) 加入 FOLLOW(VN)
                        if ((i + VN1Length) >= (int)rightBody.length())
                        {
                            // 如果是, 则将 FOLLOW(leftBody) 加入到 FOLLOW(VN1)
                            // 在产生式左部中找到非终结符 VN1 所属的 Body&
                            Body &VN1Body = FindLeftBody(std::string(rightBody, i, VN1Length), true);
                            // 将 FOLLOW(leftBody) 中的元素加入到 FOLLOW(VN1) 中
                            VN1Body.MergeFollowSet(leftBody.GetFollowSet());
                        }
                        // * 4) VN+VT: 继续循环
                        else
                        {
                            continue;
                        }
                    }
                }
                // * 1) 空串ε: 继续循环
                // * 5) VT+(*): 继续循环
                else
                {
                    continue;
                }
            }
        }
    }

    // 之前的合并没有考虑空串 ε, 这里统一去掉 ε
    for (auto &production : m_productionSet)
    {
        std::string nullString = "ε";

        production.GetLeftSide().GetFollowSet(true).erase(nullString);
    }

    return;
}

int Grammer::MagicFunction(std::string str, int index)
{
    // A' 式非终结符, 返回 2
    // A 式非终结符, 返回 1
    // 终结符(包括 ε), 返回 0
    // 索引越界, 返回 -1
    if (index < (int)str.length())
    { // index 至少是非终结符
        if (isupper(str[index]))
        {
            if ((index + 1) < (int)str.length())
            {
                if (str[index + 1] == 39)
                {
                    return 2;
                }
            }

            return 1;
        }
        // index 是终结符
        else
        {
            return 0;
        }
    }

    // index 索引越界
    return -1;
}

Body Grammer::FindLeftBody(std::string body)
{
    for (auto production : m_productionSet)
    {
        // 产生式左部的表达式只有一个非终结符
        if (production.GetLeftSide().GetExpression() == body)
        {
            return production.GetLeftSide();
        }
    }

    return Body();
}

Body &Grammer::FindLeftBody(std::string body, bool bRefs)
{
    for (auto &production : m_productionSet)
    {
        // 产生式左部的表达式只有一个非终结符
        if (production.GetLeftSide().GetExpression() == body)
        {
            return production.GetLeftSide();
        }
    }

    // return Body();
}

void Grammer::CalcLLPrasingTable()
{
    // 分析表中没必要出现空串
    std::string nullString = "ε";
    m_VT.erase(nullString);

    // 初始化分析表
    m_pharsingTable.InitTable(m_VN, m_VT);

    // for 文法 G 的每个产生式 A->γ1|γ2|...|γm
    // {
    //     if a∈FIRST(γi) 置 M[A,a] = "A->γi";
    //     if ε∈FIRST(γi)
    //         for 任何 a∈FOLLOW(A)
    //         {
    //             置 M[A,a] = "A->γi"
    //         }
    // }

    for (auto production : m_productionSet)
    {
        Body leftBody = production.GetLeftSide();

        for (Body rightBody : production.GetRightSide())
        {
            // 行头
            std::string row = leftBody.GetExpression();

            // 规则 1
            // FIRST 集
            if (rightBody.GetExpression() != nullString)
            {
                for (auto column : rightBody.GetFirstSet())
                {
                    m_pharsingTable.SetCellData(row, column, rightBody.GetExpression());
                }
            }
            // 规则 2
            // FOLLOW 集
            else
            {
                for (auto column : leftBody.GetFollowSet())
                {
                    m_pharsingTable.SetCellData(row, column, rightBody.GetExpression());
                }
            }
        }
    }

    m_pharsingTable.PrintTable();

    return;
}

// 获取文法类型
int Grammer::GetType()
{
    return m_type;
}

// 设置文法类型
void Grammer::SetType(int type)
{
    m_type = type;
}

// 打印文法
void Grammer::Print()
{
    std::cout << "The Grammer" << std::endl;
    std::cout << "start symbol: " << m_start.GetExpression() << std::endl;
    std::cout << "Production" << std::endl;

    for (auto production : m_productionSet)
    {
        std::cout << "For "
                  << production.GetLeftSide().GetExpression()
                  << ": ";

        bool bPrintVN = true;

        // 是否输出终结符的 FIRST 集
        if (bPrintVN)
        {
            auto body = production.GetLeftSide();
            std::cout << "FIRST("
                      << body.GetExpression()
                      << ")={";
            for (auto symbol : body.GetFirstSet())
            {
                std::cout << " " << symbol << " ";
            }

            std::cout << "}\t";

            std::cout << "FOLLOW("
                      << body.GetExpression()
                      << ")={";
            for (auto symbol : body.GetFollowSet())
            {
                std::cout << " " << symbol << " ";
            }

            std::cout << "}\t";
        }

        for (auto body : production.GetRightSide())
        {
            std::cout << "FIRST("
                      << body.GetExpression()
                      << ")={";

            for (auto symbol : body.GetFirstSet())
            {
                std::cout << " " << symbol << " ";
            }

            std::cout << "}\t\t";
        }

        std::cout << std::endl;
    }

    return;
}