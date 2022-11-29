var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("wait", 2);
        wikipage.loadText(qsTr("Orders a oozium to wait at the current position and end all actions for this turn. Ooziums can wait on enemy units in doing so the enemy unit gets destroyed by the oozium."));
    };

    this.getTags = function()
    {
        return [qsTr("Action")];
    };

    this.getName = function()
    {
        return qsTr("Oozium Wait");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
