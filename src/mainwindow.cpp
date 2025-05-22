
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dataload.h"
#include "chartwidget.h"
#include "datasave.h"
#include "polygon.h"
#include "dopsave.h"
#include "anchoradd.h"
#include "anchorpairing.h"
#include "friss_filter.h"
#include "colorbarwidget.h"

#include "axessetting.h"
#include "heatmap.h"
#include "calculatepoints.h"
#include "calculate_dop.h"

#include <QImage>
#include <QPainter>
#include <QColor>
#include <QMessageBox>
#include <math.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QDialog>
#include <QLabel>
#include <QSplitter>
#include <QLineSeries>
#include <QTableWidget>
#include <QButtonGroup>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), dataLoaded(false), polygonSet(false), axes {0,0,100,100}, resolution{500,500}, computed(false), methodChange(1), lastValue(0)
{


    ui->setupUi(this);
    QHBoxLayout *chartLayout = new QHBoxLayout();
    chartWidget = new ChartWidget(this);
    colorBar = new ColorBarWidget(this);
    chartLayout->addWidget(colorBar);
    chartLayout->addWidget(chartWidget);

    ui->chartContainer->setLayout(chartLayout);


    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "X", "Y", "Z"});
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->Slider->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setText("Z = 0");

    ui->Horizont_Slider_dis->setMinimum(1);
    ui->Horizont_Slider_dis->setMaximum(40);
    ui->Horizont_Slider_dis->setValue(20);
    ui->Label_distance->setText(QString("Distance: 2"));

    QButtonGroup *group1 = new QButtonGroup(this);
    group1->addButton(ui->GDOP_radio);
    group1->addButton(ui->HDOP_radio);
    group1->addButton(ui->VDOP_radio);
    QButtonGroup *group2 = new QButtonGroup(this);
    group2->addButton(ui->TDoA_radio);
    group2->addButton(ui->ToA_radio);
    group2->addButton(ui->VisibleAnch_radio);



    //connect of buttons
    connect(ui->DrawPolygon_btn, &QPushButton::clicked, this, &MainWindow::onDrawPolygon);
    connect(ui->CalcDOP_btn, &QPushButton::clicked, this, &MainWindow::onCalculateDop);
    connect(ui->AddAnchor_btn, &QPushButton::clicked, this, &MainWindow::onAddAnchor);
    connect(ui->SetAxes_btn, &QPushButton::clicked, this, &MainWindow::onSetAxes);
    connect(ui->Clear_btn, &QPushButton::clicked, this, &MainWindow::onClearAll);

    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::tableItemChange);
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &MainWindow::onCellClicked);

    connect(ui->Horizont_Slider_dis, &QSlider::valueChanged, this, &MainWindow::horizontSliderUpdate);
    connect(ui->Slider, &QSlider::valueChanged, this, &MainWindow::sliderUpdate);
    connect(ui->actionSave_DOP, &QAction::triggered, this, &MainWindow::on_actionSaveDOP_triggered);
    colorBar->hide();

}



MainWindow::~MainWindow()
{
    delete chartWidget;
    delete ui;
}


void MainWindow::updateAnchorMap(const std::vector<Anchor>& Anchors)
{
    chartWidget->removeHeatmaps_id();
    HeatMap map;
    anchorsImage = map.createAnchorsIm(anchors,500,500,axes);
    chartWidget->displayHeatmap_id(anchorsImage,"Anchors" , 2);
    ui->Anchors_check->setChecked(true);
}


void MainWindow::updateAnchors(const std::vector<Anchor>& Anchors)
{

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "X", "Y", "Z"});
    for (int i = 0; i < anchors.size(); ++i)
    {
        chartWidget->draw_points(anchors[i].x, anchors[i].y);
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        setRowInTable(row, anchors[i].ID, anchors[i].x, anchors[i].y, anchors[i].z);
    }
    updateAnchorMap(anchors);
}




void MainWindow::on_actionSave_Data_triggered()
{
    DataSave DataS;
    DataS.save_data_poly(anchors,polygonPoints);
}

