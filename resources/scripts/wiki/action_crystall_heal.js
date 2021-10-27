var Constructor = function()
{
    
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("heal", 2);
        wikipage.loadText(qsTr("Orders a crystall or monolith to heal all own units around it and also refills their ammo and fuel."));
    };

    this.getTags = function()
    {
        return ["Action"];
    };

    this.getName = function()
    {
        return qsTr("Heal with Crystall");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
