var Constructor = function()
{
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("load", 2);
        wikipage.loadText(qsTr("Unloads loaded units. Units that have been loaded this turn can't move. Units that were loaded in previous turns can immediatly move after beeing unloaded."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Unload");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
