var Constructor = function()
{
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("icon_ma", 2);
        wikipage.loadText(qsTr("The Metall Army is an army of robots. The robots were initially invented by Lash. The Robots AI developped its own behaviour instead of following Lash's or Dark Matters orders. The Metall Army defends it's own territory at all costs when it loses it's territory. The Army tries to reconquer it. Besides this the Metall Army is very defensive orientated and doesn't attack anyone."));
    };

    this.getTags = function()
    {
        return [qsTr("Army")];
    };

    this.getName = function()
    {
        return qsTr("Metall Army");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
