#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grammar.h"
#include <QButtonGroup>
#include <QByteArray>
#include <QFile>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTextBlock>
#include <QTextBrowser>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QtCore>
#include <iostream>

/**
 * @brief 主窗口.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *m_centralWidget;    /// 程序的中心控件.
    QGridLayout *m_gridLayout;   /// 中心控件的布局.
    QTextEdit *m_inputArea;      /// 文法输入区.
    QWidget *m_buttonArea;       /// 放置按钮的控件.
    QVBoxLayout *m_buttonLayout; /// 按钮的布局.
    QTextBrowser *m_outputArea;  /// 结果输出区.
    QButtonGroup *m_buttonGroup; /// 按钮们所在的 Group.
    QPushButton *m_pushButton;   /// 分析按钮.

public:
    MainWindow(QWidget *parent = nullptr); /// 构造函数.
    ~MainWindow();                         /// 析构函数.
    /**
     * @brief 将文件中的结果显示在 m_outputArea.
     */
    void DisplayResult();

public slots:
    /**
    * @brief 分析文本.
    */
    void ParsingText();
};

#endif // MAINWINDOW_H
