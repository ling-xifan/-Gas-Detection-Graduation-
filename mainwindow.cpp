#include "mainwindow.h"

static QColor Graph_color[ARRAY] = {
    QColor(255, 0, 0),//大红
    QColor(0, 255, 0),//黑色
    QColor(0, 0, 255),//Cyan
    QColor(255, 0, 255),//MediumSeaGreen
    QColor(255, 125, 0),//goldenrod
    QColor(0, 255, 255),//SandyBrown
    QColor(0, 0, 0),//DarkSeaGreen3
    QColor(125, 125, 0),//Gold2
    };

/**
 * @brief 在绘制线程中绘制折线图
 *
 * @param start 起始索引
 * @param end 结束索引
 */

void MainWindow::drawThread(int start, int end)
{
    while(read_flag);//等待读结束
    for(int num = start; num <= end; num++) {

        series[num]->setPen(QPen(Graph_color[num], 1, Qt::SolidLine));  // 设置折线显示效果

        for (int i = 0; i <= 50; i++)
            series[num]->append(50-i, array_signal[num][i]/1000);
    }

}

/**
 * @brief 构造函数
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建布局对象并设置给主窗口
    resize(1280, 720);
    layout = new MainLayout(this);
    setCentralWidget(layout);

    start_flag = 0;
    read_flag = 0;
    array_signal.resize(ARRAY, vector<float>(BUFFERSIZE));
    RH_signal.resize(BUFFERSIZE);
    Temperature_signal.resize(BUFFERSIZE);
    tcpClient = new QTcpSocket(this);   //实例化tcpClient;

    msgBox.setWindowTitle(u8"提示");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    for (int i = 0; i < ARRAY; i++) {
        series[i] = new QLineSeries;
    }

    connect(layout->res1_button, &QPushButton::clicked,this,&MainWindow::setCheckBox1Icno);
    connect(layout->res2_button, &QPushButton::clicked,this,&MainWindow::setCheckBox2Icno);
    connect(layout->res3_button, &QPushButton::clicked,this,&MainWindow::setCheckBox3Icno);
    connect(layout->res4_button, &QPushButton::clicked,this,&MainWindow::setCheckBox4Icno);
    connect(layout->res5_button, &QPushButton::clicked,this,&MainWindow::setCheckBox5Icno);
    connect(layout->res6_button, &QPushButton::clicked,this,&MainWindow::setCheckBox6Icno);
    connect(layout->res7_button, &QPushButton::clicked,this,&MainWindow::setCheckBox7Icno);
    connect(layout->res8_button, &QPushButton::clicked,this,&MainWindow::setCheckBox8Icno);
    connect(layout->beginbutton, &QPushButton::clicked,this,&MainWindow::startCollection);

}

MainWindow::~MainWindow()
{
}
/**
 * @brief 空线程函数
 */
void idle_thread() {
    //do nothing
}

/**
 * @brief 开始采集按键槽函数
 */
void MainWindow::startCollection()
{
    msgBox.setText(u8"确认开始采集？");
    mesbox_result = msgBox.exec();

    switch (mesbox_result) {
        case  QMessageBox::Yes:
            //qDebug() << "yes" << endl;//debug
        if(start_flag) {
            //do nothing
        }else {
            tcpClient->abort();                 //取消原有连接
            tcpClient->connectToHost(ServerIP, port);
            if (tcpClient->waitForConnected(1000)) {
               qDebug() << "tcp connect" << endl;//debug
               connect(tcpClient, &QTcpSocket::readyRead, this, &MainWindow::storeMessage);
               timer = new QTimer;
               timer->start(TIME_CYC);//TIME_CYC采集一次
               for (int j = 0; j < THREAD_NUM; j++) {
                   array_graph[j] = new std::thread(idle_thread);//创建空闲线程
                   array_graph[j]->join();
               }
               connect(timer, &QTimer::timeout, this, &MainWindow::drawGraph);
               start_flag = 1;
            }else {
                msgBox.setText(u8"服务器连接失败");
                msgBox.exec();
            }
        }

            break;
        case  QMessageBox::No:
            //qDebug() << "No" << endl;//debug
            break;
        default:
            break;
    }
}

/**
 * @brief TCPIP读取数据
 */
