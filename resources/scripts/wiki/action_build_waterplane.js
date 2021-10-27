var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("build", 2);
        wikipage.loadText(qsTr("Orders an aircraft carrier to produce a waterplane which can be launched next turn."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Build Waterplane");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
