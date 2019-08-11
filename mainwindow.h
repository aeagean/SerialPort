/**********************************************************
Author: Qt君
微信公众号: Qt君(首发)
QQ群: 732271126
LICENSE: MIT
**********************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void serialUpdateData(double value);

private slots:
    void on_clearButton_clicked();
    void on_sendButton_clicked();
    void on_openButton_clicked();

    void onReadData();
    void onUpdateChart();
    void onCreateTestData();
    void onTestClicked(bool status);

private:
    Ui::MainWindow *ui;
    QSerialPort m_serialPort;

    /* 定义两个可变数组存放绘图的坐标数据 */
    QVector<double> m_xs;
    QVector<double> m_ys;

    int m_xLength;
    double m_serialUpdateData;

    QTimer m_testTimer;
    QTimer m_updateChartTimer;
};

#endif // MAINWINDOW_H
