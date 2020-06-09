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
}

// 解析输入文本
void Grammer::ParseText(std::string inputText)
{
    // 向输入文本串末尾加换行符
    inputText = inputText + "\n";

    while (!inputText.empty())
    {
        int index = 0;
        Production tempProduction;
        Body tempBody;

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
                        index = i + 1 + 1;
                        break;
                    }
                }
            }

            // 不是箭头
            tempBody.SetExpression(inputText[i]);
        }

        inputText.erase(0, index);
        tempProduction.SetLeftSide(tempBody);
        tempBody.Clear();

        // 扫描产生式右部
        for (int i = 0; (inputText[i] != '\n') && (i < (int)inputText.length()); ++i, index = i)
        {
            // 找到一个表达式
            if (inputText[i] == '|')
            {
                tempProduction.SetRightSide(tempBody);
                tempBody.Clear();
                inputText.erase(0, i + 1);
                i = -1;
                continue;
            }

            tempBody.SetExpression(inputText[i]);
        }
        tempProduction.SetRightSide(tempBody);
        inputText.erase(0, index + 1);

        // 找到一个产生式
        m_productionSet.push_back(tempProduction);
    }

    m_start = m_productionSet[0].GetLeftSide();
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
        // 1. 空串 ε: 什么也不做
        // 2. VN1+VN2+: 将 FIRST(VN2) 中的元素加入到 FOLLOW(VN1) 中
        // 3. VN+VT+: 继续循环
        // 3. VT+V+ : 继续循环

        auto leftBody = production.GetLeftSide();

        for (auto rightSide : production.GetRightSide())
        {
            std::string rightBody = rightSide.GetExpression();
            for (int i = 0; i < (int)rightBody.length(); ++i)
            {
                // 候选式子中的非终结符 VN1, 找到了!
                // todo 增加对 A' 式终结符的支持
                if (isupper(rightBody[i]))
                {
                    // 该非终结符存在后继 V
                    if ((i + 1) < (int)rightBody.length())
                    {
                        // 并且 V 也是非终结符, 即 VN2
                        if (isupper(rightBody[i + 1]))
                        {
                            // 在产生式左部中找到该非终结符 VN2 所属的 Body
                            Body VN2Body = FindLeftBody(std::string(rightBody, i + 1, i + 2));
                            // 在产生式左部中找到非终结符 VN1 所属的 Body&
                            Body &VN1Body = FindLeftBody(std::string(rightBody, i, i + 1), true);
                            // 将 FIRST(VN2) 中的元素加入到 FOLLOW(VN1) 中
                            VN1Body.MergeFollowSet(VN2Body.GetFirstSet());
                        }
                    }
                }
                // 如果是终结符, 则将其本身加入到非终结符的 FOLLOW 集中
                else
                {
                    // std::set<std::string> tempFirstSet;
                    // tempFirstSet.insert(std::string(rightBody, i, i + 1));
                    // leftBody.MergeFollowSet(tempFirstSet);
                }
            }
        }
    }

    // 3. 若文法 G 有形如 B->αA 或 B->αAβ 的规则, 且 ε∈FIRST(β),
    // 则 FOLLOW(B) 中的全部元素属于 FOLLOW(A)
    for (auto &production : m_productionSet)
    {
        // 1. 空串ε: 什么也不做
        // 2. VN1+VN2: 判断是否ε∈FIRST(VN2), 若是, 则将 FOLLOW(leftBody) 加入 FOLLOW(VN1)
        // 3. VN: 将 FOLLOW(leftBody) 加入 FOLLOW(VN)
        // 4. VN+VT: 什么也不做
        // 5. VT+V: 什么也不做
        auto leftBody = production.GetLeftSide();

        for (auto rightSide : production.GetRightSide())
        {
            std::string rightBody = rightSide.GetExpression();
            for (int i = 0; i < (int)rightBody.length(); ++i)
            {
                // 候选式子中的非终结符 VN1, 找到了!
                if (isupper(rightBody[i]))
                {
                    // 该非终结符存在后继 V
                    if ((i + 1) < (int)rightBody.length())
                    {
                        // 并且 V 也是非终结符, 即VN2
                        if (isupper(rightBody[i + 1]))
                        {
                            // 在产生式左部中找到该非终结符 VN2 所属的 Body
                            Body VN2Body = FindLeftBody(std::string(rightBody, i + 1, i + 2));
                            // 如果 ε∈FIRST(VN2), 则将 FOLLOW(leftBody) 加入 FOLLOW(VN1)
                            if (1)
                            {
                                // 在产生式左部中找到非终结符 VN1 所属的 Body&
                                Body &VN1Body = FindLeftBody(std::string(rightBody, i, i + 1), true);
                                VN1Body.MergeFollowSet(leftBody.GetFollowSet());
                            }
                        }
                    }
                    // VN1 没有后继了! 将 FOLLOW 中的元素加入 FOLLOW(VN1)
                    else
                    {
                        Body &VN1Body = FindLeftBody(std::string(rightBody, i, i + 1), true);
                        VN1Body.MergeFollowSet(leftBody.GetFollowSet());
                    }
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
                  << ": \t";

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