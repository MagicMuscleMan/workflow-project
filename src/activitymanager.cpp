#include "activitymanager.h"

#include <QDir>
#include <QDebug>


#include <KIconDialog>
#include <KIcon>
#include <KWindowSystem>
#include <KConfigGroup>
#include <KConfig>
#include <KMessageBox>
#include <KStandardDirs>


#include <KActivities/Controller>
#include <KActivities/Info>


ActivityManager::ActivityManager(QObject *parent) :
    QObject(parent)
{
    m_activitiesCtrl = new KActivities::Controller(this);
}

ActivityManager::~ActivityManager()
{

    delete m_activitiesCtrl;
}

void ActivityManager::setQMlObject(QObject *obj)
{
    qmlActEngine = obj;    

    connect(this, SIGNAL(activityAddedIn(QVariant,QVariant,QVariant,QVariant,QVariant)),
            qmlActEngine,SLOT(activityAddedIn(QVariant,QVariant,QVariant,QVariant,QVariant)));

    connect(this, SIGNAL(activityUpdatedIn(QVariant,QVariant,QVariant,QVariant,QVariant)),
            qmlActEngine,SLOT(activityUpdatedIn(QVariant,QVariant,QVariant,QVariant,QVariant)));


    QStringList activities = m_activitiesCtrl->listActivities();

    foreach (const QString &id, activities) {
        activityAdded(id);
    }

    connect(m_activitiesCtrl, SIGNAL(activityAdded(QString)), this, SLOT(activityAdded(QString)));
    connect(m_activitiesCtrl, SIGNAL(activityRemoved(QString)), this, SLOT(activityRemoved(QString)));
    connect(m_activitiesCtrl, SIGNAL(currentActivityChanged(QString)), this, SLOT(currentActivityChanged(QString)));
}

QString ActivityManager::getWallpaperFromFile(QString source, QString file) const
{
    //QString fpath = QDir::home().filePath(file);
    QString fpath = file;

    KConfig config( fpath, KConfig::SimpleConfig );
    KConfigGroup conGps = config.group("Containments");

    int iterat = 0;
    bool found = false;

    while((iterat<conGps.groupList().size() && (!found))){
        QString gps = conGps.groupList().at(iterat);
        KConfigGroup tempG = conGps.group(gps);

        if(tempG.readPathEntry("activityId",QString("null")) == source){

       //     qDebug()<<"Found:"<<gps<<"-"<<tempG.readPathEntry("activityId",QString("null"));
            KConfigGroup gWall = tempG.group("Wallpaper").group("image");

            found = true;
            QString foundF = gWall.readPathEntry("wallpaper",QString("null"));
            QDir tmD(foundF+"/contents/images");
            if (tmD.exists()){
                QStringList files;
                files = tmD.entryList(QDir::Files | QDir::NoSymLinks);

                if (!files.isEmpty())
                    foundF = tmD.absoluteFilePath(files.at(0));

      //          qDebug()<<files.at(0);
            }

            if (QFile::exists(foundF))
                return foundF;
            else
                return "";
        }


        iterat++;
    }
    return "";

}

QString ActivityManager::getWallpaper(QString source) const
{
    QString res = getWallpaperForStopped(source);
    if (res=="")
        res = getWallpaperForRunning(source);

    return res;
}

QString  ActivityManager::getWallpaperForRunning(QString source) const
{
    QString fPath =KStandardDirs::locate("config","plasma-desktop-appletsrc");

    //QString(".kde4/share/config/plasma-desktop-appletsrc")
    return getWallpaperFromFile(source,fPath);
}

QString  ActivityManager::getWallpaperForStopped(QString source) const
{
    QString fPath = kStdDrs.localkdedir()+"share/apps/plasma-desktop/activities/"+source;

    //QString actPath(".kde4/share/apps/plasma-desktop/activities/"+source);
    return getWallpaperFromFile(source,fPath);
}

QPixmap ActivityManager::disabledPixmapForIcon(const QString &ic)
{
    KIcon icon3(ic);
    return icon3.pixmap(KIconLoader::SizeHuge, QIcon::Disabled);
}


