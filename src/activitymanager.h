#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <QObject>

#include <KStandardDirs>

#include <KActivities/Controller>

namespace KActivities
{
    class Controller;
    class Info;
}

class ActivityManager : public QObject
{
    Q_OBJECT
public:
    explicit ActivityManager(QObject *parent = 0);
    ~ActivityManager();

//  Q_INVOKABLE void cloneCurrentActivity();
//  Q_INVOKABLE void createActivity(const QString &pluginName);
//  Q_INVOKABLE void createActivityFromScript(const QString &script, const QString &name, const QString &icon, const QStringList &startupApps);
//  Q_INVOKABLE void downloadActivityScripts();

    Q_INVOKABLE QString getWallpaper(QString source) const;
    Q_INVOKABLE QString chooseIcon(QString);
    Q_INVOKABLE QPixmap disabledPixmapForIcon(const QString &ic);
    Q_INVOKABLE QString add(QString name);
    Q_INVOKABLE void clone(QString id, QString name);
    Q_INVOKABLE void setCurrent(QString id);
    Q_INVOKABLE void stop(QString id);
    Q_INVOKABLE void start(QString id);
    Q_INVOKABLE void setName(QString id, QString name);
    Q_INVOKABLE void remove(QString id);
//    Q_INVOKABLE int askForDelete(QString activityName);

    void setQMlObject(QObject *obj);

signals:
    void activityAddedIn(QVariant id, QVariant title, QVariant icon, QVariant stat, QVariant cur);
    void activityUpdatedIn(QVariant id, QVariant title, QVariant icon, QVariant stat, QVariant cur);


public slots:
 // void dataUpdated(QString source, Plasma::DataEngine::Data data);
  void activityAdded(QString id);
  void activityRemoved(QString id);
  void activityDataChanged();
  void activityStateChanged();
  void currentActivityChanged(const QString &id);

private:
    void setIcon(QString id, QString name);
    QString getWallpaperForRunning(QString source) const;
    QString getWallpaperForStopped(QString source) const;
    QString getWallpaperFromFile(QString source,QString file) const;


    QObject *qmlActEngine;
    KActivities::Controller *m_activitiesCtrl;
    QString activityForDelete;

    KStandardDirs kStdDrs;

};

#endif // ACTIVITYMANAGER_H