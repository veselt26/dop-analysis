#include "friss_filter.h"

Friss_Filter::Friss_Filter() {}

double Friss_Filter::friis_path_loss(double distance, double frequency, double P_t) {
    if (distance <= 0) return -400;
    double lambda = 3e8 / frequency;
    double loss = 20 * log10(lambda) - 20 * log10(4 * M_PI * distance);
    double P_r = P_t + loss;
    return P_r;
}


double Friss_Filter::dis_3D(Anchor a, Anchor b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

std::vector<Anchor> Friss_Filter::filter_by_friis(const std::vector<Anchor>& anchors, const Anchor& tag, double P_t, double freq) {
    std::vector<Anchor> valid_anchors;
    for (const auto& anchor : anchors) {
        double d = dis_3D(anchor, tag);
        if (d == 0) continue;
        double P_r = friis_path_loss(d, freq, P_t);
        if (P_r >= -107) {
            valid_anchors.push_back(anchor);

        }
    }
    return valid_anchors;
}


