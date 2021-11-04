var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("dive", 2);
        wikipage.loadText("<r>" + qsTr("Stealths a unit and makes it invisible to all enemies. A stealthed unit is only visible when a unit is nearby it. In addition only units of the same Unit Type ") + "</r>" +
                          "<div c='#00ff00'>" + qsTr(" Ground, Naval or Air") + "</div>" +
                          "<r>" + qsTr(" can attack it. In most cases stealthed units have a higher fuel consumption per turn, while beeing stealthed.") + "</r>");
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Stealth");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
