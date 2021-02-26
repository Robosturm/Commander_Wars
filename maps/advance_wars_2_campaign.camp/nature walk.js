var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.getVictoryInfo = function()
    {
        return qsTr("Reach your HQ with any Unit to win.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("No way! They made it through? I can't be defeated here... It's not possible! "),
                        "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You remember this day, princess. This is where you learned what it means to make Blue Moon your enemy. "),
                        "co_grit", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Don't worry, I'll remember. Remember that I have to crush you! Next time we meet, the result will be different! Toodles! "),
                        "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("My, oh my... Glad to see the end of that. "),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("That was great, Commander Grit! "),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well done, Grit! After today, I can see the day of our ultimate victory approaching! All we need to do is march out to meet it! Let's march! "),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_OfWar); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
    };
    this.actionDone = function(action)
    {
        var unit = map.getTerrain(7, 2).getUnit();
        if (unit !== null && unit.getOwner().getPlayerID() === 0)
        {
            map.getPlayer(1).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("All right, everyone. Keep up the pace! Victory lies just beyond those woods. Now, more than ever, we need Blue Moon to be strong... "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hold it right there, Boss! "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How dare you! I'm talking to the troops! "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We're walking into an ambush. There are enemy units in those woods. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Madness! "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("And that ain't all! They've got some crazy weapons pointed right at us. My, oh my... Getting through this is gonna take some doin'. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What should we do? "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't fret none. This one's mine. I figure my abilities are best suited to take on those monster cannons. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Truthfully, Grit. Are you sure you can handle this? "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I believe I'll muddle through somehow. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So, they sent you, did they? Blue Moon CO Grit. Your CO Power is Snipe Attack... You're an indirect-combat unit specialist. Did I miss anything? "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You know your stuff, don't you, princess? "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's no big deal. I AM a genius, you know. The Black Cannon's my invention, too. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Black Cannon, huh? What's it do? "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Do you really want to know? Well, its range of fire is much, much larger than yours. And I've tweaked it so it can now fire every day! I'm gonna destroy you way before you reach your goal. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I see... I reckon I'll have to sneak along through the woods then. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Are you seriously going to try it? "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I suppose I am. I'm sure those cannons are as powerful as can be, but being the biggest don't always mean you're the best. I think I'm gonna have to teach you a lesson, princess. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yikes! I'm so scared! NOT! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Listen up, soldiers! Our goal is to reach our HQ on the far side of these woods. Unit type is not a factor. Any one of you reaches the HQ, we win. As long as you're in the woods, you're safe from cannon fire, so rest there. Now, we cannot allow the HQ to be captured. So if you run into any enemy infantry, be sure to give 'em somethin' to remember you by. All right, let's move out! "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
        dialog7.queueAnimation(dialog8);
        dialog8.queueAnimation(dialog9);
        dialog9.queueAnimation(dialog10);
        dialog10.queueAnimation(dialog11);
        dialog11.queueAnimation(dialog12);
        dialog12.queueAnimation(dialog13);
        dialog13.queueAnimation(dialog14);
        dialog14.queueAnimation(dialog15);
        dialog15.queueAnimation(dialog16);
        dialog16.queueAnimation(dialog17);
        dialog17.queueAnimation(dialog18);
        dialog18.queueAnimation(dialog19);
        dialog19.queueAnimation(dialog20);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

