#ifndef COCKPITPAGE_H
#define COCKPITPAGE_H

#include <QWidget>

class cockpitPage : public QWidget
{
    Q_OBJECT
public:
    explicit cockpitPage(QWidget *parent = nullptr);
    //virtual ~cockpitPage();

private:
    void setupUI();
};

#endif // COCKPITPAGE_H
