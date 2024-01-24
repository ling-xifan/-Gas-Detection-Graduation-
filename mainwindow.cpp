#include "mainwindow.h"


/**
 * @brief 在绘制线程中绘制折线图
 *
 * @param start 起始索引
 * @param end 结束索引
 */

void MainWindow::drawThread(int start, int end)
{
    while(read_flag);//等待读结束
    qDebug() << "draw begin" << endl;//debug
    for(int num = start; num <= end; num++) {
        if(layout->res_ischecked[num]) {
            series[num]->setPen(QPen(Graph_color[num], 1, Qt::SolidLine));  // 设置折线显示效果
        } else {
            series[num]->setPen(QColor(0,0,0,0));//通道隐藏
        }

        for (int i = 0; i <= 50; i++) {
            series[num]->append(50-i, array_signal[num][i]/1000);
        }

        text_content[num].clear();
        text_content[num].append(QString::number(round(array_signal[num][0]/1000))); // 将数字转换为字符串并更新文本内容
        model_text[num]->setStringList(text_content[num]); // 更新对应的 QStringListModel
        layout->res_list[num]->setModel(model_text[num]); // 更新 QListView 的显示
        layout->res_list[num]->update();
    }
    if(end > ARRAY/2) {
        text_content[ARRAY].clear();
        text_content[ARRAY].append(QString::number((Temperature_signal[0]),'f',2)); // 将数字转换为字符串并更新文本内容
        model_text[ARRAY]->setStringList(text_content[ARRAY]); // 更新对应的 QStringListModel
        layout->res_list[ARRAY]->setModel(model_text[ARRAY]); // 更新 QListView 的显示
        layout->res_list[ARRAY]->update();

        text_content[ARRAY+1].clear();
        text_content[ARRAY+1].append(QString::number((RH_signal[0]),'f',2)); // 将数字转换为字符串并更新文本内容
        model_text[ARRAY+1]->setStringList(text_content[ARRAY+1]); // 更新对应的 QStringListModel
        layout->res_list[ARRAY+1]->setModel(model_text[ARRAY+1]); // 更新 QListView 的显示
        layout->res_list[ARRAY+1]->update();
    }
     qDebug() << "draw end" << endl;//debug
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

    tcpClient = new QTcpSocket(this);   //实例化tcpClient;
    timer = new QTimer;
    array_signal.resize(ARRAY, vector<float>(BUFFERSIZE));
    RH_signal.resize(BUFFERSIZE);
    Temperature_signal.resize(BUFFERSIZE);
    msgBox.setWindowTitle(u8"提示");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    for (int i = 0; i < ARRAY; i++) {
        series[i] = new QLineSeries;
    }
    for (int i = 0; i < ARRAY + 2; i++) {
        model_text[i] = new QStringListModel(text_content[i]);
    }
    connect(layout->res_button[0], &QPushButton::clicked,this,&MainWindow::setCheckBox1Icno);
    connect(layout->res_button[1], &QPushButton::clicked,this,&MainWindow::setCheckBox2Icno);
    connect(layout->res_button[2], &QPushButton::clicked,this,&MainWindow::setCheckBox3Icno);
    connect(layout->res_button[3], &QPushButton::clicked,this,&MainWindow::setCheckBox4Icno);
    connect(layout->res_button[4], &QPushButton::clicked,this,&MainWindow::setCheckBox5Icno);
    connect(layout->res_button[5], &QPushButton::clicked,this,&MainWindow::setCheckBox6Icno);
    connect(layout->res_button[6], &QPushButton::clicked,this,&MainWindow::setCheckBox7Icno);
    connect(layout->res_button[7], &QPushButton::clicked,this,&MainWindow::setCheckBox8Icno);
    connect(layout->beginbutton, &QPushButton::clicked,this,&MainWindow::startCollection);
    connect(layout->stopbutton, &QPushButton::clicked,this,&MainWindow::stopCollection);
    connect(layout->clearbutton, &QPushButton::clicked,this,&MainWindow::clearData);

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
 * @brief 按下Button停止采集
 */
void MainWindow::stopCollection()
{
    msgBox.setText(u8"确认停止采集？");
    mesbox_result = msgBox.exec();

    switch (mesbox_result) {
        case  QMessageBox::Yes:
            //qDebug() << "yes" << endl;//debug
        if(!start_flag) {
            //do nothing
        }else {
            start_flag = 0;
            disconnect(tcpClient, &QTcpSocket::readyRead, this, &MainWindow::storeMessage);
            tcpClient->abort();                 //取消原有连接
            disconnect(timer, &QTimer::timeout, this, &MainWindow::drawGraph);
            timer->stop();
            for (int j = 0; j < THREAD_NUM; j++)
            {
                delete array_graph[j];//删除线程
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
 * @brief 清空缓存区数据
 */
void MainWindow::clearData()
{
    if(start_flag) {
        //do nothing
    }else {
        for (int i = 0; i < ARRAY; i++)
        {
            text_content[i].clear();
            model_text[i]->setStringList(text_content[i]); // 更新对应的 QStringListModel
            layout->res_list[i]->setModel(model_text[i]); // 更新 QListView 的显示
            layout->res_list[i]->update();
            series[i]->clear();
        }

        array_signal.clear();
        RH_signal.clear();
        Temperature_signal.clear();
        array_signal.resize(ARRAY, vector<float>(BUFFERSIZE));
        RH_signal.resize(BUFFERSIZE);
        Temperature_signal.resize(BUFFERSIZE);
    }
}
/**
 * @brief TCPIP读取数据
 */
void MainWindow::storeMessage()
{
    uint buf_0,buf_1,buf_2,buf_3,buf ;
    read_flag = 1;
    read_buffer = tcpClient->readAll();
    qDebug() << "read begin" << endl;//debug
    for(int i = 0;i<ARRAY;i++) {
        buf_0 = ((uint)(read_buffer.at(4 * i + 0)) << 0 ) & 0x000000ff;
        buf_1 = ((uint)(read_buffer.at(4 * i + 1)) << 8 ) & 0x0000ff00;
        buf_2 = ((uint)(read_buffer.at(4 * i + 2)) << 16) & 0x00ff0000;
        buf_3 = ((uint)(read_buffer.at(4 * i + 3)) << 24) & 0xff000000;
        buf = buf_0 + buf_1 + buf_2 + buf_3;

        if((*(float*)&buf < 0)) {
            array_signal.at(i).insert(array_signal.at(i).begin(),0);
        } else {
            array_signal.at(i).insert(array_signal.at(i).begin(),
                (*(float*)&buf));
        }

    }
     //   qDebug() << "read end" << endl;//debug
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

void MainWindow::setCheckBoxIcon()
{
    for(int i = 0;i<ARRAY;i++)
    {
        if (layout->res_ischecked[i]) {
            layout->res_button[i]->setIcon(QIcon(layout->checkpixmap->scaled(QSize(35, 35))));
        } else {
            layout->res_button[i]->setIcon(QIcon(layout->uncheckpixmap->scaled(QSize(35, 35))));
        }
    }
}
void MainWindow::setCheckBox1Icno()
{
    // 设置点击响应
    layout->res_ischecked[0] = !layout->res_ischecked[0];
    this->setCheckBoxIcon();
}

void MainWindow::setCheckBox2Icno()
{
    // 设置点击响应
    layout->res_ischecked[1] = !layout->res_ischecked[1];
    this->setCheckBoxIcon();
}

void MainWindow::setCheckBox3Icno()
{
    // 设置点击响应
    layout->res_ischecked[2] = !layout->res_ischecked[2];
    this->setCheckBoxIcon();
}

void MainWindow::setCheckBox4Icno()
{
    // 设置点击响应
    layout->res_ischecked[3] = !layout->res_ischecked[3];
    this->setCheckBoxIcon();

}

void MainWindow::setCheckBox5Icno()
{
    // 设置点击响应
    layout->res_ischecked[4] = !layout->res_ischecked[4];
    this->setCheckBoxIcon();
}

void MainWindow::setCheckBox6Icno()
{
    // 设置点击响应
    layout->res_ischecked[5] = !layout->res_ischecked[5];
    this->setCheckBoxIcon();
}

void MainWindow::setCheckBox7Icno()
{
    // 设置点击响应
    layout->res_ischecked[6] = !layout->res_ischecked[6];
    this->setCheckBoxIcon();
}

void MainWindow::setCheckBox8Icno()
{
    // 设置点击响应
    layout->res_ischecked[7] = !layout->res_ischecked[7];
    this->setCheckBoxIcon();
}

