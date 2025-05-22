#include "dataload.h"

DataLoad::DataLoad() {}

bool DataLoad::loadData(std::vector<Anchor>& anchors, std::vector<Point>& line_points, bool& loaded)
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open CSV File", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
        return false;

    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        QMessageBox::warning(nullptr, "Error", "Selected file is not a CSV file.");
        return false;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Unable to open file");
        return false;
    }

    QTextStream in(&file);
    anchors.clear();
    line_points.clear();

    QString line = in.readLine();
    if(line != "UWB Address;Serial num;ID;reference;X;Y;Z")
    {
        QMessageBox::warning(nullptr, "Error", "Wrong format");
        return false;
    }

    bool check = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line == "Polygon")
        {
            check = true;
            break;
        }
        QStringList values = line.split(';');

        if (values.size() != 7) {
            QMessageBox::warning(nullptr, "Error", "Invalid line format");
            return false;
        }

        Anchor p;
        p.uwbAddress = values[0].toInt();
        p.serialNumber = values[1].toInt();
        p.ID = values[2].toInt();
        p.ref = values[3].toInt();
        p.x = values[4].toDouble();
        p.y = values[5].toDouble();
        p.z = values[6].toDouble();
        anchors.push_back(p);
    }


    if(check == true)
    {

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList values = line.split(';');

            if (values.size() != 2) {
                QMessageBox::warning(nullptr, "Error", "Invalid line format");
                return false;
            }

            Point t;
            t.x = values[0].toDouble();
            t.y= values[1].toDouble();
            line_points.push_back(t);
        }
    }

    file.close();
    loaded = true;
    QMessageBox::information(nullptr, "Info", "Data was loaded successfully.");
    return true;
}
