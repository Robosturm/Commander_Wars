var Constructor = function()
{
    // called for loading the main sprite
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("repair", 2);
        wikipage.loadText(qsTr("Repairs and refills the target unit. The repair costs don't have to be paid."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Repair");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
