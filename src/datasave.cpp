#include "datasave.h"

DataSave::DataSave() {}

bool DataSave::save_data(const std::vector<Anchor>& anchors)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save CSV File", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
        return false;

    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Unable to save file");
        return false;
    }

    QTextStream out(&file);

    out << "UWB Address;Serial num;ID;reference;X;Y;Z\n";

    for (const Anchor& p : anchors)
    {
        out << p.uwbAddress << ";"
            << p.serialNumber << ";"
            << p.ID << ";"
            << p.ref << ";"
            << p.x << ";"
            << p.y << ";"
            << p.z << "\n";
    }

    file.close();
    QMessageBox::information(nullptr, "Info", "Data was saved successfully.");
    return true;
}

bool DataSave::save_data_poly(const std::vector<Anchor>& anchors, const std::vector<Point>& line_points)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save CSV File", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
        return false;

    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Unable to save file");
        return false;
    }

    QTextStream out(&file);

    out << "UWB Address;Serial num;ID;reference;X;Y;Z\n";

    // Zápis jednotlivych kotev
    for (const Anchor& p : anchors)
    {
        out << p.uwbAddress << ";"
            << p.serialNumber << ";"
            << p.ID << ";"
            << p.ref << ";"
            << p.x << ";"
            << p.y << ";"
            << p.z << "\n";
    }

    if(line_points.size() > 2)
    {
        out << "Polygon\n";

        for (const Point& t : line_points)
        {
            out << t.x << ";" << t.y << "\n";
        }
    }

    file.close();
    QMessageBox::information(nullptr, "Info", "Data was saved successfully.");
    return true;
}
