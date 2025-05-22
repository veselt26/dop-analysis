#ifndef DATASAVE_H
#define DATASAVE_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"

class DataSave
{
public:
    DataSave();
    bool save_data(const std::vector<Anchor>& anchors);
    bool save_data_poly(const std::vector<Anchor>& anchors, const std::vector<Point>& line_points);
};

#endif // DATASAVE_H
