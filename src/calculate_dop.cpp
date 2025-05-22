#include "calculate_dop.h"


Calculate_DOP::Calculate_DOP() {}


double distance_3D(Anchor a, Anchor b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}



int invert_matrix_3x3(double mat[3][3], double result[3][3])
{
    double det =  mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
    - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
        + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

    if (det == 0) {
        printf("Singularity\n");
        return -1;
    }

    result[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) / det;
    result[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) / det;
    result[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) / det;
    result[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) / det;
    result[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) / det;
    result[1][2] = (mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]) / det;
    result[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) / det;
    result[2][1] = (mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]) / det;
    result[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) / det;

    return 0;
}


//  J^T * J
void JTJ_3D(double J[][3], int n, double JTJ[3][3])
{
    for (int i = 0; i < n; i++) {
        JTJ[0][0] += J[i][0] * J[i][0];
        JTJ[0][1] += J[i][0] * J[i][1];
        JTJ[0][2] += J[i][0] * J[i][2];
        JTJ[1][0] += J[i][1] * J[i][0];
        JTJ[1][1] += J[i][1] * J[i][1];
        JTJ[1][2] += J[i][1] * J[i][2];
        JTJ[2][0] += J[i][2] * J[i][0];
        JTJ[2][1] += J[i][2] * J[i][1];
        JTJ[2][2] += J[i][2] * J[i][2];
    }
}



DOPValues Calculate_DOP::ToA_gdop_3D(std::vector<Anchor>& anchors, int n, Anchor tag) {
    DOPValues res;
    res.GDOP = -1;
    res.HDOP = -1;
    res.VDOP = -1;
    if (n < 4) {
        return res;
    }

    double J[n][3];

    // Calculate of Jakobian
    for (int i = 0; i < n; i++) {
        double d = distance_3D(anchors[i], tag);
        if (d == 0) {
            printf("tag and anchor have same coordinates\n");
            return res;
        }
        J[i][0] = (tag.x - anchors[i].x) / d;
        J[i][1] = (tag.y - anchors[i].y) / d;
        J[i][2] = (tag.z - anchors[i].z) / d;
    }

    //  J^T * J
    double JTJ[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    JTJ_3D(J,n,JTJ);

    //  J^T * J
    double inv_JTJ[3][3];
    if (invert_matrix_3x3(JTJ, inv_JTJ) == -1)
        return res;

    res.GDOP = sqrt(inv_JTJ[0][0] + inv_JTJ[1][1] + inv_JTJ[2][2]);
    res.HDOP = sqrt(inv_JTJ[0][0] + inv_JTJ[1][1]);
    res.VDOP = sqrt(inv_JTJ[2][2]);

    return res;
}


DOPValues Calculate_DOP::TDoA_gdop_3D_pair (std::vector<Anchor>& anchors, std::vector<IDComb>& comb, int n, Anchor tag)
{
    DOPValues res;
    res.GDOP = -1;
    res.HDOP = -1;
    res.VDOP = -1;

    if (n < 4) {       
        return res;
    }

    double J[n][3];


    // Calculate of Jakobian
    for (int i = 0; i < n; i++) {
        Anchor ref_anchor = anchors[comb[i].firstID];
        Anchor sec_anchor = anchors[comb[i].secondID];

        double d = distance_3D(sec_anchor, tag);
        double ref_distance = distance_3D(ref_anchor,tag);
        if (d == 0) {
            printf("tag and anchor have same coordinates\n");
            return res;
        }
        J[i][0] = (tag.x - sec_anchor.x) / d - (tag.x - ref_anchor.x) /ref_distance;
        J[i][1] = (tag.y - sec_anchor.y) / d - (tag.y - ref_anchor.y) /ref_distance;
        J[i][2] = (tag.z - sec_anchor.z) / d - (tag.z - ref_anchor.z) /ref_distance;
    }


    double JTJ[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    JTJ_3D(J,n,JTJ);


    //  J^T * J
    double inv_JTJ[3][3];
    if(invert_matrix_3x3(JTJ, inv_JTJ) == -1)
        return res;
    //DOP values
    res.GDOP = sqrt(inv_JTJ[0][0] + inv_JTJ[1][1] + inv_JTJ[2][2]);
    res.HDOP = sqrt(inv_JTJ[0][0] + inv_JTJ[1][1]);
    res.VDOP = sqrt(inv_JTJ[2][2]);
    return res;
}




