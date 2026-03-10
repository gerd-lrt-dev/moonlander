#include "configmanager.h"

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{

}

bool ConfigManager::loadConfig(const QString& path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    rawJson = QString::fromUtf8(file.readAll());

    QJsonDocument doc = QJsonDocument::fromJson(rawJson.toUtf8());

    if (!doc.isObject())
        return false;

    spacecraftList.clear();

    QJsonArray arr = doc.object()["spacecraft"].toArray();

    for (const QJsonValue &val : arr)
    {
        spacecraftList.append(val.toObject());
    }
    qDebug() << "Json loaded signal is going to emit";
    emit jsonLoaded();
    return true;
}

int ConfigManager::spacecraftCount() const
{
    return spacecraftList.size();
}

QString ConfigManager::spacecraftName(int index) const
{
    if(index < 0 || index >= spacecraftList.size())
        return "Invalid spacecraft";

    return spacecraftList[index]["name"].toString("Unnamed spacecraft");
}

QJsonObject ConfigManager::spacecraftObject(int index) const
{
    if(index < 0 || index >= spacecraftList.size())
        return {};

    return spacecraftList[index];
}

QString ConfigManager::spacecraftJson(int index) const
{
    if(index < 0 || index >= spacecraftList.size())
        return {};

    QJsonDocument doc(spacecraftList[index]);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

QString ConfigManager::defaultSpacecraftJson() const
{
    if(spacecraftList.isEmpty())
        return {};

    QJsonDocument doc(spacecraftList[0]);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
