#include "MainWindowLayout.h"


MainLayout::MainLayout(QWidget *parent)
    : QWidget(parent)
{
    uncheckpixmap = new QPixmap (":/check/image/UnChecked_new.png");
    checkpixmap = new QPixmap (":/check/image/UnChecked_png.png");
    font.setPointSize(14);
    for(int i = 0;i<ARRAY;i++)
    {
        res_list[i] = new QListView(this);
        res_list[i]->setFont(font);
        res_ischecked[i] = true;
        res_button[i] = new QPushButton(this);
        res_button[i]->setFont(font);
    }

    Temperature_list = new QListView(this);
    Humidity_list = new QListView(this);
    heat1_list = new QListView(this);

    res_list[0]->setGeometry(290, 40, 101, 35);
    res_list[1]->setGeometry(490, 40, 101, 35);
    res_list[2]->setGeometry(690, 40, 101, 35);
    res_list[3]->setGeometry(890, 40, 101, 35);
    res_list[4]->setGeometry(290, 90, 101, 35);
    res_list[5]->setGeometry(490, 90, 101, 35);
    res_list[6]->setGeometry(690, 90, 101, 35);
    res_list[7]->setGeometry(890, 90, 101, 35);
    Temperature_list->setGeometry(30, 40, 101, 35);
    Humidity_list->setGeometry(30, 90, 101, 35);
    heat1_list->setGeometry(1050, 90, 101, 35);

    savebutton = new QPushButton(this);
    beginbutton = new QPushButton(this);
    stopbutton = new QPushButton(this);
    clearbutton = new QPushButton(this);
    heatbutton = new QPushButton(this);

    this->setButtonIcon();
    this->axisInit();
    this->labelInit();

}

MainLayout::~MainLayout()
{

}

