#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grammer.h"
#include <QGridLayout>
#include <QMainWindow>
#include <QTextBlock>
#include <QTextBrowser>
#include <QTextEdit>
#include <iostream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout;
    QTextEdit *m_inputArea;
    QWidget *m_buttonArea;
    QTextBrowser *m_outputArea;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ParseText();
};
#endif // MAINWINDOW_H
