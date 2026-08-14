var Constructor = function()
{
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("luck", 2);
        wikipage.loadText(qsTr("Luck is a complex calculation which modifies the result of an attack."));
        wikipage.loadText(qsTr("First two random luck roles are done."));
        wikipage.loadText(qsTr("One between negativ luck and zero."));
        wikipage.loadText(qsTr("And a second one between 0 and positiv luck."));
        wikipage.loadText(qsTr("The result of both values is added up representing the final luck value."));
        wikipage.loadText(qsTr("This luck value is reduced by missing health of the attacking unit."));
        wikipage.loadText(qsTr("Finally the defense of the defending unit further reduces the luck damage."));
    };

    this.getTags = function()
    {
        return [qsTr("Others")];
    };

    this.getName = function()
    {
        return qsTr("Luck");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
