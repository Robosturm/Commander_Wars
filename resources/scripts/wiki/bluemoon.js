var Constructor = function()
{
    // called for loading the main sprite
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("bm", 2);
        wikipage.loadText(qsTr("Due to Clone Andy leading an invasion on their land, Blue Moon was at war with Orange Star at the beginning of the original Advance Wars, though ") + 
						  qsTr("later allied with them, co-finding the Allied Nations along with Green Earth and Yellow Comet to defend against the Black Hole invasion. "));
    };

    this.getTags = function()
    {
        return ["Army"];
    };

    this.getName = function()
    {
        return qsTr("Blue Moon");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
