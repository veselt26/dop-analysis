#ifndef SCROLLPARAM_H
#define SCROLLPARAM_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class ScrollParam : public QDialog
{
    Q_OBJECT


public:
    explicit ScrollParam(QWidget *parent = nullptr);
    int get_samples() const;
    double get_max_z() const;
    double get_min_z() const;



private:
    QLineEdit *samples;
    QLineEdit *max_z;
    QLineEdit *min_z;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
};

#endif // SCROLLPARAM_H
