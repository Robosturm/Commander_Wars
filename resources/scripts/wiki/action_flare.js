var Constructor = function()
{
    // called for loading the main sprite
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("icon_fire", 2);
        wikipage.loadText(qsTr("Fires a flare rocket at the given location. The flare rocket will reveal all terrains in an area around the target during fog of war."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Flare");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
