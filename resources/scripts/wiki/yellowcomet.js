var Constructor = function()
{
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("icon_yc", 2);
        wikipage.loadText(qsTr("Yellow Comet is on a group of islands on the far east. They're also the smallest nation of the Allied Nations, but their strong army deters any outside forces from invading them. "));
    };

    this.getTags = function()
    {
        return [qsTr("Army")];
    };

    this.getName = function()
    {
        return qsTr("Yellow Comet");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
