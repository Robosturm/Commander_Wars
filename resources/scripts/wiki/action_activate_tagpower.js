var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("tagpower", 2);
        wikipage.loadText(qsTr("Activates the superpower of both CO's. Read the CO's superpower for more information. This will also apply the Tagpower-Synergy. ") +
		                  qsTr("The synergy gives a hidden firepower boost and a luck boost based on the affinity of the co's."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Tagpower");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
