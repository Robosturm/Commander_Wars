var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt1", "+alt2"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(0);
        co.setSuperpowerStars(0);
    };

    this.activatePower = function(co, map)
    {
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/dark_matter+0.mp3", 7684, 58947)
                audio.addMusic("resources/music/cos/dark_matter+1.mp3", 16931, 53583);
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 0;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A Blank CO for Dark Matter.") +
               qsTr("A military production company. Dark Matter originally supplied Black Hole with weapons in order to create a war to raise the income of the company. ") +
               qsTr("After three wars against the allied nations. The allied nations got some intel about the original creators of the war.") +
               qsTr("The Intelligent Defense System section (IDS) is the research section of Dark Matter lead by Dr. Caulder. ") +
               qsTr("Dark Matter is lead by Yukio who lives to make money.");
    };
    this.getHits = function(co)
    {
        return qsTr("N/A");
    };
    this.getMiss = function(co)
    {
        return qsTr("N/A");
    };
    this.getCODescription = function(co)
    {
        return qsTr("N/A");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("N/A");
    };
    this.getPowerName = function(co)
    {
        return qsTr("N/A");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("N/A");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("N/A");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Attack!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Victory!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Defeat...")];
    };
    this.getName = function()
    {
        return qsTr("Dark Matter");
    };
}

Constructor.prototype = CO;
var CO_EMPTY_DM = new Constructor();
