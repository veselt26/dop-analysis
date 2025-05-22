#include "dopsave.h"
#include "mainwindow.h"

DopSave::DopSave() {}

bool DopSave::save_dop(std::vector<Anchor>& anchors, std::vector<TestingPoints> &tag_points, std::vector<double> &z_values)
{


    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save CSV File", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
    {
        QMessageBox::warning(nullptr, "Error", "No file selected");
        return false;
    }

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

    out << "ANCHORS: \n";
    out << "ID;X;Y;Z \n";
    for (const Anchor& p : anchors)
    {
        out << p.ID << ";"
            << p.x  << ";"
            << p.y  << ";"
            << p.z << "\n";
    }

    out << "X;Y;Z;TDoA_GDOP;TDoA_HDOP;TDoA_VDOP;ToA_GDOP;ToA_HDOP;ToA_VDOP;VisibleAnchors\n";
    for (int x = 0; x < z_values.size(); x++)
    {
        for (const TestingPoints& p : tag_points)
        {
            out << p.x << ";"
                << p.y << ";"
                << z_values[x] << ";"
                << p.dopValuesTDoA[x].GDOP << ";"
                << p.dopValuesTDoA[x].HDOP << ";"
                << p.dopValuesTDoA[x].VDOP << ";"
                << p.dopValuesToA[x].GDOP << ";"
                << p.dopValuesToA[x].HDOP << ";"
                << p.dopValuesToA[x].VDOP << ";";

            for (const Anchor& a : p.visibleAnchs)
            {
                out << a.ID << " ";
            }
            out << ";\n";
        }
    }
    file.close();
    QMessageBox::information(nullptr, "Info", "Data was saved successfully.");
    return true;

}

