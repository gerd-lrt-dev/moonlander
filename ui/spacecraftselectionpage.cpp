#include "spacecraftselectionpage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QJsonDocument>

#include "uibuilder.h"

SpacecraftSelectionPage::SpacecraftSelectionPage(ConfigManager& configManager, QWidget *parent)
    : QWidget(parent),
    configManager_(configManager)
{
    setupUI();
    setupConnections();
}

void SpacecraftSelectionPage::setupConnections()
{
    connect(selectButton, &QPushButton::clicked,
            this, &SpacecraftSelectionPage::onSelectClicked);

    connect(backButton, &QPushButton::clicked,
            this, &SpacecraftSelectionPage::backRequested);

    connect(listWidget, &QListWidget::currentRowChanged,
            this, &SpacecraftSelectionPage::onRowChanged);

    connect(&configManager_, &ConfigManager::jsonLoaded,
            this, &SpacecraftSelectionPage::receiveJsonConfigStr);
}

void SpacecraftSelectionPage::setupUI()
{
    UIBuilder uiBuilder_;

    QVBoxLayout *mainlayout = new QVBoxLayout(this);

    QLabel *title = uiBuilder_.createPageTitle("Select Spacecraft", this);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    listWidget = new QListWidget(this);

    QWidget *detailsWidget = new QWidget(this);
    QVBoxLayout *detailsLayout = new QVBoxLayout(detailsWidget);

    nameLabel = new QLabel("No spacecraft selected", this);
    nameLabel->setAlignment(Qt::AlignCenter);

    detailsView = new QTextEdit(this);
    detailsView->setReadOnly(true);

    detailsLayout->addWidget(nameLabel);
    detailsLayout->addWidget(detailsView);

    contentLayout->addWidget(listWidget,1);
    contentLayout->addWidget(detailsWidget,2);

    selectButton = uiBuilder_.createButton("Select", this);
    backButton   = uiBuilder_.createButton("Back", this);

    mainlayout->addWidget(title);
    mainlayout->addLayout(contentLayout);
    mainlayout->addWidget(selectButton, 0, Qt::AlignHCenter);
    mainlayout->addWidget(backButton, 0, Qt::AlignHCenter);
}

void SpacecraftSelectionPage::populateList()
{
    listWidget->clear();

    int count = configManager_.spacecraftCount();

    qDebug() << "[SpacecraftSelectionPage] spacecraft count:" << count;

    for(int i = 0; i < count; i++)
    {
        qDebug() << "Added Item: " << configManager_.spacecraftName(i);
        listWidget->addItem(configManager_.spacecraftName(i));
    }

    if(count > 0)
        listWidget->setCurrentRow(0);
}

void SpacecraftSelectionPage::onRowChanged(int row)
{
    if(row < 0 || row >= configManager_.spacecraftCount())
        return;

    QJsonObject sc = configManager_.spacecraftObject(row);

    nameLabel->setText(sc["name"].toString());

    QJsonDocument doc(sc);
    detailsView->setText(doc.toJson(QJsonDocument::Indented));
}

void SpacecraftSelectionPage::onSelectClicked()
{
    int row = listWidget->currentRow();

    if(row < 0 || row >= configManager_.spacecraftCount())
        return;

    QString spacecraftJson = configManager_.spacecraftJson(row);

    qDebug() << "[SpacecraftSelectionPage] Selected spacecraft JSON:" << spacecraftJson;

    emit spacecraftSelected(spacecraftJson);
}

void SpacecraftSelectionPage::receiveJsonConfigStr()
{
    qDebug() << "receiveJsonConfigStr wird aufgerufen";
    populateList();
}
