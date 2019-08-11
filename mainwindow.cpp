/**********************************************************
Author: Qt君
微信公众号: Qt君(首发)
QQ群: 732271126
LICENSE: MIT
**********************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_xLength(0)
{
    ui->setupUi(this);

    /* 向绘图区域QCustomPlot(从widget提升来的)添加一条曲线 */
    ui->qCustomPlot->addGraph();

    m_updateChartTimer.start(1000);

    connect(&m_updateChartTimer, SIGNAL(timeout()), this, SLOT(onUpdateChart()));
    connect(&m_testTimer, SIGNAL(timeout()), this, SLOT(onCreateTestData()));
    connect(ui->m_testBtn, SIGNAL(clicked(bool)), this, SLOT(onTestClicked(bool)));

    /* 连接数据信号槽 */
    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(onReadData()));

    /* 设置坐标轴标签名称 */
    ui->qCustomPlot->xAxis->setLabel("时间");
    ui->qCustomPlot->yAxis->setLabel("");

    /* 设置坐标轴显示范围,否则我们只能看到默认的范围 */
    ui->qCustomPlot->xAxis->setRange(0, 10);
    ui->qCustomPlot->yAxis->setRange(0, 2000);

    /* 查找可用的串口 */
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->PortBox->addItem(info.portName());
    }

    /* 设置波特率下拉菜单默认显示第三项(9600) */
    ui->BaudBox->setCurrentIndex(3);

    /* 关闭发送按钮的使能 */
    ui->sendButton->setEnabled(false);

    qDebug()<<"界面初始化成功！";
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 清空接收窗口 */
void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

/* 发送数据 */
void MainWindow::on_sendButton_clicked()
{
    m_serialPort.write(ui->textEdit_2->toPlainText().toLatin1());
}

/* 读取接收到的数据 */
void MainWindow::onReadData()
{
    QByteArray buf;
    buf = m_serialPort.readAll();
    m_serialUpdateData = buf.toDouble();

    if(! buf.isEmpty()) {
        QString str = ui->textEdit->toPlainText();
        str += tr(buf);
        ui->textEdit->clear();
        ui->textEdit->append(str);

    }

    buf.clear();
}

void MainWindow::onUpdateChart()
{
    if (m_xLength > 10) {
        m_xLength = 0;
        m_xs.clear();
        m_ys.clear();
    }

    m_xs.append(m_xLength);
    m_ys.append(m_serialUpdateData);
    m_serialUpdateData = 0;

    ui->qCustomPlot->replot();
    ui->qCustomPlot->graph(0)->setData(m_xs, m_ys);

    m_xLength++;
}

void MainWindow::onCreateTestData()
{
    m_serialUpdateData = rand() % 1000 + 1000;
}

void MainWindow::onTestClicked(bool status)
{
    if (m_testTimer.isActive()) {
        m_testTimer.stop();
        ui->m_testBtn->setText("开启");
    }
    else {
        m_testTimer.start();
        ui->m_testBtn->setText("关闭");
        m_serialUpdateData = 0;
    }
}

void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==tr("打开串口")) {
        if (m_serialPort.isOpen()) {
            m_serialPort.close();
            m_serialPort.clear();
        }

        /* 设置串口名 */
        m_serialPort.setPortName(ui->PortBox->currentText());

        /* 打开串口 */
        m_serialPort.open(QIODevice::ReadWrite);

        /* 设置波特率 */
        m_serialPort.setBaudRate(ui->BaudBox->currentText().toInt());

        /* 设置数据位数 */
        if (ui->BitNumBox->currentIndex() == 8) {
            m_serialPort.setDataBits(QSerialPort::Data8);
        }

        /* 设置奇偶校验 */
        if (ui->ParityBox->currentIndex() == 0) {
            m_serialPort.setParity(QSerialPort::NoParity);
        }

        /* 设置停止位 */
        switch(ui->StopBox->currentIndex()) {
            case 1: m_serialPort.setStopBits(QSerialPort::OneStop); break;
            case 2: m_serialPort.setStopBits(QSerialPort::TwoStop); break;
            default: break;
        }

        /* 设置流控制 */
        m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

        /* 关闭设置菜单使能 */
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitNumBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->openButton->setText(tr("关闭串口"));
        ui->sendButton->setEnabled(true);

    }
    else {
        /* 关闭串口 */
        m_serialPort.close();
        m_serialPort.clear();

        /* 恢复设置使能 */
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
        ui->sendButton->setEnabled(false);
    }
}
