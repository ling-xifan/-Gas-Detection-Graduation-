#include "MainWindowLayout.h"


MainLayout::MainLayout(QWidget *parent)
    : QWidget(parent)
{
    uncheckpixmap = new QPixmap (":/check/image/UnChecked_new.png");
    checkpixmap = new QPixmap (":/check/image/UnChecked_png.png");

    res1_list = new QListView(this);
    res2_list = new QListView(this);
    res3_list = new QListView(this);
    res4_list = new QListView(this);
    res5_list = new QListView(this);
    res6_list = new QListView(this);
    res7_list = new QListView(this);
    res8_list = new QListView(this);

    Temperature_list = new QListView(this);
    Humidity_list = new QListView(this);
    heat1_list = new QListView(this);

    res1_list->setGeometry(290, 40, 101, 35);
    res2_list->setGeometry(490, 40, 101, 35);
    res3_list->setGeometry(690, 40, 101, 35);
    res4_list->setGeometry(890, 40, 101, 35);
    res5_list->setGeometry(290, 90, 101, 35);
    res6_list->setGeometry(490, 90, 101, 35);
    res7_list->setGeometry(690, 90, 101, 35);
    res8_list->setGeometry(890, 90, 101, 35);
    Temperature_list->setGeometry(30, 40, 101, 35);
    Humidity_list->setGeometry(30, 90, 101, 35);
    heat1_list->setGeometry(1050, 90, 101, 35);

    res1_ischecked = true;
    res2_ischecked = true;
    res3_ischecked = true;
    res4_ischecked = true;
    res5_ischecked = true;
    res6_ischecked = true;
    res7_ischecked = true;
    res8_ischecked = true;

    res1_button = new QPushButton(this);
    res2_button = new QPushButton(this);
    res3_button = new QPushButton(this);
    res4_button = new QPushButton(this);
    res5_button = new QPushButton(this);
    res6_button = new QPushButton(this);
    res7_button = new QPushButton(this);
    res8_button = new QPushButton(this);
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
    container->setGeometry(20, 160, 1000, 500);

}
void MainLayout::setButtonIcon()
{
    //设置位置
    res1_button->setGeometry(240, 40, 50, 35);
    res2_button->setGeometry(440, 40, 50, 35);
    res3_button->setGeometry(640, 40, 50, 35);
    res4_button->setGeometry(840, 40, 50, 35);
    res5_button->setGeometry(240, 90, 50, 35);
    res6_button->setGeometry(440, 90, 50, 35);
    res7_button->setGeometry(640, 90, 50, 35);
    res8_button->setGeometry(840, 90, 50, 35);
    //设置通道数字
    res1_button->setText(QString::number(1));
    res2_button->setText(QString::number(2));
    res3_button->setText(QString::number(3));
    res4_button->setText(QString::number(4));
    res5_button->setText(QString::number(5));
    res6_button->setText(QString::number(6));
    res7_button->setText(QString::number(7));
    res8_button->setText(QString::number(8));
    // 将文本与图标一起显示
    res1_button->setStyleSheet("text-align:right; padding-right:0px;");
    res2_button->setStyleSheet("text-align:right; padding-right:0px;");
    res3_button->setStyleSheet("text-align:right; padding-right:0px;");
    res4_button->setStyleSheet("text-align:right; padding-right:0px;");
    res5_button->setStyleSheet("text-align:right; padding-right:0px;");
    res6_button->setStyleSheet("text-align:right; padding-right:0px;");
    res7_button->setStyleSheet("text-align:right; padding-right:0px;");
    res8_button->setStyleSheet("text-align:right; padding-right:0px;");
    //显示默认图标
    res1_button->setIcon(*checkpixmap);
    res2_button->setIcon(*checkpixmap);
    res3_button->setIcon(*checkpixmap);
    res4_button->setIcon(*checkpixmap);
    res5_button->setIcon(*checkpixmap);
    res6_button->setIcon(*checkpixmap);
    res7_button->setIcon(*checkpixmap);
    res8_button->setIcon(*checkpixmap);
    //设置图标大小
    res1_button->setIconSize(QSize(35, 35));
    res2_button->setIconSize(QSize(35, 35));
    res3_button->setIconSize(QSize(35, 35));
    res4_button->setIconSize(QSize(35, 35));
    res5_button->setIconSize(QSize(35, 35));
    res6_button->setIconSize(QSize(35, 35));
    res7_button->setIconSize(QSize(35, 35));
    res8_button->setIconSize(QSize(35, 35));
    // 设置无边框显示
    res1_button->setFlat(true);
    res2_button->setFlat(true);
    res3_button->setFlat(true);
    res4_button->setFlat(true);
    res5_button->setFlat(true);
    res6_button->setFlat(true);
    res7_button->setFlat(true);
    res8_button->setFlat(true);


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
