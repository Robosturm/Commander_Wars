var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediately without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team, map)
    {
        if (team === 0)
        {
            var playername = settings.getUsername();
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Hey, there, Junior"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("And what was your partner's name again? ") + playername + "?",
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Stop calling me Junior! I have a name! It's Andy!"),
                        "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Well now! That's the spirit! Y'all are new to the Orange Star Army, right?"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yeah, so? Hey! How'd you know that?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Well now, that's 'cause I'm Grit! I was beholden to Orange Star not too long ago, myself. So, I'm pretty familiar with most of the military types."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Well, y'all beat me this time. Who knows, maybe we'll see each other on the flip side. So long, ") + playername + qsTr("! You too, Junior!"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr(".... Stop calling me junior!"),
                        "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
        }
    };
    this.gameStart = function(map)
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var list = campaignScript.getBasicBuildList();
        map.getPlayer(0).setBuildList(list);
        map.getPlayer(1).setBuildList(list);
    };
    this.actionDone = function(action, map)
    {

    };

    this.turnStart = function(turn, player, map)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog(map);
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.bmday1Dialog(map);
        }
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog(map);
        }
    };

    this.initDialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Grit! Grit! Where's Grit?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Easy there! I heard you the first time, O bearded one."),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Bearded one? Why you..."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("A slip of the tongue. My apologies, commander."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Enough! Just get ready to move out! Now, listen. Orange Star forces have been deployed where I anticipated. Their commanding officer is an untested youngster, but he's not alone. He's working with an advisor, ") + playername + qsTr(". So don't let your guard down! Our contingent in the area has some indirect combat units for you to use. Now get going, Mr. Sharpshooter!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("What? You want me to go personally? Like, in person?"),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Why you lazy, no-good... What do you think a commanding officer does? If you don't get moving..."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("All right. Keep your beard on. I don't much like the idea, but I'm going."),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Gone at last... He's an impudent rogue, but he's also the best marksman I've ever seen. Those Orange Star fools will never know what hit them. Perfect!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Ha, ha, ha, ha, ha!"),
                    "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                    playername + qsTr("! Andy!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Can you hear me?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("You've been given the authority to use bases to deploy new units onto the field. ") + playername + qsTr(" hasn't received instruction on how to do this, but you're up to speed, right, Andy?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog0.queueAnimation(dialog1);
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
    };

    this.bmday1Dialog = function(map)
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Possum spit! This area wasn't Blue Moon's to begin with."),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Blue Moon is just plain greedy. That's what it is. Fightin' for these folks just don't sit well with me. Well, I'll guess I'll just mosey on out and see what happens."),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        dialog1.queueAnimation(dialog2);
    };

    this.day2Dialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    playername + qsTr("! Andy!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Pick up now!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Yeah, Nell. We're here. What's going on?"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("We've learned that Grit is the opposing CO! Grit is an indirect-fire expert whose attack range is larger than normal. Plus, his range gets even bigger when he uses his CO Power! Make sure you check Grit's attack range throughout the battle."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Understood. Hey, Nell... How do you know so much about this guy?"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Well... we used to be... friends. Um, that's all I had to tell you. Nell out."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Huh? Nell was sure in a hurry. I wonder what was wrong?"),
                    "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

