#ifndef FRISS_FILTER_H
#define FRISS_FILTER_H
#include "mainwindow.h"

class Friss_Filter
{
public:
    Friss_Filter();
    std::vector<Anchor> filter_by_friis(const std::vector<Anchor>& anchors, const Anchor& tag, double P_t, double freq);
private:
    double dis_3D(Anchor a, Anchor b);
    double friis_path_loss(double distance, double frequency, double P_t);
};

#endif // FRISS_FILTER_H