void MainWindow::storeMessage()
{
    uint buf_0 ;
    uint buf_1 ;
    uint buf_2 ;
    uint buf_3 ;
    uint buf ;
    read_flag = 1;
    read_buffer = tcpClient->readAll();
    qDebug() << "read begin" << endl;//debug
    for(int i = 0;i<ARRAY;i++) {
        buf_0 = ((uint)(read_buffer.at(4 * i + 0)) << 0 ) & 0x000000ff;
        buf_1 = ((uint)(read_buffer.at(4 * i + 1)) << 8 ) & 0x0000ff00;
        buf_2 = ((uint)(read_buffer.at(4 * i + 2)) << 16) & 0x00ff0000;
        buf_3 = ((uint)(read_buffer.at(4 * i + 3)) << 24) & 0xff000000;
        buf = buf_0 + buf_1 + buf_2 + buf_3;

        array_signal.at(i).insert(array_signal.at(i).begin(),
            *(float*)&buf);
        qDebug() << (*(float*)&buf)/1000 << endl;//debug
    }
        qDebug() << "read end" << endl;//debug
    buf_0 = ((uint)read_buffer.at(4 * 8))&0x000000ff;
    buf_1 = ((uint)(read_buffer.at(4 * 8 + 1)) << 8) & 0x0000ff00;
    buf_2 = ((uint)(read_buffer.at(4 * 8 + 2)) << 16) & 0x00ff0000;
    buf_3 = ((uint)(read_buffer.at(4 * 8 + 3)) << 24) & 0xff000000;
    buf = buf_0 + buf_1 + buf_2 + buf_3;
    Temperature_signal.insert(Temperature_signal.begin(),(*(float*)&buf)/100.0);

    buf_0 = ((uint)read_buffer.at(4 * 9))&0x000000ff;
    buf_1 = ((uint)(read_buffer.at(4 * 9 + 1)) << 8) & 0x0000ff00;
    buf_2 = ((uint)(read_buffer.at(4 * 9 + 2)) << 16) & 0x00ff0000;
    buf_3 = ((uint)(read_buffer.at(4 * 9 + 3)) << 24) & 0xff000000;
    buf = buf_0 + buf_1 + buf_2 + buf_3;
    RH_signal.insert(RH_signal.begin(),(*(float*)&buf)/100.0);

    qDebug() << "read end" << endl;//debug
    read_buffer.clear();
    read_flag = 0;
}
/**
 * @brief 画图设置
 */
void MainWindow::drawGraph()
{
    for (int j = 0; j < THREAD_NUM; j++) {
         delete array_graph[j];//取消原线程
    }
    for (int j = 0; j < ARRAY; j++) {
        delete series[j];
        series[j] = new QLineSeries;
    }
    for (int j = 0; j < THREAD_NUM; j++) {
        array_graph[j] = new std::thread(&MainWindow::drawThread, this, j*ARRAY/THREAD_NUM, (j+1)*ARRAY / THREAD_NUM - 1);//创建画图线程
        array_graph[j]->join();
    }
    for (int j = 0; j < ARRAY; j++) {
        layout->draw_curve->addSeries(series[j]);
        layout->draw_curve->setAxisX(layout->axisX, series[j]);
        layout->draw_curve->setAxisY(layout->axisY, series[j]);
    }
}
void MainWindow::setCheckBox1Icno()
{
    // 设置点击响应
    layout->res1_ischecked = !layout->res1_ischecked;
    if (layout->res1_ischecked) {
        layout->res1_button->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
    } else {
        layout->res1_button->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
    }

}

void MainWindow::setCheckBox2Icno()
{
    // 设置点击响应
    layout->res2_ischecked = !layout->res2_ischecked;
    if (layout->res2_ischecked) {
        layout->res2_button->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
    } else {
        layout->res2_button->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
    }

}

void MainWindow::setCheckBox3Icno()
{
    // 设置点击响应
    layout->res3_ischecked = !layout->res3_ischecked;
    if (layout->res3_ischecked) {
        layout->res3_button->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
    } else {
        layout->res3_button->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
    }

}

void MainWindow::setCheckBox4Icno()
{
    // 设置点击响应
    layout->res4_ischecked = !layout->res4_ischecked;
    if (layout->res4_ischecked) {
        layout->res4_button->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
    } else {
        layout->res4_button->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
    }

}

void MainWindow::setCheckBox5Icno()
{
    // 设置点击响应
    layout->res5_ischecked = !layout->res5_ischecked;
    if (layout->res5_ischecked) {
        layout->res5_button->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
    } else {
        layout->res5_button->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
    }
}

void MainWindow::setCheckBox6Icno()
{
    // 设置点击响应
    layout->res6_ischecked = !layout->res6_ischecked;
    if (layout->res6_ischecked) {
        layout->res6_button->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
    } else {
        layout->res6_button->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
    }

}

void MainWindow::setCheckBox7Icno()
{
    // 设置点击响应
    layout->res7_ischecked = !layout->res7_ischecked;
    if (layout->res7_ischecked) {
        layout->res7_button->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
    } else {
        layout->res7_button->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
    }

}

void MainWindow::setCheckBox8Icno()
{
    // 设置点击响应
    layout->res8_ischecked = !layout->res8_ischecked;
    if (layout->res8_ischecked) {
        layout->res8_button->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
    } else {
        layout->res8_button->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
    }

}

