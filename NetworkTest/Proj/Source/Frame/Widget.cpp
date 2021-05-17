#include "Widget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include "MainController.h"

const QString c_sLoading = QStringLiteral("正在获取数据，请稍等...");

MainForm::MainForm(QWidget *parent)
    : QWidget(parent),
      m_pBtnReceiveData(nullptr),
      m_pController(new MainController)
{
    initUI();
    initData();
}

MainForm::~MainForm()
{
}

void MainForm::initUI()
{
    setFixedSize(400, 300);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    this->setLayout(pMainLayout);

    QHBoxLayout *pTopLayout = new QHBoxLayout;
    m_pLineEdit = new QLineEdit;
    m_pLineEdit->setFixedSize(280, 28);
    m_pLineEdit->setPlaceholderText(QStringLiteral("请输入地址"));
    pTopLayout->addWidget(m_pLineEdit);
    pTopLayout->addStretch();
    m_pBtnReceiveData = new QPushButton;
    m_pBtnReceiveData->setFixedSize(84, 30);
    m_pBtnReceiveData->setText(QStringLiteral("接收数据"));
    pTopLayout->addWidget(m_pBtnReceiveData);
    pMainLayout->addLayout(pTopLayout);

    m_pResultEdit = new QTextEdit;
    m_pResultEdit->setReadOnly(true);
    pMainLayout->addWidget(m_pResultEdit);

    connect(m_pBtnReceiveData, &QPushButton::clicked, this, &MainForm::clickBtn);
}

void MainForm::initData()
{
    m_pLineEdit->setText(m_pController->loadLastAccessInfo());
}

void MainForm::clickBtn()
{
    m_pResultEdit->setText(c_sLoading);

    QString sUrl = m_pLineEdit->text();
    QString sServerData = m_pController->getServerData(sUrl);

    m_pResultEdit->setText(sServerData);
}