void MainWindow::on_actionLoad_Data_triggered()
{
    DataLoad DataL;
    if (DataL.loadData(anchors, polygonPoints, dataLoaded))
    {
        update();
        ui->tableWidget->setRowCount(0);
        chartWidget->clear_series();
    }
    updateAnchors(anchors);
    if(polygonPoints.size() > 2)
    {
        HeatMap map;
        QColor lineColor = QColor(0, 0, 255);
        int lineWidth = 2;
        polygonImage = map.createPolygonIm(polygonPoints, resolution.Width, resolution.Height,axes, lineColor, lineWidth);
        chartWidget->displayHeatmap_id(polygonImage, "Polygon", 2);
        ui->Polygon_check->setChecked(true);
        ui->DrawPolygon_btn->hide();
    }

}

void MainWindow::on_actionSaveDOP_triggered()
{
    if(computed == false)
    {
       QMessageBox::information(this, "Canceled", "You need compute DOP first.");
    }
    else
    {
        DopSave save;
        save.save_dop(anchors,tagPoints,zValues);
    }
}

void MainWindow::horizontSliderUpdate()
{
    double realValue = ui->Horizont_Slider_dis->value() / 10.0;
    ui->Label_distance->setText(QString("Distance: %1").arg(realValue, 0, 'f', 1));
}



void MainWindow::sliderUpdate(){
    // Hide last heatmaps
    chartWidget->HideHeatmap_id("TDoA_GDOP_" + QString::number(lastValue));
    chartWidget->HideHeatmap_id("TDoA_HDOP_" + QString::number(lastValue));
    chartWidget->HideHeatmap_id("TDoA_VDOP_" + QString::number(lastValue));
    chartWidget->HideHeatmap_id("ToA_GDOP_" + QString::number(lastValue));
    chartWidget->HideHeatmap_id("ToA_HDOP_" + QString::number(lastValue));
    chartWidget->HideHeatmap_id("ToA_VDOP_" + QString::number(lastValue));

    int sliderValue = ui->Slider->value();
    double value = minZ + sliderValue * (static_cast<double>(maxZ - minZ) / (stepsZ - 1));

    ui->textEdit->setText("Z = " + QString::number(value));    // Zobrazíme novou heatmapu
    int newIndex = sliderValue;

    if (methodChange == 1) {
        if (ui->GDOP_radio->isChecked())
            chartWidget->ShowHeatmap_id("TDoA_GDOP_" + QString::number(newIndex));
        if (ui->HDOP_radio->isChecked())
            chartWidget->ShowHeatmap_id("TDoA_HDOP_" + QString::number(newIndex));
        if (ui->VDOP_radio->isChecked())
            chartWidget->ShowHeatmap_id("TDoA_VDOP_" + QString::number(newIndex));
        lastValue = newIndex;
    }

    if (methodChange == 2) {
        if (ui->GDOP_radio->isChecked())
            chartWidget->ShowHeatmap_id("ToA_GDOP_" + QString::number(newIndex));
        if (ui->HDOP_radio->isChecked())
            chartWidget->ShowHeatmap_id("ToA_HDOP_" + QString::number(newIndex));
        if (ui->VDOP_radio->isChecked())
            chartWidget->ShowHeatmap_id("ToA_VDOP_" + QString::number(newIndex));
        lastValue = newIndex;
    }
}

void MainWindow::setRowInTable(int row, int id, double x, double y, double z)
{

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(x)));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(y)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(z)));

}

void MainWindow::addAnchorPoint(const QPointF &point)
{
    if(computed != true)
    {
        if (polygonSet == false)
        {
            Anchor p;
            p.ID = anchors.size();
            p.x = point.x();
            p.y = point.y();
            p.z = 0;
            p.uwbAddress = 0;
            p.ref = 0;
            p.serialNumber = 0;

            anchors.push_back(p);
            updateAnchors(anchors);

        }
        else
        {
            Point p;
            p.x = point.x();
            p.y = point.y();
            p.z = 0;
            polygonPoints.push_back(p);
            chartWidget->draw_line_between_points(p.x,p.y);
        }
    }

}

void MainWindow::createTagsMaps()
{
    HeatMap map;
    std::vector<TestingPoints> tmp;
    for (int i = 0; i < anchors.size(); i++) {
        for (TestingPoints& p : tagPoints)
        {
            for (Anchor& a : p.visibleAnchs) {
                if(a.ID == i)
                    tmp.push_back(p);
            }
        }
        tagHeatmas.push_back(map.createVisiblePintsIm(tmp,resolution.Width,resolution.Height,axes,anchors[i]));
        tmp.clear();
    }
    for (int x = 0; x < tagHeatmas.size(); x++) {
        QString idPrefix = QString::number(x);
        chartWidget->displayHeatmap_id(tagHeatmas[x],"TagHeatMap_"+ idPrefix ,2);
        chartWidget->HideHeatmap_id("TagHeatMap_"+ idPrefix);
    }

}



