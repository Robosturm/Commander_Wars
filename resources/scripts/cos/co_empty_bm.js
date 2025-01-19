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
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/blue_moon+0.ogg", 0, 60283)
                audio.addMusic("resources/music/cos/blue_moon+1.ogg", 0, 24065);
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
        return "BM";
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A Blank CO for Blue Moon.") +
               qsTr("Due to Clone Andy leading an invasion on their land, Blue Moon was at war with Orange Star at the beginning of the original Advance Wars, though ") +
               qsTr("later allied with them, co-finding the Allied Nations along with Green Earth and Yellow Comet to defend against the Black Hole invasion. ");
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
        return qsTr("Blue Moon");
    };
}

Constructor.prototype = CO;
var CO_EMPTY_BM = new Constructor();
