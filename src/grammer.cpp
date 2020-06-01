#include "grammer.h"

Grammer::Grammer()
{
    std::cout << "This is a grammer" << std::endl;
}

Grammer::Grammer(std::string inputText)
{
    ParseText(inputText);
    GetFirstSet();
}

void Grammer::SetType(int type)
{
    m_type = type;
}

int Grammer::GetType()
{
    return m_type;
}

void Grammer::GetFirstSet()
{
    for (auto production : m_productionSet)
    {
        // 针对产生式右部的每一个候选式 body, 求 FIRST(body)
        for (auto body : production.GetRightSide())
        {
            // 候选式的第一个符号
            char X = body.GetString()[0];

            // 如果 X 是终结符
            if (islower(X))
            {
                // FITRST(body) = {X}
                body.SetFirstSet(X);
                continue;
            }
            // 如果 X 是非终结符
            else
            {
                DFSTraverse(body, X);
            }
        }
    }

    return;
}

void Grammer::DFSTraverse(Body &originBody, char X)
{
    // 遍历所有产生式, 得到非终结符 X 的 FIRST
    for (auto production : m_productionSet)
    {
        if (production.GetLeftSide().GetString()[0] == X)
        {
            for (auto nowBody : production.GetRightSide())
            {
                // 如果是非终结符
                if (isupper(nowBody.GetString()[0]))
                {
                    // 递归
                    DFSTraverse(originBody, nowBody.GetString()[0]);
                }
                else
                {
                    originBody.SetFirstSet(nowBody.GetString()[0]);
                    nowBody.SetFirstSet(nowBody.GetString()[0]);
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

void Grammer::ParseText(std::string inputText)
{
    // 向输入文本串末尾加换行符
    inputText = inputText + "\n";

    while (!inputText.empty())
    {
        int index = 0;
        Production tempProduction;
        Body tempExpression;

        // 扫描产生式左部
        for (int i = 0;; ++i)
        {
            // 检查箭头 "->"
            if (inputText[i] == '-')
            {
                // 防止越界
                if ((i + 1) <= (int)inputText.length() - 1)
                {
                    if (inputText[i + i] == '>')
                    {
                        index = i + 1 + 1;
                        break;
                    }
                }
            }

            // 不是箭头
            tempExpression.PushBack(inputText[i]);
        }

        inputText.erase(0, index);
        tempProduction.SetLeftPart(tempExpression);
        tempExpression.Clear();

        // 扫描产生式右部
        for (int i = 0; (inputText[i] != '\n') && (i < (int)inputText.length()); ++i, index = i)
        {
            // 找到一个表达式
            if (inputText[i] == '|')
            {
                tempProduction.SetRightPart(tempExpression);
                tempExpression.Clear();
                inputText.erase(0, i + 1);
                i = -1;
                continue;
            }

            tempExpression.PushBack(inputText[i]);
        }
        // tempExpression.PushBack(inputText[0]);
        tempProduction.SetRightPart(tempExpression);
        // inputText.clear();
        inputText.erase(0, index + 1);

        // 找到一个产生式
        m_productionSet.push_back(tempProduction);
    }

    m_start = m_productionSet[0].GetLeftSide();

    Print();
}

void Grammer::Print()
{
    std::cout << "grammer" << std::endl;
    std::cout << "start symbol: " << m_start.GetString() << std::endl;
    std::cout << "production" << std::endl;

    for (int i = 0; i < (int)m_productionSet.size(); ++i)
    {
        std::cout << "production " << i + 1 << ": ";
        std::cout << m_productionSet[i].GetLeftSide().GetString() << " -> ";

        for (int j = 0; j < (int)m_productionSet[i].GetRightSide().size(); ++j)
        {
            std::cout << m_productionSet[i].GetRightSide()[j].GetString();
            if (j != (int)m_productionSet[i].GetRightSide().size() - 1)
            {
                std::cout << " | ";
            }
            else
            {
                std::cout << std::endl;
            }
        }
    }
}