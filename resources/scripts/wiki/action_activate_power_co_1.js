var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("power", 2);
        wikipage.loadText(qsTr("Activates the Power of your second CO. Read the CO's power for more information."));
    };

    this.getTags = function()
    {
        return [qsTr("Action")];
    };

    this.getName = function()
    {
        return qsTr("Power of CO 2");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
