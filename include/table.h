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
    int m_nRow;
    // 表格列数
    int m_nColumn;
    // 表格大小
    int m_nSize;

    // 表格行头
    std::map<std::string, int> m_mapRowHeader;
    // 表格列头
    std::map<std::string, int> m_mapColumnHeader;
    // 按行存储的单元格
    std::vector<std::string> m_vecCells;

public:
    /**
     *  @brief  初始化分析表.
     *  @param  __VN  非终结符作为行头.
     *  @param  __VT  终结符作为列头.
     */
    void InitTable(std::set<std::string> __VN, std::set<std::string> __VT);

    /**
     *  @brief  将表格的 rowHeader 行, columnHeader 列的单元格内容设置为 content.
     *  @param  __rowHeader  单元格行坐标.
     *  @param  __columnHeader  单元格列坐标.
     *  @param  __content  将要设置的内容.
     */
    void SetCellData(std::string __row, std::string __column, std::string __content);

    /**
     *  打印表格.
     */
    void PrintTable();
};

#endif // TABLE_H