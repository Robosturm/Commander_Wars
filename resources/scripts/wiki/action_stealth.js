var Constructor = function()
{
    // called for loading the main sprite
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("dive", 2);
        wikipage.loadText(qsTr("Stealths a unit and makes it invisible to all enemies. A stealthed unit is only visible when a unit is nearby it. ") +
		                  qsTr("In addition only units of the same Unit Type <div c='00ff00'> Ground, Naval or Air</div> can attack it. ") +
		                  qsTr("In most cases stealthed units have a higher fuel consumption per turn, while beeing stealthed."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Unstealth");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