void MainWindow::onCellClicked(int row, int column)
{
    for (int x = 0; x < tagHeatmas.size(); x++) {
        QString idPrefix = QString::number(x);
        chartWidget->HideHeatmap_id("TagHeatMap_"+ idPrefix);
    }
    if(column == 0 && computed == true)
    {
        QString idPrefix = QString::number(row);
        chartWidget->ShowHeatmap_id("TagHeatMap_"+idPrefix);
    }

}



void MainWindow::tableItemChange(QTableWidgetItem *item)
{
    int row = item->row();
    int col = item->column();
    QString newValue = item->text();


    if (row >= 0 && row < anchors.size()) {
        Anchor &anchor = anchors[row];
        if (col == 0) {
            anchor.ID = newValue.toInt();
        } else if (col == 1) {
            anchor.x = newValue.toDouble();
        } else if (col == 2) {
            anchor.y = newValue.toDouble();
        } else if (col == 3) {
            anchor.z = newValue.toDouble();
        }
    }
    chartWidget->clear_series();
    updateAnchorMap(anchors);

}




void MainWindow::onDrawPolygon()
{
    if(polygonSet == true)
    {
        polygonSet = false;
        chartWidget->set_bool();
        chartWidget->draw_line_between_points(polygonPoints[0].x,polygonPoints[0].y);
        chartWidget->clear_series();
        HeatMap map;
        QColor lineColor = QColor(0, 0, 255);
        int lineWidth = 2;
        polygonImage = map.createPolygonIm(polygonPoints, resolution.Width, resolution.Height, axes, lineColor, lineWidth);
        chartWidget->displayHeatmap_id(polygonImage, "Polygon", 2);
        ui->Polygon_check->setChecked(true);
        ui->DrawPolygon_btn->hide();
    }
    else
    {
        chartWidget->set_bool();
        polygonSet = true;
        ui->DrawPolygon_btn->setText("Stop Drawing");
    }
}


void MainWindow::getPairs()
{
    // pairing by ID
     for (TestingPoints& p : tagPoints)
     {

        if (!p.visibleAnchs.empty()) {
            IDComb tmp;
            tmp.firstID = 0;
            bool first = true;


            for (int x = 1; x < p.visibleAnchs.size()-1; x++) {
                tmp.secondID = x;
                p.reference.push_back(tmp);
            }

        }
     }

     // pairing (0-1),(0-2)....
     for (TestingPoints& p : tagPoints)
     {
         if(p.visibleAnchs.size() < 2)
             continue;
            IDComb tmp;
            for (int x = 0; x < p.visibleAnchs.size()-1; x++) {
                 tmp.firstID = x;
                 tmp.secondID = x+1;
                 p.pairs.push_back(tmp);
            }
     }


}



void MainWindow::getVisibleAnchors()
{
    polygon poly;
    Friss_Filter f;
    for (TestingPoints& p : tagPoints)
    {
        Anchor z;
        z.x = p.x;
        z.y = p.y;
        Point tmp;
        tmp.x = p.x;
        tmp.y = p.y;
        std::vector<Anchor> tmp_a;
        int x = poly.countVisibleAnchors(tmp,anchors,polygonPoints,tmp_a);
        p.visibleAnchs = f.filter_by_friis(tmp_a,z, -14.3 , 3.9936e9);
        visibleAnchors.push_back(p.visibleAnchs.size());
        tmp_a.clear();
    }
}



