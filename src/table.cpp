#include "table.h"

void Table::InitTable(std::set<std::string> __VN, std::set<std::string> __VT)
{
    int value = 0;
    for (auto symbol : __VN)
    {
        m_rowHeader.insert(std::pair<std::string, int>(symbol, value++));
    }

    m_row = value;

    value = 0;
    for (auto symbol : __VT)
    {
        m_columnHeader.insert(std::pair<std::string, int>(symbol, value++));
    }

    m_column = value;

    m_size = m_row * m_column;

    for (int i = 0; i < m_size; ++i)
    {
        m_cells.push_back(std::string("error"));
    }

    return;
}

void Table::SetCellData(std::string row, std::string column, std::string content)
{
    m_cells[m_rowHeader[row] * m_column + m_columnHeader[column]] = row + "->" + content;
}

void Table::PrintTable()
{
    int printRow = m_row + 2;
    int printColumn = m_column + 1;
    int cellWidth = 12;

    // std::cout << std::fixed << std::left;

    std::cout << std::setw(cellWidth) << std::setfill(' ') << " ";
    auto itColumnHeader = m_columnHeader.begin();

    // 打印列头
    for (int i = 0; i < m_column; ++i, itColumnHeader++)
    {
        std::cout << "|";
        std::cout << std::setw(cellWidth) << itColumnHeader->first;
    }

    std::cout << "\n";
    std::cout << std::setfill(' ');

    auto itRowHeader = m_rowHeader.begin();

    // 按行打印表格内容
    for (int i = 0; i < m_row; ++i, itRowHeader++)
    {
        std::cout << std::setw(cellWidth) << itRowHeader->first;

        for (int j = 0; j < m_column; ++j)
        {
            std::cout << std::setw(cellWidth) << m_cells[i * m_column + j];
        }

        std::cout << "\n";
    }

    std::cout << "\n";

    return;
}