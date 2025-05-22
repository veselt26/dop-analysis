#include "anchorpairing.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>

AnchorPairing::AnchorPairing(QWidget *parent)
    : QDialog(parent)
{
    QLabel *label = new QLabel("Please select an option:", this);

    option1 = new QRadioButton("Pair by ID", this);
    option2 = new QRadioButton("Pair by pairs", this);

    ok_btn = new QPushButton("OK", this);
    cancel_btn = new QPushButton("Cancel", this);

    connect(ok_btn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel_btn, &QPushButton::clicked, this, &QDialog::reject);

    QHBoxLayout *button_lay = new QHBoxLayout;
    button_lay->addWidget(ok_btn);
    button_lay->addWidget(cancel_btn);

    QVBoxLayout *main_lay = new QVBoxLayout;
    main_lay->addWidget(label);
    main_lay->addWidget(option1);
    main_lay->addWidget(option2);
    main_lay->addLayout(button_lay);

    setLayout(main_lay);
    setWindowTitle("Choose Pairing");
}

int AnchorPairing::getSelectedOption() const {
    if (option1->isChecked())
    {

        return 1;
    }
    else if (option2->isChecked())
    {

        return 2;
    }
    return 0;
}
