#ifndef DATALOAD_H
#define DATALOAD_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"



class DataLoad
{
    public:
        DataLoad();
        bool loadData(std::vector<Anchor>& anchors, std::vector<Point>& line_points, bool& loaded);
    private:
        QString FileName;

};


#endif // DATALOAD_H
