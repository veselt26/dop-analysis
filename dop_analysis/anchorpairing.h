#ifndef ANCHORPAIRING_H
#define ANCHORPAIRING_H


#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>


class AnchorPairing : public QDialog
{

    Q_OBJECT



    public:
    explicit AnchorPairing(QWidget *parent = nullptr);

    int getSelectedOption() const;

private:
    QRadioButton *option1;
    QRadioButton *option2;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;


};

#endif // ANCHORPAIRING_H
