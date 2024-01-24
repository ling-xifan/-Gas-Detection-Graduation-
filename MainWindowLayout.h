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
using namespace QtCharts;

class MainLayout : public QWidget{

    Q_OBJECT
public:
    MainLayout(QWidget *parent = nullptr);
    ~MainLayout();
    friend class MainWindow;
private:
    //listview 显示
    QListView* res1_list;
    QListView* res2_list;
    QListView* res3_list;
    QListView* res4_list;
    QListView* res5_list;
    QListView* res6_list;
    QListView* res7_list;
    QListView* res8_list;
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

    QPushButton *res1_button;
    QPushButton *res2_button;
    QPushButton *res3_button;
    QPushButton *res4_button;
    QPushButton *res5_button;
    QPushButton *res6_button;
    QPushButton *res7_button;
    QPushButton *res8_button;
    QPushButton *savebutton;
    QPushButton *beginbutton;
    QPushButton *stopbutton;
    QPushButton *clearbutton;
    QPushButton *heatbutton;

    bool res1_ischecked;
    bool res2_ischecked;
    bool res3_ischecked;
    bool res4_ischecked;
    bool res5_ischecked;
    bool res6_ischecked;
    bool res7_ischecked;
    bool res8_ischecked;

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
