#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <memory>

class QPushButton;
class MainController;

class MainForm : public QWidget
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

    void initUI();
    void initData();

private slots:
    void clickBtn();

private:
    QPushButton *m_pBtnReceiveData;
    QLineEdit *m_pLineEdit;
    QTextEdit *m_pResultEdit;

    std::unique_ptr<MainController> m_pController;
};
#endif // WIDGET_H
