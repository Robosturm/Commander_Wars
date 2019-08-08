var Constructor = function()
{
    // called for loading the main sprite
    this.loadPage = function(wikipage)
    {
        wikipage.loadHeadline(LOADEDWIKIPAGE.getName());
        wikipage.loadImage("dm", 2);
        wikipage.loadText(qsTr("A military production company. Dark Matter originally supplied Black Hole with weapons in order to create a war to raise the income of the company. ") +
						  qsTr("After three wars against the allied nations. The allied nations got some intel about the original creators of the war.") +
		                  qsTr("The Intelligent Defense System section (IDS) is the research section of Dark Matter lead by Dr. Caulder. ") +
						  qsTr("Dark Matter is lead by Yukio who lives to make money."));
    };

    this.getTags = function()
    {
        return ["Army"];
    };

    this.getName = function()
    {
        return qsTr("Dark Matter");
    };
};

Constructor.prototype = WIKIPAGE;
var LOADEDWIKIPAGE = new Constructor();
