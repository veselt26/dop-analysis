#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include "colorbarwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Resolution
{
    int Width;
    int Height;
};

struct ChartAxes
{
    double minX;
    double minY;

    double maxX;
    double maxY;
};

struct Point
{
    double x;
    double y;
    double z;
};

struct Anchor
{
    double x;
    double y;
    double z;
    int ID;
    int ref;
    int uwbAddress;
    int serialNumber;
};

struct IDComb
{
    int firstID;
    int secondID;
};

struct DOPValues
{
    double GDOP;
    double HDOP;
    double VDOP;

};

struct TestingPoints
{
    double x;
    double y;
    double z;
    std::vector<Anchor> visibleAnchs;
    std::vector<IDComb> pairs;
    std::vector<IDComb> reference;
    std::vector<DOPValues> dopValuesTDoA;
    std::vector<DOPValues> dopValuesToA;

};

struct DOPImages
{
    QImage GDOP_im;
    QImage HDOP_im;
    QImage VDOP_im;
};

struct HeatmapsImages
{
    std::vector<DOPImages> ToA_im;
    std::vector<DOPImages> TDoA_im;
};


class ChartWidget;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void addAnchorPoint(const QPointF &point);
        void updateAnchors(const std::vector<Anchor>& Anchors);
        void updateAnchorMap(const std::vector<Anchor>& Anchors);

    private slots:
        void horizontSliderUpdate();
        void sliderUpdate();
        void setRowInTable(int row, int id, double x, double y, double z);
        void tableItemChange(QTableWidgetItem *item);
        void onCellClicked(int row, int column);



        void getPairs();
        void getVisibleAnchors();
        void createTagsMaps();

        void onDrawPolygon();
        void onCalculateDop();
        void onAddAnchor();
        void onSetAxes();
        void onClearAll();
        void hideAllMaps();

        // created by Qt
        void on_actionSave_Data_triggered();
        void on_actionLoad_Data_triggered();
        void on_actionSaveDOP_triggered();

        void on_Anchors_check_stateChanged(int state);
        void on_Tags_check_stateChanged(int state);
        void on_Polygon_check_stateChanged(int state);

        void on_TDoA_radio_clicked();
        void on_VisibleAnch_radio_clicked();
        void on_ToA_radio_clicked();
        void on_pushButton_clicked();



    private:

        Ui::MainWindow *ui;

        ChartWidget *chartWidget;
        ChartAxes axes;
        Resolution resolution;

        ColorBarWidget *colorBar;

        std::vector<Anchor> anchors;
        std::vector<TestingPoints> tagPoints;
        std::vector<Point> polygonPoints;
        std::vector<Anchor> points_calc;


        int methodChange;

        std::vector<double> visibleAnchors;

        bool dataLoaded;
        bool polygonSet;
        bool computed;


        std::vector<QImage> tagHeatmas;
        QImage testingPointsImage;
        QImage visibleAnchorsImage;
        QImage anchorsImage;
        QImage polygonImage;
        HeatmapsImages dopHeatmaps;

        int lastValue;
        double minZ;
        double maxZ;
        double stepsZ;
        std::vector<double> zValues;


};
#endif // MAINWINDOW_H
