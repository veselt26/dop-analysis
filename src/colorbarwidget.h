#ifndef COLORBARWIDGET_H
#define COLORBARWIDGET_H

#include <QWidget>

class ColorBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBarWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // COLORBARWIDGET_H
