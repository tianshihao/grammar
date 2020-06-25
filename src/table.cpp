#include "table.h"

void Table::InitTable(std::set<std::string> __VN, std::set<std::string> __VT)
{
    // 将 __VN 添加到映射中,
    // 以非终结符为键, 索引为值.
    int nValue = 0;
    for (auto symbol : __VN)
    {
        m_mapRowHeader.insert(std::pair<std::string, int>(symbol, nValue++));
    }

    // 确定表格行数.
    m_nRow = nValue;

    nValue = 0;
    for (auto symbol : __VT)
    {
        m_mapColumnHeader.insert(std::pair<std::string, int>(symbol, nValue++));
    }

    m_nColumn = nValue;

    m_nSize = m_nRow * m_nColumn;

    for (int i = 0; i < m_nSize; ++i)
    {
        m_vecCells.push_back(std::string("error"));
    }

    return;
}

void Table::SetCellData(std::string __leftBody, std::string __column, std::string __rightBody)
{
    m_vecCells[m_mapRowHeader[__leftBody] * m_nColumn + m_mapColumnHeader[__column]] = __leftBody + "->" + __rightBody;
}

void Table::Print()
{
    std::ofstream outFile;
    outFile.open("result.txt", std::ios::openmode::_S_app);

    int nCellWidth = 16;

    outFile << std::fixed << std::left;

    // 打印列头
    outFile << std::setw(nCellWidth) << std::setfill(' ') << " ";

    auto itColumnHeader = m_mapColumnHeader.begin();

    for (int i = 0; i < m_nColumn; ++i, itColumnHeader++)
    {
        outFile << std::setw(nCellWidth) << itColumnHeader->first;
    }

    outFile << "\n";

    // 按行打印表格内容.
    auto itRowHeader = m_mapRowHeader.begin();

    for (int i = 0; i < m_nRow; ++i, itRowHeader++)
    {
        outFile << std::setw(nCellWidth) << itRowHeader->first;

        for (int j = 0; j < m_nColumn; ++j)
        {
            outFile << std::setw(nCellWidth) << m_vecCells[i * m_nColumn + j];

            // 希腊字母 ε 存储长度为2, 显示长度为 1.
            if (m_vecCells[i * m_nColumn + j][m_vecCells[i * m_nColumn + j].length() - 1] < 0)
            {
                outFile << " ";
            }
        }

        outFile << "\n";
    }

    outFile.close();

    return;
}