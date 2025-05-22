// chartwidget.cpp

#include "chartwidget.h"
#include <QtCharts/QValueAxis>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "mainwindow.h"

ChartWidget::ChartWidget(MainWindow *mainWindow, QWidget *parent)
    : QChartView(new QChart(), parent), series(new QScatterSeries()), lineSeries(new QLineSeries()), mainWindow(mainWindow)//, pixmapItem(nullptr)
{
    chart()->addSeries(series);
    chart()->addSeries(lineSeries);
    chart()->setTitle("Click to add an anchor");

     chart()->legend()->hide();
    // default axes settings
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    axisX->setRange(0, 100);
    axisY->setRange(0, 100);
    chart()->addAxis(axisX, Qt::AlignBottom);
    chart()->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series->setMarkerSize(8.0);  // Velikost kotvy
    bo_line = false;

    lineSeries->attachAxis(chart()->axes(Qt::Horizontal).first());
    lineSeries->attachAxis(chart()->axes(Qt::Vertical).first());

    connect(chart(), &QChart::plotAreaChanged, this, &ChartWidget::updateHeatmaps);
}

// axes resize
void ChartWidget::set_axis(const ChartAxes& axes)
{
    if (auto axisX = dynamic_cast<QValueAxis *>(chart()->axes(Qt::Horizontal).first())) {
        axisX->setRange(axes.minX, axes.maxX);
    }
    if (auto axisY = dynamic_cast<QValueAxis *>(chart()->axes(Qt::Vertical).first())) {
        axisY->setRange(axes.minY, axes.maxY);
    }
}




void ChartWidget::updateHeatmaps() {

    if (heatmapItems_id.isEmpty())
        return;

    QRectF plotArea = chart()->plotArea();


    for (auto& entry : heatmapItems_id) {
        QGraphicsPixmapItem* pixmapItem = entry.item;
        QImage& image = entry.image;

        if (!pixmapItem || image.isNull())
            continue;

        QImage scaledHeatmap = image.scaled(
            plotArea.width(), plotArea.height(),
            Qt::IgnoreAspectRatio, Qt::SmoothTransformation
            );

        pixmapItem->setPixmap(QPixmap::fromImage(scaledHeatmap));
        pixmapItem->setPos(plotArea.left(), plotArea.top());
    }
}





void ChartWidget::set_bool()
{
    if(bo_line == true)
        bo_line = false;
    else
        bo_line = true;
}


void ChartWidget::clear_series()
{
    series->clear();
    lineSeries->clear();
}

void ChartWidget::draw_points(double x, double y)
{

    QPointF point ={x,y};
    series->append(point);

}


void ChartWidget::draw_line_between_points(double x, double y) {

    lineSeries->append(x, y);
}

void ChartWidget::resizeEvent(QResizeEvent* event) {
    QChartView::resizeEvent(event);
    updateHeatmaps();
}


void ChartWidget::mousePressEvent(QMouseEvent *event) {


        QPointF clickedPoint = chart()->mapToValue(event->pos());

        // add anchor to series
        if(bo_line == false)
            series->append(clickedPoint);

        if (mainWindow)
            mainWindow->addAnchorPoint(clickedPoint);

    QChartView::mousePressEvent(event);
}


void ChartWidget::displayHeatmap_id(const QImage &image, const QString& id, int z_value) {
    if (image.isNull()) {
        qWarning() << "Attempted to display a null image. Operation aborted.";
        return;
    }
    // Check if ID already exists
    for (const auto& entry : heatmapItems_id) {
        if (entry.id == id) {
            qWarning() << "Heatmap with ID" << id << "already exists.";
            return;
        }
    }
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem();
    pixmapItem->setZValue(z_value);
    chart()->scene()->addItem(pixmapItem);

    QRectF plotArea = chart()->plotArea();
    QImage scaledHeatmap = image.scaled(
        plotArea.width(), plotArea.height(),
        Qt::IgnoreAspectRatio, Qt::SmoothTransformation
    );

    pixmapItem->setPixmap(QPixmap::fromImage(scaledHeatmap));
    pixmapItem->setPos(plotArea.left(), plotArea.top());

    heatmapItems_id.append({ pixmapItem, image, id });
}

void ChartWidget::HideHeatmap_id(const QString& id) {
    for (auto& entry : heatmapItems_id) {
        if (entry.id == id) {
            if (entry.item) {
                entry.item->setVisible(false);
            }
            return;
        }
    }

    qWarning() << "Heatmap with ID" << id << "not found for hiding.";
}
void ChartWidget::ShowHeatmap_id(const QString& id) {
    for (auto& entry : heatmapItems_id) {
        if (entry.id == id) {
            if (entry.item) {
                entry.item->setVisible(true);
            }
            return;
        }
    }

    qWarning() << "Heatmap with ID" << id << "not found for showing.";
}

void ChartWidget::removeHeatmaps_id() {
    for (const auto& entry : heatmapItems_id) {
        QGraphicsPixmapItem* pixmapItem = entry.item;
        if (pixmapItem) {
            chart()->scene()->removeItem(pixmapItem);
            delete pixmapItem;
        }
    }
    heatmapItems_id.clear();
}


// destructor
ChartWidget::~ChartWidget()
{
    for (const auto& entry : heatmapItems_id) {
        QGraphicsPixmapItem* pixmapItem = entry.item;
        if (pixmapItem) {
            chart()->scene()->removeItem(pixmapItem);
            delete pixmapItem;
        }
    }
    heatmapItems_id.clear();
    chart()->removeSeries(series);
    chart()->removeSeries(lineSeries);
    delete series;
    delete lineSeries;
    mainWindow = nullptr;
}
