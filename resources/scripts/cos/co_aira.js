var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(7);
    };

    this.activatePower = function(co)
    {

        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);



        audio.clearPlayList();
        CO_AIRA.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        audio.clearPlayList();
        CO_AIRA.loadCOMusic(co);
        audio.playRandom();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/aira.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "PF";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A reserved commander who is often found daydreaming. Likes to walk in the wind.");
    };
    this.getHits = function()
    {
        return qsTr("Cool Breezes");
    };
    this.getMiss = function()
    {
        return qsTr("Surprises");
    };
    this.getCODescription = function()
    {
        return qsTr("Aira never felt a need to specialize, so all units are average.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Enemies expend 5 times more fuel for the next day.");
    };
    this.getPowerName = function()
    {
        return qsTr("Gust Storm");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Weak enemy units and air units suffer 3 HP of damage. All other enemy units have increased movement costs.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Hurricannon");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Amazing what a little gust can do..."),
                qsTr("How can one fight against what they can't see or damage?"),
                qsTr("This is the only surprise I don't mind..."),
                qsTr("The winds will move the tides of war."),
                qsTr("A victory depends on whether my enemy can survive this weather."),
                qsTr("I am certain that my determination is not what one would call sub par.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("...Huh? Oh, right. I won. Woohoo?"),
                qsTr("I'm glad I expected this. A surprise would have been unbearable."),
                qsTr("Victory blew in my direction today.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("The wind blew in the wrong direction."),
                qsTr("Hmm! Oh i have dreamed to much.")];
    };
    this.getName = function()
    {
        return qsTr("Aira");
    };
}

Constructor.prototype = CO;
var CO_AIRA = new Constructor();
