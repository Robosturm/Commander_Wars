var Constructor = function()
{
    // called for loading the main sprite
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("icon_fire", 2);
        wikipage.loadText(qsTr("Orders a death ray to attack all enemy units in front of it, dealing devastating damage to them. This order has a cooldown of 7 days."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Fire with Death Ray");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
