var Constructor = function()
{
    this.coPowers = [  ["POWER",       "power",        qsTr("Power"),         300,    qsTr("Boo-ya!")],
                       ["SUPERPOWER",  "superpower",   qsTr("Superpower"),    300,    qsTr("I got the power!")],
                       ["TAGPOWER",    "tagpower",     qsTr("Tagpower"),      300,    qsTr("Whoa!")],];

    this.registerAchievements = function()
    {
        for (var i = 0; i < ACHIEVEMENT_POWERS.coPowers.length; ++i)
        {
            var data = ACHIEVEMENT_POWERS.coPowers[i];
            var description = qsTr("Use the given amount of %0.")
            description = replaceTextArgs(description, [data[2]]);
            userdata.addAchievement("CO_POWER_" + data[0],
                                    data[3],
                                    data[4],
                                    description,
                                    data[1],
                                    false);
        }
    };
    this.powerUsed = function(power)
    {
        userdata.increaseAchievement("CO_POWER_" + power, 1);
    };
};

Constructor.prototype = ACHIEVEMENT;
var ACHIEVEMENT_POWERS = new Constructor();
