var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("co1", 2);
        wikipage.loadText(qsTr("Deploys your second co onto the given unit. All units inside the co range of this unit will get the buff of your second co."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("CO 2");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
