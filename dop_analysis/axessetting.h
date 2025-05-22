#ifndef AXESSETTING_H
#define AXESSETTING_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>


class AxesSetting : public QDialog
{
    Q_OBJECT


public:
    explicit AxesSetting(QWidget *parent = nullptr);
    double get_min_x() const;
    double get_min_y() const;
    double get_max_x() const;
    double get_max_y() const;

private:
    QLineEdit *min_x;
    QLineEdit *min_y;
    QLineEdit *max_x;
    QLineEdit *max_y;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
};

#endif // AXESSETTING_H