void ActivityManager::activityAdded(QString id) {

    KActivities::Info *activity = new KActivities::Info(id, this);


    QString state;
    switch (activity->state()) {
        case KActivities::Info::Running:
            state = "Running";
            break;
        case KActivities::Info::Starting:
            state = "Starting";
            break;
        case KActivities::Info::Stopping:
            state = "Stopping";
            break;
        case KActivities::Info::Stopped:
            state = "Stopped";
            break;
        case KActivities::Info::Invalid:
        default:
            state = "Invalid";
    }

    emit activityAddedIn(QVariant(id),
                         QVariant(activity->name()),
                         QVariant(activity->icon()),
                         QVariant(state),
                         QVariant(m_activitiesCtrl->currentActivity() == id));

    connect(activity, SIGNAL(infoChanged()), this, SLOT(activityDataChanged()));
    connect(activity, SIGNAL(stateChanged(KActivities::Info::State)), this, SLOT(activityStateChanged()));

}

void ActivityManager::activityRemoved(QString id) {

    QMetaObject::invokeMethod(qmlActEngine, "activityRemovedIn",
                              Q_ARG(QVariant, id));

}

void ActivityManager::activityDataChanged()
{
    KActivities::Info *activity = qobject_cast<KActivities::Info*>(sender());
    if (!activity) {
        return;
    }

    QString state;
    switch (activity->state()) {
        case KActivities::Info::Running:
            state = "Running";
            break;
        case KActivities::Info::Starting:
            state = "Starting";
            break;
        case KActivities::Info::Stopping:
            state = "Stopping";
            break;
        case KActivities::Info::Stopped:
            state = "Stopped";
            break;
        case KActivities::Info::Invalid:
        default:
            state = "Invalid";
    }



    emit activityUpdatedIn(QVariant(activity->id()),
                           QVariant(activity->name()),
                           QVariant(activity->icon()),
                           QVariant(state),
                           QVariant(m_activitiesCtrl->currentActivity() == activity->id()));
}

void ActivityManager::activityStateChanged()
{
    KActivities::Info *activity = qobject_cast<KActivities::Info*>(sender());
    const QString id = activity->id();
    if (!activity) {
        return;
    }
    QString state;
    switch (activity->state()) {
        case KActivities::Info::Running:
            state = "Running";
            break;
        case KActivities::Info::Starting:
            state = "Starting";
            break;
        case KActivities::Info::Stopping:
            state = "Stopping";
            break;
        case KActivities::Info::Stopped:
            state = "Stopped";
            break;
        case KActivities::Info::Invalid:
        default:
            state = "Invalid";
    }

    //qDebug() <<activity->id()<< "-" << state;

    QMetaObject::invokeMethod(qmlActEngine, "setCState",
                              Q_ARG(QVariant, id),
                              Q_ARG(QVariant, state));

    if((activity->id()==activityForDelete)&&
            (state=="Stopped")){
        m_activitiesCtrl->removeActivity(activity->id());
        activityForDelete = "";
    }
}

void ActivityManager::currentActivityChanged(const QString &id)
{
    QMetaObject::invokeMethod(qmlActEngine, "setCurrentSignal",
                              Q_ARG(QVariant, id));
}



/////////SLOTS

void ActivityManager::setIcon(QString id, QString name)
{
    m_activitiesCtrl->setActivityIcon(id,name);
}

QString ActivityManager::chooseIcon(QString id)
{
    KIconDialog *dialog = new KIconDialog;
    dialog->setup(KIconLoader::Desktop);
    dialog->setProperty("DoNotCloseController", true);
    KWindowSystem::setOnDesktop(dialog->winId(), KWindowSystem::currentDesktop());
    dialog->showDialog();
    KWindowSystem::forceActiveWindow(dialog->winId());
    QString icon = dialog->openDialog();
    dialog->deleteLater();

    if (icon != "")
        setIcon(id,icon);

    return icon;
}



QString ActivityManager::add(QString name) {
   return m_activitiesCtrl->addActivity(name);
}

void ActivityManager::clone(QString id, QString name) {

}

void ActivityManager::setCurrent(QString id) {
    m_activitiesCtrl->setCurrentActivity(id);
}

void ActivityManager::stop(QString id) {
    m_activitiesCtrl->stopActivity(id);
}

void ActivityManager::start(QString id) {

    m_activitiesCtrl->startActivity(id);
}

void ActivityManager::setName(QString id, QString name) {
    m_activitiesCtrl->setActivityName(id,name);
}

void ActivityManager::remove(QString id) {
    m_activitiesCtrl->stopActivity(id);
    activityForDelete = id;
}

/*

 int ActivityManager::askForDelete(QString activityName)
{
    QString question("Do you yeally want to delete activity ");
    question.append(activityName);
    question.append(" ?");
    int responce =  KMessageBox::questionYesNo(0,question,"Delete Activity");
    return responce;
}


*/
#include "activitymanager.moc"