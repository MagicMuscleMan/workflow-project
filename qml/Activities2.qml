// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.qtextracomponents 0.1


import "delegates"
import "instances"

import "ui"

import "DynamicAnimations.js" as DynamAnim

Rectangle {
    id:mainView
    width: 1024;  height: 700

    color: "#dcdcdc"

    //  z:0
    clip:true
    anchors.fill: parent

    property int currentColumn:-1
    property int currentRow:-1

    property int scaleMeter:zoomSlider.value

    property real zoomingHeightFactor: ((zoomSlider.value-zoomSlider.minimum)/(zoomSlider.maximum-zoomSlider.minimum))*0.6
    property int workareaHeight:(3.6 - zoomingHeightFactor)*scaleMeter
    property int workareaY:2*scaleMeter

    property int workareaWidth: 70+(2.8*mainView.scaleMeter) + (mainView.scaleMeter-5)/3;

    property bool showWinds: true
    property bool lockActivities: false

    property int currentDesktop: 2

    Behavior on scaleMeter{
        NumberAnimation {
            duration: 100;
            easing.type: Easing.InOutQuad;
        }
    }

    SharedActivitiesList{
        id:instanceOfActivitiesList
        objectName: "instActivitiesEngine"
    }

    SharedWorkareasList{
        id:instanceOfWorkAreasList
    }

    SharedTasksList{
        id:instanceOfTasksList
        objectName: "instTasksEngine"
    }

    Item{
        id:centralArea
        x: 0
        y:0
        width:mainView.width
        height:mainView.height

        property string typeId: "centralArea"


        WorkAreasAllLists{
            id: allWorkareas
            z:4
        }


        StoppedActivitiesPanel{
            id:stoppedPanel
            z:4
        }


        MainAddActivityButton{
            id: mAddActivityBtn
            z:4
        }

        TitleMainView{
            id:oxygenT
            z:4
        }


        AllActivitiesTasks{
            id:allActT
            z:4
        }


        Slider {
            id:zoomSlider
            y:mainView.height - height - 5
            x:stoppedPanel.x - width - 5
            maximum: 65
            minimum: 35
            value: 50
            width:125
            z:10

            Image{
                x:-0.4*width
                y:-0.3*height
                width:30
                height:1.5*width
                source:"Images/buttons/magnifyingglass.png"
            }

        }

        WorkAreaFull{
            id:wkFull
            z:11
        }

    }

    DraggingInterface{
        id:mDragInt
    }


    Component.onCompleted: DynamAnim.createComponents();

    function getDynLib(){
        return DynamAnim;
    }


    /*--------------------Dialogs ---------------- */
    Rectangle{
        id:removeDialog
        //     visualParent:mainView

        anchors.centerIn: mainView
        property string activityCode
        property string activityName

        property real defOpacity:0.5
        color:"#d5333333"
        border.color: "#aaaaaa"
        border.width: 2
        radius:4

        visible:false

        width:mainTextInf.width+100
        height:infIcon.height+90

        //Title
        Text{
            id:titleMesg
            color:"#ffffff"
            text: i18n("Remove Activity")+"..."
            width:parent.width
            horizontalAlignment:Text.AlignHCenter
            anchors.top:parent.top
            anchors.topMargin: 5
        }

        Rectangle{
            anchors.top:titleMesg.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width:0.93*parent.width
            color:"#ffffff"
            opacity:0.3
            height:1
            /*                gradient: Gradient {
                GradientStop {position: 0; color: "#00ffffff"}
                GradientStop {position: 0.5; color: "#ffffffff"}
                GradientStop {position: 1; color: "#00ffffff"}
            }*/
        }

        //Main Area
        QIconItem{
            id:infIcon
            anchors.top:titleMesg.bottom
            anchors.topMargin:10
            icon:QIcon("messagebox_info")
            width:70
            height:70
        }
        Text{
            id:mainTextInf
            anchors.left: infIcon.right
            anchors.verticalCenter: infIcon.verticalCenter
            color:"#ffffff"
            text:"Are you sure you want to remove activity <b>"+removeDialog.activityName+"</b> ?"
        }

        //Buttons

        Item{
            anchors.top: infIcon.bottom
            anchors.topMargin:10
            anchors.right: parent.right
            anchors.rightMargin: 10
            height:30
            width:parent.width
            PlasmaComponents.Button{
                id:button1
                anchors.right: button2.left
                anchors.rightMargin: 10
                anchors.bottom: parent.bottom
                width:100
                text:i18n("Yes")
                iconSource:"dialog-apply"

                onClicked:{
                    activityManager.remove(removeDialog.activityCode);
                    instanceOfActivitiesList.activityRemovedIn(removeDialog.activityCode);
                    removeDialog.close();
                }
            }
            PlasmaComponents.Button{
                id:button2
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width:100
                text:i18n("No")
                iconSource:"editdelete"

                onClicked:{
                    removeDialog.close();
                }
            }
        }
        function open(){
            removeDialog.visible = true;
        }
        function close(){
            removeDialog.visible = false;
        }

    }
}



