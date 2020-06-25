#ifndef BODY_H
#define BODY_H

#include <iostream>
#include <set>
#include <string>
#include <vector>

/**
 * @brief 候选式.
 */
class Body
{
private:
    std::string m_strExpression;          /// 候选式表达式.
    std::set<std::string> m_setFirstSet;  /// 候选式 FIRST 集.
    std::set<std::string> m_setFollowSet; /// 非终结符 FOLLOW 集.

public:
    /**
     * @return 返回候选式字符串.
     */
    std::string GetExpression();

    /**
     * @brief 设置候选式.
     * @param __ch  向表达式中添加的字符.
     */
    void SetExpression(char __ch);

    /**
     * @return 返回候选式的 First 集.
     */
    std::set<std::string> GetFirstSet();

    /**
     * @brief 向候选式的 First 集中添加终结符.
     * @param 向 FIRSET 集中添加 __strSymbol.
     */
    void SetFirstSet(std::string __strSymbol);

    /**
     * @return 返回非终结符的 FOLLOW 集.
     */
    std::set<std::string> GetFollowSet();

    /**
     * @brief 获得 FOLLOW 集.
     * @param __bRefs  用于区分.
     * @return 返回非终结符 FOLLOW 集的引用.
     */
    std::set<std::string> &GetFollowSet(bool __bRefs);

    /**
     * @brief 向非终结符的 FOLLOW 集中添加终结符.
     * @param 向 FOLLOW 集中添加 __strSymbol.
     */
    void SetFollowSet(const std::string &__strSymbol);

    /**
     * @brief 将另一个产生式的 FIRST 集或 FOLLOW 集合并到 FOLLOW 集.
     * @param 将 __set 中的元素合并到 FOLLOW 集.
     */
    void MergeFollowSet(std::set<std::string> __set);

    /**
     * @return 返回候选式首符.
     */
    std::string GetFirstSymbol();

    /**
     * @brief 候选式首符是否为终结符.
     */
    bool IsTerminal();

    /**
     * @brief 清空候选式.
     */
    void Clear();
};

#endif // BODY_H