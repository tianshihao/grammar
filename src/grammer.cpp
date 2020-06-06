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
    CalcFirstSet();
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
                    originBody.SetFirstSet(nowBody.GetFirstSymbol().c_str()[0]);
                    nowBody.SetFirstSet(nowBody.GetFirstSymbol().c_str()[0]);
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

        for (auto body : production.GetRightSide())
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