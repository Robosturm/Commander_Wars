var Constructor = function()
{
    // called for loading the main sprite
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("yc", 2);
        wikipage.loadText(qsTr("Yellow Comet is on a group of islands on the far east. They're also the smallest nation of the Allied Nations, but their strong army deters any ") +
		                  qsTr("outside forces from invading them. "));
    };

    this.getTags = function()
    {
        return ["Army"];
    };

    this.getName = function()
    {
        return qsTr("Yellow Comet");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
