var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("build", 2);
        wikipage.loadText(qsTr("Orders an apc to build an airport on a plain which can be used to resupply air units. The airport will reduce the material of the apc by 1."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Build Airport");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
