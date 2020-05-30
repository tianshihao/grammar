#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grammer.h"
#include <QButtonGroup>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTextBlock>
#include <QTextBrowser>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QtCore>
#include <iostream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout;
    QTextEdit *m_inputArea;
    QWidget *m_buttonArea;
    QVBoxLayout *m_buttonLayout;
    QTextBrowser *m_outputArea;
    QButtonGroup *m_buttonGroup;
    QPushButton *m_pushButton1;
    QPushButton *m_pushButton2;
    QPushButton *m_pushButton3;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void PassText();
};
#endif // MAINWINDOW_H
