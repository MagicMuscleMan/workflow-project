// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


Item {
    id: buttonsArea
    width: buttonsSize
    height: taskDeleg2.height

    state: "hide"

    property string status:"nothover"

    signal changedStatus();

    property alias opacityClose: closeBtn.opacity
    property alias yClose: closeBtn.y
    property alias opacityWSt:  placeStateBtn.opacity
    property alias yWSt:  placeStateBtn.y

    property int buttonsSize
    property int buttonsSpace: -buttonsSize/8


    CloseWindowButton{
        id:closeBtn

        width: parent.buttonsSize
        height: width
        y:0

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onEntered: {
                closeBtn.onEntered();
                buttonsArea.state = "show"
                buttonsArea.status = "hover"
                changedStatus();
            }

            onExited: {
                closeBtn.onExited();
                buttonsArea.state = "hide"
                buttonsArea.status = "nothover"
                changedStatus();
            }

            onReleased: {
                closeBtn.onReleased();
            }

            onPressed: {
                closeBtn.onPressed();
            }

            onClicked: {
                closeBtn.onClicked();
                instanceOfTasksList.removeTask(taskDeleg2.ccode);
                allActT.changedChildState();
            }


        }

    }

    WindowPlaceButton{
        id: placeStateBtn

        width: parent.buttonsSize
        height: width
        y:buttonsSize + buttonsSpace

        allDesks: onAllDesktops || 0 ? true : false
        allActiv: onAllActivities || 0 ? true : false

        function informState(){
            if (placeStateBtn.state === "one")
                instanceOfTasksList.setTaskState(taskDeleg2.ccode,"oneDesktop");
            else if (placeStateBtn.state === "allDesktops")
                instanceOfTasksList.setTaskState(taskDeleg2.ccode,"allDesktops");
            else if (placeStateBtn.state === "everywhere")
                instanceOfTasksList.setTaskState(taskDeleg2.ccode,"allActivities");
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            z:4

            onEntered: {
                placeStateBtn.onEntered();
                buttonsArea.state = "show";
                buttonsArea.status = "hover"
                changedStatus();
            }

            onExited: {
                placeStateBtn.onExited();
                buttonsArea.state = "hide";
                buttonsArea.status = "nothover"
                changedStatus();
            }

            onPressAndHold:{
                if (placeStateBtn.state === "allDesktops"){
                    if(taskDeleg2.centralListView === desktopDialog.getTasksList()){
                        instanceOfTasksList.setTaskDesktop(taskDeleg2.ccode,taskDeleg2.centralListView.desktopInd);
                        placeStateBtn.previousState();
                        placeStateBtn.informState();
                    }
                }

            }

            onClicked: {
                //Animation must start before changing state
                if (placeStateBtn.state === "allDesktops"){
                    if(mainView.animationsStep2!==0){
                        var x3 = imageTask2.x;
                        var y3 = imageTask2.y;

                        mainView.getDynLib().animateDesktopToEverywhere(code,imageTask2.mapToItem(mainView,x3, y3),1);
                    }
                }

                placeStateBtn.onClicked();
                placeStateBtn.nextState();
                placeStateBtn.informState();

                if (placeStateBtn.state !== "everywhere"){
                    if(mainView.animationsStep2!==0){
                        var x1 = imageTask2.x;
                        var y1 = imageTask2.y;

                        mainView.getDynLib().animateEverywhereToActivity(code,imageTask2.mapToItem(mainView,x1, y1),1);
                    }
                }
            }

            onReleased: {
                placeStateBtn.onReleased();
            }

            onPressed: {
                placeStateBtn.onPressed();
            }


        }

    }

    states: [
        State {
            name: "show"

            PropertyChanges {
                target: buttonsArea

                opacityClose: 1
                opacityWSt: 1
            }
        },
        State {
            name: "hide"
            PropertyChanges {
                target: buttonsArea

                opacityClose: 0
                opacityWSt: 0
            }
        }
    ]

    transitions: [

        Transition {
            from:"hide"; to:"show"
            reversible: false

            SequentialAnimation{

                ParallelAnimation{
                    NumberAnimation {
                        target: buttonsArea;
                        property: "opacityClose";
                        duration: mainView.animationsStep;
                        easing.type: Easing.InOutQuad;

                    }

                }

                ParallelAnimation{
                    NumberAnimation {
                        target: buttonsArea;
                        property: "opacityWSt";
                        duration: mainView.animationsStep;
                        easing.type: Easing.InOutQuad;
                    }
                }

            }
        },
        Transition {
            from:"show"; to:"hide"
            reversible: false

            SequentialAnimation{

                ParallelAnimation{
                    NumberAnimation {
                        target: buttonsArea;
                        property: "opacityClose";
                        duration: mainView.animationsStep;
                        easing.type: Easing.InOutQuad;

                    }

                }

                ParallelAnimation{
                    NumberAnimation {
                        target: buttonsArea;
                        property: "opacityWSt";
                        duration: mainView.animationsStep;
                        easing.type: Easing.InOutQuad;
                    }
                }

            }
        }
    ]

}