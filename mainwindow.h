#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <thread>
#include <cmath>
#include <QStringListModel>
#include <QtNetwork/QTcpSocket>                 //通信套接字//对方的(客户端的)套接字(通信套接字)
#include "MainWindowLayout.h"

using std::vector;
//设置socket信息
const static int port = 8090;
const QString ServerIP = "192.168.4.1";
const int TIME_CYC = 500;
const int THREAD_NUM = 4;
//const int ARRAY = 8;
const int BUFFERSIZE = 2*60*60*24;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    MainLayout* layout;

    QMessageBox msgBox;//询问对话窗
    QTcpSocket* tcpClient;
    QTimer *timer;
    QLineSeries* series [ARRAY];
    QStringList text_content[ARRAY+2];//记录文本的变量
    QStringListModel *model_text[ARRAY+2];
    std::thread* array_graph[THREAD_NUM];

    vector<vector<float>> array_signal;//二维数组保存采集数据
    vector<float> Temperature_signal;//温度数组
    vector<float> RH_signal;//湿度数组
    int mesbox_result;
    QByteArray read_buffer;
    bool start_flag;
    bool read_flag;

    void setCheckBoxIcon();
private slots:

    void setCheckBox1Icno();
    void setCheckBox2Icno();
    void setCheckBox3Icno();
    void setCheckBox4Icno();
    void setCheckBox5Icno();
    void setCheckBox6Icno();
    void setCheckBox7Icno();
    void setCheckBox8Icno();

    void startCollection();
    void storeMessage();
    void drawGraph();
    void drawThread(int start, int end);
    void stopCollection();
    void clearData();

};
#endif // MAINWINDOW_H
