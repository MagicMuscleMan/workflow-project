#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <QObject>



#include <KStandardDirs>
#include <KActivities/Controller>
#include <KConfigGroup>

#include <QTimer>

namespace KActivities
{
    class Controller;
    class Info;
}

namespace Plasma {
    class Containment;
    class Corona;
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

    Q_INVOKABLE QString getWallpaper(QString source);
    Q_INVOKABLE QPixmap disabledPixmapForIcon(const QString &ic);
    Q_INVOKABLE QString add(QString name);
//    Q_INVOKABLE void clone(QString id, QString name);
    Q_INVOKABLE void setCurrent(QString id);
    Q_INVOKABLE void stop(QString id);
    Q_INVOKABLE void start(QString id);
    Q_INVOKABLE void setName(QString id, QString name);
    Q_INVOKABLE void remove(QString id);
    Q_INVOKABLE QString chooseIcon(QString);
    Q_INVOKABLE void setIcon(QString id, QString name);

    //are used in cloning
    Q_INVOKABLE void initCloningPhase02(QString id);
    Q_INVOKABLE void initCloningPhase04(QString id);
//    Q_INVOKABLE int askForDelete(QString activityName);

    //Interact with Corona() and Desktops Containments()

    Q_INVOKABLE void unlockWidgets();
    Q_INVOKABLE void showWidgetsExplorer(QString);

    void setQMlObject(QObject *,Plasma::Corona *);

signals:
    void activityAddedIn(QVariant id, QVariant title, QVariant icon, QVariant stat, QVariant cur);
    void activityUpdatedIn(QVariant id, QVariant title, QVariant icon, QVariant stat, QVariant cur);
    void showedIconDialog();
    void answeredIconDialog();

public slots:
 // void dataUpdated(QString source, Plasma::DataEngine::Data data);
  void activityAdded(QString id);
  void activityRemoved(QString id);
  void activityDataChanged();
  void activityStateChanged();
  void currentActivityChanged(const QString &id);

private slots:
  void timerTrigerred();

private:
    QString getWallpaperForRunning(QString source);
    QString getWallpaperForStopped(QString source);
    QString getWallpaperFromFile(QString source,QString file);
    QString getWallpaperFromContainment(Plasma::Containment *actContainment);
    QString getWallpaperForSingleImage(KConfigGroup &);

    QString getContainmentId(QString txt) const;

    int loadCloneActivitySettings();
    int storeCloneActivitySettings();

    QObject *qmlActEngine;

    KActivities::Controller *m_activitiesCtrl;
    QString activityForDelete;

    KStandardDirs kStdDrs;

    QString fromCloneActivityText;
    QString fromCloneActivityId;
    QString fromCloneContainmentId;

    QString toCloneContainmentId;
    QString toCloneActivityId;

    Plasma::Corona *m_corona;

    int m_timerPhase;

    QTimer *m_timer;

    //This is an indicator for the corona() actions in order to check
    //if widgets are already unlocked.
    QString m_unlockWidgetsText;


    ////////////
    Plasma::Containment *getContainment(QString actId);
};

#endif // ACTIVITYMANAGER_H
