#pragma once

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class ConfigManager : public QObject
{
    Q_OBJECT
public:

    explicit ConfigManager(QObject *parent = nullptr);

    bool loadConfig(const QString& path);

    int spacecraftCount() const;

    QString spacecraftName(int index) const;

    QJsonObject spacecraftObject(int index) const;

    QString spacecraftJson(int index) const;

    QString defaultSpacecraftJson() const;

signals:
    void jsonLoaded();

private:
    QString rawJson;
    QList<QJsonObject> spacecraftList;
};


