/***************************************************************************
 *   Copyright (C) 2011 by Martin Gräßlin <mgraesslin@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "workflow.h"

#include <KDebug>
#include <KGlobalSettings>


#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>

#include <Plasma/Extender>
#include <Plasma/ExtenderItem>
#include <Plasma/ToolTipManager>
#include <Plasma/Corona>
#include <Plasma/Containment>
#include <Plasma/Wallpaper>

#include <QDir>



WorkFlow::WorkFlow(QObject *parent, const QVariantList &args):
    Plasma::PopupApplet(parent, args),
    m_mainWidget(0),
    actManager(0)
{
    setPopupIcon("preferences-activities");
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPassivePopup(true);

    actManager = new ActivityManager(this);
    taskManager = new PTaskManager(this);


    //qDebug() << fpath << "-" << KGlobalSettings::self()->desktopPath();

   // qDebug() << containment()->activity();
}

WorkFlow::~WorkFlow()
{
    delete actManager;
    delete taskManager;
}

void WorkFlow::init(){

    Plasma::ToolTipManager::self()->registerWidget(this);
    extender()->setEmptyExtenderMessage(i18n("No Activities..."));
    // don't grow too much height
    //extender()->setMaximumHeight(300);
    if (extender()->item("WorkFlow") == 0) {
      // create the item
      Plasma::ExtenderItem *item = new Plasma::ExtenderItem(extender());
      // initialize the item
      initExtenderItem(item);
      // set item name and title
      item->setName("WorkFlow");
      item->setTitle("WorkFlow");
    }
    // connect data sources
}

void WorkFlow::initExtenderItem(Plasma::ExtenderItem *item) {

    m_mainWidget = new QGraphicsWidget(this);
    m_mainWidget->setPreferredSize(550, 300);
    //m_mainWidget->setMinimumSize(50,50);

    mainLayout = new QGraphicsLinearLayout(m_mainWidget);
    mainLayout->setOrientation(Qt::Vertical);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    KStandardDirs *sd =    KGlobal::dirs();

    QString path =  sd->findResource("data","plasma-workflowplasmoid/qml/Activities2.qml");

    declarativeWidget = new Plasma::DeclarativeWidget();
  //  declarativeWidget->setInitializationDelayed(true);
    declarativeWidget->setQmlPath(path);

    mainLayout->addItem(declarativeWidget);
    m_mainWidget->setLayout(mainLayout);

    if (declarativeWidget->engine()) {
        QDeclarativeContext *ctxt = declarativeWidget->engine()->rootContext();
        if (ctxt) {
            ctxt->setContextProperty("activityManager", actManager);
            ctxt->setContextProperty("taskManager", taskManager);

            QObject *rootObject = dynamic_cast<QObject *>(declarativeWidget->rootObject());
            QObject *qmlActEng = rootObject->findChild<QObject*>("instActivitiesEngine");
            QObject *qmlTaskEng = rootObject->findChild<QObject*>("instTasksEngine");


            if(!rootObject)
                qDebug() << "root was not found...";
            else{
                if(qmlActEng)
                    actManager->setQMlObject(qmlActEng, dataEngine("org.kde.activities"));
                if(qmlTaskEng)
                    taskManager->setQMlObject(qmlTaskEng, dataEngine("tasks"));
            }
        }
    }

    //the activitymanager class will be directly accessible from qml

    item->setWidget(m_mainWidget);
    resize(1000,700);
}

////INVOKES
   
#include "workflow.moc"
   