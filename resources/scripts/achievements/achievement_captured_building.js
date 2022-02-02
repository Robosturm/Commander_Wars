var Constructor = function()
{
    this.buildingTable = [["AIRPORT",                 400,    qsTr("What's an airport?")],
                          ["FACTORY",                 700,    qsTr("My factory now!")],
                          ["HARBOUR",                 400,    qsTr("Onto the sea!")],
                          ["HQ",                      200,    qsTr("Nice HQ you got!")],
                          ["LABOR",                   100,    qsTr("Blob spawnpoint!")],
                          ["MINE",                    300,    qsTr("There's some gold!")],
                          ["OILRIG",                  300,    qsTr("We found some oil!")],
                          ["PIPESTATION",             300,    qsTr("What's in those pipes?")],
                          ["RADAR",                   300,    qsTr("I can see you now!")],
                          ["TOWER",                   300,    qsTr("We have a good connection now!")],
                          ["TOWN",                   1500,    qsTr("We need you for money!")],
                          ["SILO",                    300,    qsTr("All fear your name!")],];


    this.registerAchievements = function()
    {
        for (var i = 0; i < ACHIEVEMENT_CAPTURED_BUILDING.buildingTable.length; ++i)
        {
            var data = ACHIEVEMENT_CAPTURED_BUILDING.buildingTable[i];
            userdata.addAchievement("CAPTURED_BUILDING_" + data[0],
                                    data[1],
                                    data[2],
                                    qsTr("Capture a given amount of ") + Global[data[0]].getName(),
                                    data[0],
                                    false, qsTr("Captured building"));
        }
    };
    this.buildingCaptured = function(buildingId)
    {
        userdata.increaseAchievement("CAPTURED_BUILDING_" + buildingId, 1);
    };
    this.siloFired = function()
    {
        userdata.increaseAchievement("CAPTURED_BUILDING_SILO", 1);
    };
};

Constructor.prototype = ACHIEVEMENT;
var ACHIEVEMENT_CAPTURED_BUILDING = new Constructor();
