// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

ListView{
    model: ActivitiesModel1{}

    function setCState(cod, val){
        var ind = getCurrentIndex(cod);
        model.setProperty(ind,"CState",val);

        instanceOfWorkAreasList.setCState(cod,val);

        allWorkareas.updateShowActivities();
    }

    function getCurrentIndex(cod){
        for(var i=0; model.count; ++i){
            var obj = model.get(i);
            if (obj.code === cod)
                return i;
        }
        return -1;
    }

    function cloneActivity(cod){
        var p = getCurrentIndex(cod);

        var ob = model.get(p);
        var nId = getNextId();

        model.insert(p+1,
                     {"code": nId,
                      "Current":false,
                      "Name":"New Activity",
                      "Icon":ob.Icon,
                      "CState":"Running"}
                     );
        instanceOfWorkAreasList.cloneActivity(cod,nId);

        allWorkareas.updateShowActivities();
    }

    function removeActivity(cod){
        var n = getCurrentIndex(cod);
        model.remove(n);
        instanceOfWorkAreasList.removeActivity(cod);
        allWorkareas.updateShowActivities();
    }

    function addNewActivity(){
        var nId = getNextId();

        model.append( {  "code": nId,
                         "Current":false,
                         "Name":"New Activity",
                         "Icon":"Images/icons/plasma.png",
                         "CState":"Running"} );


        instanceOfWorkAreasList.addNewActivity(nId);
        allWorkareas.updateShowActivities();
    }

    function getNextId(){
        var max = 0;

        for(var i=0; i<model.count; ++i){
            var obj = model.get(i);
            if (obj.code > max)
                max = obj.code
        }
        return max+1;
    }

}
