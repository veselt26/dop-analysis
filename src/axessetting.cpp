#include "axessetting.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>

AxesSetting::AxesSetting(QWidget *parent)
    : QDialog(parent)
{

    min_x = new QLineEdit(this);
    min_x->setValidator(new QIntValidator(this));
    QLabel *firstLabel = new QLabel("Min range for X:", this);


    min_y = new QLineEdit(this);
    min_y->setValidator(new QIntValidator(this));
    QLabel *secondLabel = new QLabel("Min range for Y:", this);


    max_x = new QLineEdit(this);
    max_x->setValidator(new QIntValidator(this));
    QLabel *thirdLabel = new QLabel("Max range for X:", this);


    max_y = new QLineEdit(this);
    max_y->setValidator(new QIntValidator(this));
    QLabel *fourthLabel = new QLabel("Max range for Y:", this);


    ok_btn = new QPushButton("OK", this);
    cancel_btn = new QPushButton("Cancel", this);

    QHBoxLayout *button_lay = new QHBoxLayout;
    button_lay->addWidget(ok_btn);
    button_lay->addWidget(cancel_btn);

    QVBoxLayout *main_lay = new QVBoxLayout;
    main_lay->addWidget(firstLabel);
    main_lay->addWidget(min_x);
    main_lay->addWidget(secondLabel);
    main_lay->addWidget(min_y);
    main_lay->addWidget(thirdLabel);
    main_lay->addWidget(max_x);
    main_lay->addWidget(fourthLabel);
    main_lay->addWidget(max_y);
    main_lay->addLayout(button_lay);

    setLayout(main_lay);
    setWindowTitle("Enter new ranges for chart");

    connect(ok_btn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel_btn, &QPushButton::clicked, this, &QDialog::reject);
}

double AxesSetting::get_max_x() const {
    return max_x->text().toDouble();
}
double AxesSetting::get_min_x() const {
    return min_x->text().toDouble();
}
double AxesSetting::get_max_y() const {
    return max_y->text().toDouble();
}

double AxesSetting::get_min_y() const {
    return min_y->text().toDouble();
}

