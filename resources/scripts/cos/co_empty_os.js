var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(0);
        co.setSuperpowerStars(0);
    };

    this.activatePower = function(co)
    {
    };

    this.activateSuperpower = function(co, powerMode)
    {
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
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
                audio.addMusic("resources/music/cos/orange_star+0.mp3", 0, 39498)
                audio.addMusic("resources/music/cos/orange_star+1.mp3", 0, 39294);
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 0;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A Blank CO for Orange Star.") +
               qsTr("The Orange Star army is lead by Nell and her Sister Rachel also  de-facto leads the Allied Nations. ") +
               qsTr("With the exception of Jake, the forces of Orange Star concentrates in direct warfare, excelling in frontal assaults. ");
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
        return qsTr("Orange Star");
    };
}

Constructor.prototype = CO;
var CO_EMPTY_OS = new Constructor();
