import QtQuick 1.0

Component{

 Item{
     id: mainWorkArea

     property alias normalStateArea_visible: normalStateWorkArea.visible
     property alias workAreaName_visible: workAreaName.visible
     property alias areaButtons_visible: workAreaButtons.visible

     //width:60+3*mainView.scaleMeter
     //height:50+2*mainView.scaleMeter
     width: 1.4 * mainView.workareaHeight
     height:mainView.workareaHeight

        Item{
            id:normalWorkArea

            //parent:workareas

     //       x: mainWorkArea.x + 5; y: mainWorkArea.y + 5
            x:mainView.scaleMeter/10; y:x;
            width: mainWorkArea.width - (mainView.scaleMeter/5);
            height: mainWorkArea.height - (mainView.scaleMeter/5) + (4*mainView.scaleMeter/5);

            Behavior on x { enabled: normalWorkArea.state!="dragging"; NumberAnimation { duration: 400; easing.type: Easing.OutBack } }
            Behavior on y { enabled: normalWorkArea.state!="dragging"; NumberAnimation { duration: 400; easing.type: Easing.OutBack } }

            state:"s1"


            Row{
                id:normalStateWorkArea
                spacing: 0

                WorkAreaImage{
                    id:borderRectangle
                    width: normalWorkArea.width
                    height: normalWorkArea.height -  (4*mainView.scaleMeter/5)

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            workAreaButtons.state="show"
                        }

                        onExited: {
                            workAreaButtons.state="hide"
                        }


                    }//image mousearea
                }

                WorkAreaBtns{
                    id:workAreaButtons
                    height: normalWorkArea.height -  (4*mainView.scaleMeter/5)

                    opacity: mainWorkArea.ListView.view.model.count>1 ? 1 : 0

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            workAreaButtons.state="show"
                        }

                        onExited: {
                            workAreaButtons.state="hide"
                        }

                    }//image mousearea
                }


            }//Row

            DTextLine{
                id:workAreaName
                y:normalStateWorkArea.height-7
                width:50+mainView.scaleMeter*3
                height:20+2.5*mainView.scaleMeter/5
                text: elemTitle
               // acceptedText: elemTitle
            }

            states: [
                State {
                    name: "s1"
                    PropertyChanges {
                        target: mainWorkArea
                        normalStateArea_visible: true
                        workAreaName_visible:true
                    }
                },
  /*              State {
                    name: "dragging"
                    when: (loc.currentId == gridRow)&&(elemVisible)
                     PropertyChanges {
                         target: normalWorkArea;
                         x: loc.mouseX - width/2;
                         y: loc.mouseY - height/2;
                         scale: 0.5;
                         z: 18 }
                     PropertyChanges {
                         target: mainWorkArea
                         normalStateArea_visible: true
                         workAreaName_visible:true
                         areaButtons_visible:false
                     }
                }, testing for dragging*/
                State {
                    name:"hidden"
                    when: (!elemVisible)
                    PropertyChanges {
                        target: mainWorkArea
                        normalStateArea_visible: false
                        workAreaName_visible:false
                        areaButtons_visible:false
                    }

                }

            ]

            transitions: Transition { NumberAnimation { property: "scale"; duration: 150} }





        } //Column

        ListView.onAdd: ParallelAnimation {
            PropertyAction { target: mainWorkArea; property: "height"; value: 0 }
            PropertyAction { target: borderRectangle; property: "opacity"; value: 0 }
            PropertyAction { target: borderRectangle; property: "height"; value: 0 }
            PropertyAction { target: workAreaName; property: "opacity"; value: 0 }

            NumberAnimation { target: mainWorkArea; property: "height"; to: mainView.workareaHeight; duration: 400; easing.type: Easing.InOutQuad }
            NumberAnimation { target: borderRectangle; property: "opacity"; to: 1; duration: 500; easing.type: Easing.InOutQuad }
            NumberAnimation { target: workAreaName; property: "opacity"; to: 1; duration: 200; easing.type: Easing.InOutQuad }
        }

        ListView.onRemove: SequentialAnimation {
            PropertyAction { target: mainWorkArea; property: "ListView.delayRemove"; value: true }

            ParallelAnimation{
                  NumberAnimation { target: mainWorkArea; property: "height"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
                  NumberAnimation { target: borderRectangle; property: "height"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
                  NumberAnimation { target: borderRectangle; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
                  NumberAnimation { target: workAreaButtons; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
                  NumberAnimation { target: workAreaName; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
            }


            // Make sure delayRemove is set back to false so that the item can be destroyed
            PropertyAction { target: mainWorkArea; property: "ListView.delayRemove"; value: false }
        }


    }//Item

}//Component