void MainWindow::onCalculateDop()
{
    bool check = true;

    Calculate_DOP d;
    HeatMap map;
    polygon poly;
    if(polygonPoints.empty() == true)
    {
        QMessageBox::information(this, "Canceled", "You need to draw polygon first.");
        check = false;
    }
    if(anchors.size() < 4)
    {
        QMessageBox::information(this, "Canceled", "You need atleast 3 anchors.");
        check = false;

    }

    AnchorPairing pair(this);
    int pairnum;
    if (pair.exec() == QDialog::Accepted)
    {
        // 1 ID 2 pary
        pairnum = pair.getSelectedOption();
    }
    else
    {
        check = false;
        QMessageBox::information(this, "Canceled", "Someting went wrong.");
    }

    if(check == true)
    {
        colorBar->show();
        //vypocet bodu
        CalculatePoints calc;
        calc.Points_To_Calc(polygonPoints,points_calc,tagPoints,ui->Horizont_Slider_dis->value()/10.0);
        ui->Horizont_Slider_dis->setEnabled(false);
        getVisibleAnchors();
        getPairs();

        // SETTING OF Z RANGE
        minZ = -10;
        maxZ = 10;
        stepsZ = 41;

        double step = static_cast<double>(maxZ - minZ) / (stepsZ - 1);

        for (int i = 0; i < stepsZ; ++i) {
            double value_z = minZ + i * step;
            DOPValues tmp_d;
            for (TestingPoints& p : tagPoints)
            {

                Anchor tmp_a;
                tmp_a.x = p.x;
                tmp_a.y = p.y;
                tmp_a.z = value_z;
                if(pairnum == 1)
                    p.dopValuesTDoA.push_back(d.TDoA_gdop_3D_pair(p.visibleAnchs,p.reference,p.reference.size(),tmp_a));
                if(pairnum == 2)
                    p.dopValuesTDoA.push_back(d.TDoA_gdop_3D_pair(p.visibleAnchs,p.pairs,p.pairs.size(),tmp_a));
                p.dopValuesToA.push_back(d.ToA_gdop_3D(p.visibleAnchs,p.visibleAnchs.size(),tmp_a));


            }
            dopHeatmaps.TDoA_im.push_back(map.createHeatmapsImForDOPs(tagPoints,anchors, resolution.Width, resolution.Height,axes, -1, i, 1));
            dopHeatmaps.ToA_im.push_back(map.createHeatmapsImForDOPs(tagPoints,anchors, resolution.Width, resolution.Height,axes, -1, i, 2));
            zValues.push_back(value_z);

        }

        testingPointsImage = map.createTestingPointsIm(tagPoints,resolution.Width,resolution.Height,axes);
        chartWidget->displayHeatmap_id(testingPointsImage, "TestingPoints",2);
        chartWidget->HideHeatmap_id("TestingPoints");
        visibleAnchorsImage = map.createHeatmapIm(points_calc, visibleAnchors,anchors, resolution.Width, resolution.Height,axes, anchors.size()-1);
        chartWidget->displayHeatmap_id(visibleAnchorsImage, "VisibleAnchors",1);
        chartWidget->HideHeatmap_id("VisibleAnchors");
        createTagsMaps();
        computed = true;
    }

    //Display of all DOP maps
    for (int i = 0; i < dopHeatmaps.TDoA_im.size(); i++) {
        QString idPrefix = QString::number(i);

        chartWidget->displayHeatmap_id(dopHeatmaps.TDoA_im[i].GDOP_im, "TDoA_GDOP_" + idPrefix, 1);
        chartWidget->HideHeatmap_id("TDoA_GDOP_" + idPrefix);

        chartWidget->displayHeatmap_id(dopHeatmaps.ToA_im[i].GDOP_im, "ToA_GDOP_" + idPrefix, 1);
        chartWidget->HideHeatmap_id("ToA_GDOP_" + idPrefix);

        chartWidget->displayHeatmap_id(dopHeatmaps.TDoA_im[i].HDOP_im, "TDoA_HDOP_" + idPrefix, 1);
        chartWidget->HideHeatmap_id("TDoA_HDOP_" + idPrefix);

        chartWidget->displayHeatmap_id(dopHeatmaps.ToA_im[i].HDOP_im, "ToA_HDOP_" + idPrefix, 1);
        chartWidget->HideHeatmap_id("ToA_HDOP_" + idPrefix);

        chartWidget->displayHeatmap_id(dopHeatmaps.TDoA_im[i].VDOP_im, "TDoA_VDOP_" + idPrefix, 1);
        chartWidget->HideHeatmap_id("TDoA_VDOP_" + idPrefix);

        chartWidget->displayHeatmap_id(dopHeatmaps.ToA_im[i].VDOP_im, "ToA_VDOP_" + idPrefix, 1);
        chartWidget->HideHeatmap_id("ToA_VDOP_" + idPrefix);
    }
    ui->Slider->setEnabled(true);
    ui->Slider->setRange(0, (stepsZ-1));
}

