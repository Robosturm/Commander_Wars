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
                audio.addMusic("resources/music/cos/green_earth+0.ogg", 0, 36228)
                audio.addMusic("resources/music/cos/green_earth+1.ogg", 0, 36053);
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
        return "GE";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        return 0;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A Blank CO for Green Earth.") +
               qsTr("Green Earth's forces are extremely organized. Eagle, the leader of Green Earth, specializes in aerial combat, with Jess dominating the ground, and ") +
               qsTr("Drake the seas. ");
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
        return qsTr("Green Earth");
    };
}

Constructor.prototype = CO;
var CO_EMPTY_GE = new Constructor();
