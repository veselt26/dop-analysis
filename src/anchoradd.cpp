#include "anchoradd.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>

AnchorAdd::AnchorAdd(QWidget *parent)
    : QDialog(parent)
{
    cor_x = new QLineEdit(this);
    cor_x->setValidator(new QIntValidator(this));
    QLabel *firstLabel = new QLabel("X:", this);


    cor_y = new QLineEdit(this);
    cor_y->setValidator(new QIntValidator(this));
    QLabel *secondLabel = new QLabel("Y:", this);

    cor_z = new QLineEdit(this);
    cor_z->setValidator(new QIntValidator(this));
    QLabel *thirdLabel = new QLabel("Z:", this);


    ok_btn = new QPushButton("OK", this);
    cancel_btn = new QPushButton("Cancel", this);

    QHBoxLayout *button_lay = new QHBoxLayout;
    button_lay->addWidget(ok_btn);
    button_lay->addWidget(cancel_btn);

    QVBoxLayout *main_lay = new QVBoxLayout;
    main_lay->addWidget(firstLabel);
    main_lay->addWidget(cor_x);
    main_lay->addWidget(secondLabel);
    main_lay->addWidget(cor_y);
    main_lay->addWidget(thirdLabel);
    main_lay->addWidget(cor_z);
    main_lay->addLayout(button_lay);

    setLayout(main_lay);
    setWindowTitle("Enter coordinates for anchor");

    connect(ok_btn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel_btn, &QPushButton::clicked, this, &QDialog::reject);
}

double AnchorAdd::get_x() const {
    return cor_x->text().toDouble();
}

double AnchorAdd::get_y() const {
    return cor_y->text().toDouble();
}

double AnchorAdd::get_z() const {
    return cor_z->text().toDouble();
}
