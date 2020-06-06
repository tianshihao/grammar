#include "grammer.h"
#include <utility>

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
                    if (inputText[i + i] == '>')
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
        // 值传递, 调用复制构造函数
        tempProduction.SetLeftSide(tempBody);
        tempBody.Clear();

        // 扫描产生式右部
        for (int i = 0; (inputText[i] != '\n') && (i < (int)inputText.length()); ++i, index = i)
        {
            // 找到一个表达式
            if (inputText[i] == '|')
            {
                // 值传递, 调用复制构造函数
                tempProduction.SetRightSide(tempBody);
                tempBody.Clear();
                inputText.erase(0, i + 1);
                i = -1;
                continue;
            }

            tempBody.SetExpression(inputText[i]);
        }
        // 值传递, 调用复制构造函数
        tempProduction.SetRightSide(tempBody);
        inputText.erase(0, index + 1);

        // 找到一个产生式
        m_productionSet.push_back(tempProduction);
    }

    m_start = m_productionSet[0].GetLeftSide();
}

// 计算 FIRST 集
void Grammer::CalcFirstSet()
{
    for (auto &production : m_productionSet)
    {
        // 针对产生式右部的每一个候选式 body, 求 FIRST(body)
        for (auto &body : production.GetRightSide())
        {
            if (body.IsTerminal())
            {
                body.SetFirstSet(body.GetFirstCharacter());
                continue;
            }
            else
            {
                DFS(body, body.GetFirstCharacter());
            }
        }
    }

    return;
}

// DFS
void Grammer::DFS(Body &originBody, char X)
{
    // 遍历所有产生式, 得到非终结符 X 的 FIRST
    for (auto &production : m_productionSet)
    {
        if (production.GetLeftSide().GetFirstCharacter() == X)
        {
            for (auto &nowBody : std::move(production.GetRightSide()))
            {
                // 如果是非终结符
                if (isupper(nowBody.GetFirstCharacter()))
                {
                    // 递归
                    DFS(originBody, nowBody.GetFirstCharacter());
                }
                else
                {
                    originBody.SetFirstSet(nowBody.GetFirstCharacter());
                    nowBody.SetFirstSet(nowBody.GetFirstCharacter());
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
                  << ": ";

        // 值传递, 调用复制构造函数
        for (auto body : production.GetRightSide())
        {
            std::cout << "FIRST("
                      << body.GetExpression()
                      << ")={";

            // 循环打印候选式 body 的 FIRST 集
            int length = (int)body.GetFirstSet().length();
            for (int i = 0; i < length; ++i)
            {
                std::cout << body.GetFirstSet()[i];

                if (i == length - 1)
                {
                    break;
                }

                std::cout << ", ";
            }

            std::cout << "}\t";
        }

        std::cout << std::endl;
    }

    return;
}