#ifndef CALCULATE_DOP_H
#define CALCULATE_DOP_H

#include "mainwindow.h"
#include <vector>
#include <math.h>

class Calculate_DOP
{
    public:
        Calculate_DOP();
        DOPValues ToA_gdop_3D(std::vector<Anchor>& anchors, int n, Anchor tag);
        DOPValues TDoA_gdop_3D_pair(std::vector<Anchor>& anchors, std::vector<IDComb>& comb, int n, Anchor tag);
};

#endif // CALCULATE_DOP_H
