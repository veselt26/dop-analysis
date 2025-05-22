#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QMouseEvent>
#include <QLineSeries>
#include <QtCharts/QChart>
#include <QGraphicsPixmapItem>
#include "mainwindow.h"

class MainWindow;

struct HeatmapEntry {
    QGraphicsPixmapItem* item;
    QImage image;
    QString id;
};

class ChartWidget : public QChartView {
    Q_OBJECT

public:
    explicit ChartWidget(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~ChartWidget();
    void clear_series();
    void draw_points(double x, double y);
    void draw_line_between_points(double x, double y);
    void set_bool();
    void removeHeatmaps_id();
    void set_axis(const ChartAxes& axes);
    void displayHeatmap_id(const QImage &image, const QString& id, int z_value);
    void HideHeatmap_id(const QString& id);
    void ShowHeatmap_id(const QString& id);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    QScatterSeries *series;
    MainWindow *mainWindow;
    QLineSeries *lineSeries;
    bool bo_line;
    QVector<HeatmapEntry> heatmapItems_id;
    void updateHeatmaps();

};

#endif // CHARTWIDGET_H
