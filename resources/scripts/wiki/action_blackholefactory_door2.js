var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("build", 2);
        wikipage.loadText(qsTr("Orders a Black Hole Factory to produce a unit at the second door. ") +
		                  qsTr("The unit is able to move immediatly."));
    };

    this.getTags = function()
    {
        return [qsTr("Action")];
    };

    this.getName = function()
    {
        return qsTr("Black Hole Factory Door 2");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
