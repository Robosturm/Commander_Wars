var Constructor = function()
{
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("icon_bg", 2);
        wikipage.loadText(qsTr("Bolt Guard is a sub section of the Black Hole Army lead by Von Bolt. ") +
		                  qsTr("The CO's of Bolt Guard only wishes to destroy the land to achieve their own goals."));
    };

    this.getTags = function()
    {
        return ["Army"];
    };

    this.getName = function()
    {
        return qsTr("Bolt Guard");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
