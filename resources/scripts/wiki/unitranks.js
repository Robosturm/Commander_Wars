var Constructor = function()
{
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        for (var i = 0; i < UNITRANKINGSYSTEM.icons.length; i++)
        {
            wikipage.loadHeadline(UNITRANKINGSYSTEM.names[i]);
            wikipage.loadImage(UNITRANKINGSYSTEM.icons[i], 2);
            wikipage.loadText(qsTr("Offensive bonus: ") + UNITRANKINGSYSTEM.offensiveBonus[i]);
            wikipage.loadText(qsTr("Defensive bonus: ") + UNITRANKINGSYSTEM.defensiveBonus[i]);
        }
    };

    this.getTags = function()
    {
        return [qsTr("Others")];
    };

    this.getName = function()
    {
        return qsTr("Unit ranks");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
