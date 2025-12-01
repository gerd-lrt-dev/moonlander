#include "uibuilder.h"

UIBuilder::UIBuilder() {}

const QString UIBuilder::STDButtonLayout::styleSheet =
    "QPushButton#STDButton { "
        "background-color: #1B2A44; "
        "color: white; font-size: "
        "20px; font-weight: bold; "
        "padding: 12px 24px; "
        "border-radius: 12px; "
        "border: 2px solid #2F4A72; "
        "letter-spacing: 1px; }"
    "QPushButton#STDButton:hover { "
        "background-color: #24385A; "
        "border-color: #3B5C8A; }"
    "QPushButton#STDButton:pressed { "
        "background-color: #162133; "
        "border-color: #203252; }";

QPushButton* UIBuilder::createButton(const QString &buttonName, QWidget *parent)
{
    QPushButton *button = new QPushButton(buttonName, parent);
    button->setObjectName("STDButton");
    button->setFixedSize(STDButtonLayout::buttonwidth, STDButtonLayout::buttonHeight);

    button->setStyleSheet(STDButtonLayout::styleSheet);

    return button;
}

QLabel* UIBuilder::createCopyright(const QString &copyRightStr, QWidget *parent)
{
    QLabel *copyright = new QLabel(copyRightStr, parent);
    copyright->setAlignment(Qt::AlignCenter);
    copyright->setStyleSheet("color: #AFC2D9; font-size: 14px; font-weight: 500; letter-spacing: 1px;");

    return copyright;
}

QLabel* UIBuilder::createPageTitle(const QString &titleStr, QWidget *parent)
{
    QLabel *title = new QLabel(titleStr, parent);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 32px;"
                         "font-weight: bold;");

    return title;
}
