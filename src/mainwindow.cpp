#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setMinimumSize(800, 600);

    m_centralWidget = new QWidget(this);
    m_gridLayout = new QGridLayout(m_centralWidget);

    // 文法输入区
    m_inputArea = new QTextEdit(this);
    m_inputArea->setText("这里输入文法");

    // 放置按钮的地方
    m_buttonArea = new QWidget(this);
    m_buttonArea->setFixedSize(200, 600);

    // 结果输出区
    m_outputArea = new QTextBrowser(this);
    m_outputArea->setText("这里输出结果");

    m_gridLayout->addWidget(m_inputArea, 0, 0);
    m_gridLayout->addWidget(m_buttonArea, 0, 1);
    m_gridLayout->addWidget(m_outputArea, 0, 2);

    this->setCentralWidget(m_centralWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::ParseText()
{
    // 获取输入文本
    QString inputText = m_inputArea->toPlainText();

    Grammer G(inputText.toStdString());

    return;
}