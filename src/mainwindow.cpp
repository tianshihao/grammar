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

    // 按钮
    m_pushButton = new QPushButton(m_buttonArea);
    m_pushButton->setText((QString) "分析文法");

    // 按钮区的布局
    m_buttonLayout = new QVBoxLayout(m_buttonArea);
    m_buttonLayout->addWidget(m_pushButton);

    // m_buttonArea->setLayout(m_buttonLayout);

    // 结果输出区
    m_outputArea = new QTextBrowser(this);
    m_outputArea->setText("这里输出结果");

    m_gridLayout->addWidget(m_inputArea, 0, 0);
    m_gridLayout->addWidget(m_buttonArea, 0, 1);
    m_gridLayout->addWidget(m_outputArea, 0, 2);

    this->setCentralWidget(m_centralWidget);

    connect(m_pushButton, SIGNAL(clicked()), this, SLOT(ParsingText()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::ParsingText()
{
    // 获取输入文本
    Grammer G(m_inputArea->toPlainText().toStdString());

    DisplayResult();

    return;
}

void MainWindow::DisplayResult()
{
    QFile inFile("result.txt");

    if (!inFile.open(QFile::ReadOnly | QFile::Text))
        qDebug() << "Can not open";

    QByteArray byteArray = inFile.readAll();
    QString strData = QString::fromUtf8(byteArray.data());

    m_outputArea->setText(strData);

    inFile.close();

    return;
}