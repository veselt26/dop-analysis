#ifndef DOPSAVE_H
#define DOPSAVE_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"

class DopSave
{
public:
    DopSave();
    bool save_dop(std::vector<Anchor>& anchors, std::vector<TestingPoints> &tag_points, std::vector<double> &z_values);
};

#endif // DOPSAVE_H
