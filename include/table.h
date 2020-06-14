#ifndef TABLE_H
#define TABLE_H

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

// 表格
class Table
{
private:
    // 表格行数
    int m_row;
    // 表格列数
    int m_column;
    // 表格大小
    int m_size;

    // 表格行头
    std::map<std::string, int> m_rowHeader;
    // 表格列头
    std::map<std::string, int> m_columnHeader;
    // std::vector<std::tuple<std::string, std::string, std::string>> cells;
    std::vector<std::string> m_cells;

public:
    /**
     *  @brief  初始化分析表
     *  @param  __VN  行头
     *  @param  __VT  列头
     */
    void InitTable(std::set<std::string> __VN, std::set<std::string> __VT);

    // std::string GetCellData();

    /**
     *  @brief  将表格的 rowHeader 行, columnHeader 列的单元格内容设置为 content.
     *  @param  rowHeader  单元格行标
     *  @param  columnHeader  单元格列标
     *  @param  content  将要设置的内容
     */
    void SetCellData(std::string row, std::string column, std::string content);

    /**
     *  @brief  打印表格
     */
    void PrintTable();
};

#endif // TABLE_H