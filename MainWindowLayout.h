#ifndef MAINWINDOWLAYOUT_H
#define MAINWINDOWLAYOUT_H
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QListView>
#include <QCheckBox>
#include <QPushButton>
#include <QPainter>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFont>
#include <QLabel>
#include <QVBoxLayout>

#define ARRAY 8
using namespace QtCharts;

const QColor Graph_color[ARRAY] = {
    QColor(255, 0, 0),//大红
    QColor(0, 255, 0),//黑色
    QColor(0, 0, 255),//Cyan
    QColor(255, 0, 255),//MediumSeaGreen
    QColor(255, 125, 0),//goldenrod
    QColor(0, 255, 255),//SandyBrown
    QColor(0, 0, 0),//DarkSeaGreen3
    QColor(125, 125, 0),//Gold2
    };


class MainLayout : public QWidget{

    Q_OBJECT
public:
    MainLayout(QWidget *parent = nullptr);
    ~MainLayout();
    friend class MainWindow;
private:

    //listview 显示
    QListView* res_list[ARRAY];
    QListView* Temperature_list;
    QListView* Humidity_list;
    QListView* heat1_list;

    QCheckBox* res1_box;
    QCheckBox* res2_box;
    QCheckBox* res3_box;
    QCheckBox* res4_box;
    QCheckBox* res5_box;
    QCheckBox* res6_box;
    QCheckBox* res7_box;
    QCheckBox* res8_box;

    QPushButton *res_button[ARRAY];
    QPushButton *savebutton;
    QPushButton *beginbutton;
    QPushButton *stopbutton;
    QPushButton *clearbutton;
    QPushButton *heatbutton;

    bool res_ischecked[ARRAY];

    QPixmap *uncheckpixmap;
    QPixmap *checkpixmap;

    QLine horizontal_line1;
    QLine vertical_line1;
    QLine vertical_line2;

    QLabel *temperature_label;
    QLabel *humidity_label;
    QLabel *wifi_label;
    QLabel *heat_label;
    QFont font;

    QValueAxis *axisX;
    QValueAxis *axisY;
    QChart *draw_curve;
    QChartView *drawview;

    void setButtonIcon();
    void axisInit();
    void labelInit();
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOWLAYOUT_H