void MainWindow::onAddAnchor()
{
    Anchor p;
    AnchorAdd dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        p.x = dialog.get_x();
        p.y = dialog.get_y();
        p.z = dialog.get_z();
        p.ID = anchors.size();
        p.uwbAddress = 0;
        p.ref = 0;
        p.serialNumber = 0;
        anchors.push_back(p); // Uložení
        updateAnchors(anchors);
    }
    else
        QMessageBox::information(this, "Canceled", "Input was canceled.");
}

void MainWindow::onSetAxes()
{
    HeatMap map;
    AxesSetting dialog(this);
    if(dialog.exec() == QDialog::Accepted)
    {
        axes.minX = dialog.get_min_x();
        axes.maxX = dialog.get_max_x();

        axes.minY = dialog.get_min_y();
        axes.maxY = dialog.get_max_y();
        chartWidget->set_axis(axes);
        updateAnchors(anchors);

        if(polygonPoints.size() > 2)
        {
            QColor lineColor = QColor(0, 0, 255);
            int lineWidth = 2;
            polygonImage = map.createPolygonIm(polygonPoints, resolution.Width, resolution.Height,axes, lineColor, lineWidth);

            chartWidget->displayHeatmap_id(polygonImage, "Polygon", 2);
            chartWidget->HideHeatmap_id("Polygon");
            chartWidget->ShowHeatmap_id("Polygon");
        }

    }
    else
        QMessageBox::information(this, "Canceled", "Input was canceled.");
}



void MainWindow::on_Anchors_check_stateChanged(int state)
{
    if (state == Qt::Checked)
        chartWidget->ShowHeatmap_id("Anchors");
    else
        chartWidget->HideHeatmap_id("Anchors");
}

void MainWindow::on_Polygon_check_stateChanged(int state)
{
    if (state == Qt::Checked)
        chartWidget->ShowHeatmap_id("Polygon");
    else
        chartWidget->HideHeatmap_id("Polygon");
}


void MainWindow::on_Tags_check_stateChanged(int state)
{
    if (state == Qt::Checked)
        chartWidget->ShowHeatmap_id("TestingPoints");
    else
        chartWidget->HideHeatmap_id("TestingPoints");
}



void MainWindow::hideAllMaps()
{
    for (int i = 0; i < dopHeatmaps.TDoA_im.size(); ++i) {
           QString indexStr = QString::number(i);

           chartWidget->HideHeatmap_id("TDoA_GDOP_" + indexStr);
           chartWidget->HideHeatmap_id("TDoA_HDOP_" + indexStr);
           chartWidget->HideHeatmap_id("TDoA_VDOP_" + indexStr);

           chartWidget->HideHeatmap_id("ToA_GDOP_" + indexStr);
           chartWidget->HideHeatmap_id("ToA_HDOP_" + indexStr);
           chartWidget->HideHeatmap_id("ToA_VDOP_" + indexStr);
       }
       chartWidget->HideHeatmap_id("VisibleAnchors");
       for (int x = 0; x < tagHeatmas.size(); x++) {
           QString idPrefix = QString::number(x);
           chartWidget->HideHeatmap_id("TagHeatMap_"+ idPrefix);
       }
}



void MainWindow::on_VisibleAnch_radio_clicked()
{
    hideAllMaps();
    ui->Slider->setEnabled(false);
    chartWidget->ShowHeatmap_id("VisibleAnchors");
}


void MainWindow::on_TDoA_radio_clicked()
{
    hideAllMaps();
    methodChange = 1;
    ui->Slider->setEnabled(true);
    ui->Slider->setValue(0);
}



void MainWindow::on_ToA_radio_clicked()
{
    hideAllMaps();
    methodChange = 2;
    ui->Slider->setEnabled(true);
    ui->Slider->setValue(0);
}


void MainWindow::onClearAll()
{
    computed = false;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    chartWidget->removeHeatmaps_id();
    anchors.clear();
    polygonPoints.clear();
    points_calc.clear();
    chartWidget->clear_series();
    visibleAnchors.clear();
    tagPoints.clear();
    ui->DrawPolygon_btn->show();
    ui->DrawPolygon_btn->setText("Draw Polygon");
}


void MainWindow::on_pushButton_clicked()
{
    hideAllMaps();
}

