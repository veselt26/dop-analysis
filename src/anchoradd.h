#ifndef ANCHORADD_H
#define ANCHORADD_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>



class AnchorAdd : public QDialog
{
    Q_OBJECT


    public:
    explicit AnchorAdd(QWidget *parent = nullptr);
    double get_x() const;
    double get_y() const;
    double get_z() const;

    private:
    QLineEdit *cor_z;
    QLineEdit *cor_x;
    QLineEdit *cor_y;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
};

#endif // ANCHORADD_H
