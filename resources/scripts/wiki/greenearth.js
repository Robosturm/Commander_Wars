var Constructor = function()
{
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("icon_ge", 2);
        wikipage.loadText(qsTr("Green Earth's forces are extremely organized. Eagle, the leader of Green Earth, specializes in aerial combat, with Jess dominating the ground, and Drake the seas. "));
    };

    this.getTags = function()
    {
        return ["Army"];
    };

    this.getName = function()
    {
        return qsTr("Green Earth");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
