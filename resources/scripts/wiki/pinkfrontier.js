var Constructor = function()
{
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("icon_pf", 2);
        wikipage.loadText(qsTr("A matriarchal land, Pink Frontier is generally peaceful and focuses on cooperation rather than competition, with deeply spiritual roots. However, experience has taught them that they need to maintain a military to ensure that others respect their desire for peace."));
    };

    this.getTags = function()
    {
        return [qsTr("Army")];
    };

    this.getName = function()
    {
        return qsTr("Pink Frontier");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
