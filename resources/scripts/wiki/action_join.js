var Constructor = function()
{
    // called for loading the main sprite
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("join", 2);
        wikipage.loadText(qsTr("Combines a wounded unit of the same type with the current unit. This will lead to a unit with combined ammo, fuel and hp ad the highest rank of both units. ") +
		                  qsTr("Hp exceeding the maximum will be added in form of funds to your war funds."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Join");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
