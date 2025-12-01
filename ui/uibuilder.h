#ifndef UIBUILDER_H
#define UIBUILDER_H

#include <QLabel>
#include <QPushButton>


class UIBuilder
{
public:
    UIBuilder();

    QPushButton* createButton(const QString &buttonName, QWidget *parent);
    QLabel* createCopyright(const QString &copyRightStr, QWidget *parent);
    QLabel *createPageTitle(const QString &titleStr, QWidget *parent);


private:

    // ==========================================
    // Member Variables
    // ==========================================
    struct STDButtonLayout
    {
        static const QString styleSheet;

        static const int buttonwidth     = 300;
        static const int buttonHeight    = 50;
    };


};

#endif // UIBUILDER_H
