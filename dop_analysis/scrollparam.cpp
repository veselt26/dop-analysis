#include "scrollparam.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>

ScrollParam::ScrollParam(QWidget * parent) :
    QDialog(parent)
{

    samples = new QLineEdit(this);
    samples->setValidator(new QIntValidator(this));
    QLabel *samplesLabel = new QLabel("Number of samples:", this);


    max_z = new QLineEdit(this);
    max_z->setValidator(new QIntValidator(this));
    QLabel *max_zLabel = new QLabel("Maximum Z:", this);

    min_z = new QLineEdit(this);
    min_z->setValidator(new QIntValidator(this));
    QLabel *min_zLabel = new QLabel("Minimum Z", this);


    ok_btn = new QPushButton("OK", this);
    cancel_btn = new QPushButton("Cancel", this);


    QHBoxLayout *button_lay = new QHBoxLayout;
    button_lay->addWidget(ok_btn);
    button_lay->addWidget(cancel_btn);


    QVBoxLayout *main_lay = new QVBoxLayout;
    main_lay->addWidget(max_zLabel);
    main_lay->addWidget(max_z);
    main_lay->addWidget(min_zLabel);
    main_lay->addWidget(min_z);
    main_lay->addWidget(samplesLabel);
    main_lay->addWidget(samples);
    main_lay->addLayout(button_lay);

    setLayout(main_lay);
    setWindowTitle("Enter information to sampling");

    connect(ok_btn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel_btn, &QPushButton::clicked, this, &QDialog::reject);
}

int ScrollParam::get_samples() const {
    return samples->text().toInt();
}

double ScrollParam::get_max_z() const {
    return max_z->text().toDouble();
}

double ScrollParam::get_min_z() const {
    return min_z->text().toDouble();
}