void MainLayout::labelInit()
{
    font.setPointSize(16);
    // 温度标签
    temperature_label = new QLabel(this);
    temperature_label->setText("℃");
    temperature_label->setFont(font);
    temperature_label->setAlignment(Qt::AlignCenter);
    temperature_label->setGeometry(130,30,50,50);
    // 湿度标签
    humidity_label = new QLabel(this);
    humidity_label->setText("% RH");
    humidity_label->setFont(font);
    humidity_label->setAlignment(Qt::AlignCenter);
    humidity_label->setGeometry(140,80,70,50);
    // wifi标签
    wifi_label = new QLabel(this);
    wifi_label->setText("wifi 未连接！");
    wifi_label->setFont(font);
    wifi_label->setAlignment(Qt::AlignCenter);
    wifi_label->setGeometry(1040,650,200,50);

    font.setPointSize(14);
    // 加热标签
    heat_label = new QLabel(this);
    heat_label->setText("通道加热(V)");
    heat_label->setFont(font);
    heat_label->setAlignment(Qt::AlignCenter);
    heat_label->setGeometry(1015,30,200,50);
}
void MainLayout::axisInit()
{
    /***********************画图*****************************/
    draw_curve = new QChart();
    draw_curve->setTitle(u8"阵列数据曲线");
    drawview = new QChartView(draw_curve);
    axisX = new QValueAxis;
    axisY = new QValueAxis;
    //建立数据源队列

    //建立坐标轴
    QBrush AxisColor;
    AxisColor.setColor(Qt::black);
    axisX->setRange(0, 50);                // 设置范围
    axisX->setLabelFormat("%d");                            // 设置刻度的格式
    axisX->setGridLineVisible(true);                        // 网格线可见
    axisX->setTickCount(6);                                 // 设置多少个大格
    axisX->setMinorTickCount(1);                            // 设置每个大格里面小刻度线的数目
    axisX->setTitleText(u8"时间");                             // 设置描述

    axisY->setRange(0, 100);
    axisY->setLabelFormat("%.1f");
    axisY->setGridLineVisible(true);
    axisY->setTickCount(8);
    axisY->setMinorTickCount(1);
    axisY->setTitleText(u8"数据");
    //为曲线添加坐标轴
    draw_curve->addAxis(axisX, Qt::AlignBottom);               // 下：Qt::AlignBottom  上：Qt::AlignTop
    draw_curve->addAxis(axisY, Qt::AlignLeft);                 // 左：Qt::AlignLeft    右：Qt::AlignRight
    draw_curve->legend()->hide();                              //隐藏图例
    //chart放入chartview内
    drawview->setRenderHint(QPainter::Antialiasing);

    // 创建垂直布局对象
    QVBoxLayout *cruve_layout = new QVBoxLayout();
    // 将 drawview 部件添加到垂直布局中
    cruve_layout->addWidget(drawview);
    // 创建容器部件
    QWidget *container = new QWidget(this);
    // 将垂直布局设置为容器部件的布局
    container->setLayout(cruve_layout);
    // 设置容器部件的位置和大小
    container->setGeometry(20, 150, 1000, 550);

}
void MainLayout::setButtonIcon()
{
    QPalette palette;

    //设置位置
    res_button[0]->setGeometry(240, 40, 50, 35);
    res_button[1]->setGeometry(440, 40, 50, 35);
    res_button[2]->setGeometry(640, 40, 50, 35);
    res_button[3]->setGeometry(840, 40, 50, 35);
    res_button[4]->setGeometry(240, 90, 50, 35);
    res_button[5]->setGeometry(440, 90, 50, 35);
    res_button[6]->setGeometry(640, 90, 50, 35);
    res_button[7]->setGeometry(840, 90, 50, 35);

    for(int i = 0;i<ARRAY;i++) {
        res_button[i]->setText(QString::number(i));//设置通道数字
        palette = res_button[i]->palette();
        palette.setColor(QPalette::ButtonText, Graph_color[i]);
        res_button[i]->setPalette(palette);
        res_button[i]->setStyleSheet("text-align:right; padding-right:0px;");// 将文本与图标一起显示
        res_button[i]->setIcon(*checkpixmap);//显示默认图标
        res_button[i]->setIconSize(QSize(35, 35));//设置图标大小
        res_button[i]->setFlat(true);    // 设置无边框显示
    }

    savebutton->setGeometry(0, 0, 90, 35);
    savebutton->setIcon(QIcon(":/check/image/saveBtn-up.png"));
    savebutton->setIconSize(QSize(90, 35));
    savebutton->setFlat(true);

    beginbutton->setGeometry(1060, 200, 171, 75);
    beginbutton->setIcon(QIcon(":/check/image/begin.png"));
    beginbutton->setIconSize(QSize(171, 75));
    beginbutton->setFlat(true);

    stopbutton->setGeometry(1060, 350, 171, 75);
    stopbutton->setIcon(QIcon(":/check/image/stop.png"));
    stopbutton->setIconSize(QSize(171, 75));
    stopbutton->setFlat(true);

    clearbutton->setGeometry(1060, 500, 171, 75);
    clearbutton->setIcon(QIcon(":/check/image/clear.png"));
    clearbutton->setIconSize(QSize(171, 75));
    clearbutton->setFlat(true);

    heatbutton->setGeometry(1160, 90, 91, 40);
    heatbutton->setIcon(QIcon(":/check/image/send_heat.png"));
    heatbutton->setIconSize(QSize(91, 40));
    heatbutton->setFlat(true);

}
//重写paintEvent函数 隐式调用
void MainLayout::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen(Qt::lightGray);      // 创建宽度为2的画笔
    painter.setPen(pen);  // 设置绘图设备的画笔为宽度为2的画笔
    painter.setPen(Qt::lightGray);
    horizontal_line1.setLine(0,0,1280,0);
    vertical_line1.setLine(220,0,220,150);
    vertical_line2.setLine(1020,0,1020,720);
    painter.drawLine(horizontal_line1);
    painter.drawLine(vertical_line1);
    painter.drawLine(vertical_line2);
}
