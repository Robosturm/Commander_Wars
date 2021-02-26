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
        return qsTr("Capture all 8 surroundings of the Omega Rocket.");
    };
    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("B-B-Blast it! If we could've launched this missile, we would've blown 'em all to bits!"),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes! We've captured them all! That neutralizes the missile silo!"),
                        "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wahoo! We won!"),
                        "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Right! We did it! Of course, I could've done it myself. What are your plans now, whelp? If you like, I will let you continue to battle here in Blue Moon..."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I'm gonna head back to Orange Star for now."),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hm? You're leaving already?"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I'll be back as soon as everyone at home's ready to go!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Oh, is that so? Well then, see you aga... No, wait! There's no need for you to come again."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        map.getGameRules().addVictoryRule("VICTORYRULE_TURNLIMIT");
        var turnLimit = map.getGameRules().getVictoryRule("VICTORYRULE_TURNLIMIT"); // loose if the turn limit is gone
        turnLimit.setRuleValue(15, 0);

        var bmList = campaignScript.getBMBuildList();
        map.getPlayer(0).setBuildList(bmList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
        map.getPlayer(2).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };
    this.actionDone = function(action)
    {
        var town1 = map.getTerrain(7, 7).getBuilding();
        var town2 = map.getTerrain(7, 10).getBuilding();
        var town3 = map.getTerrain(8, 6).getBuilding();
        var town4 = map.getTerrain(8, 11).getBuilding();
        var town5 = map.getTerrain(11, 6).getBuilding();
        var town6 = map.getTerrain(11, 11).getBuilding();
        var town7 = map.getTerrain(12, 7).getBuilding();
        var town8 = map.getTerrain(12, 10).getBuilding();
        if (town1 !== null && town1.getOwner().getPlayerID() === 0 &&
            town2 !== null && town2.getOwner().getPlayerID() === 0 &&
            town3 !== null && town3.getOwner().getPlayerID() === 0 &&
            town4 !== null && town4.getOwner().getPlayerID() === 0 &&
            town5 !== null && town5.getOwner().getPlayerID() === 0 &&
            town6 !== null && town6.getOwner().getPlayerID() === 0 &&
            town7 !== null && town7.getOwner().getPlayerID() === 0 &&
            town8 !== null && town8.getOwner().getPlayerID() === 0)
        {
            map.getPlayer(2).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Omigash! Commander Olaf! Look at that! It's... It's..."),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What... What... What in winter's name is that?!?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It looks like a missile silo... A really big missile silo. I doubt we can just walk up and push that monster over."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hit it with everything we've got! No Black Hole base is a match for our might!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Do you see the size of that thing? I'm worried about you, Chief."),
                    "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Wha-What should WE do?"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, son, you and I are on intel duty. We've gotta find a way to beat that thing."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Now that Olaf's here, your days are numbered!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm here to help!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmph! It's Andy of Orange Star. What are you doing here?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I just told you... I'm here to help."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Blue Moon needs no aid! We have the power to..."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's a giant missile silo. When 15 days have passed, it'll launch a missile! That missile will take out half of Blue Moon's troops in one blow."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Madness!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's what Nell said anyway."),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That... That's... We must stop that missile!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'll help you! Let's stop that missile together!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grumble grumble... If you're going to insist, you can help!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
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
    };

    this.day2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Olaf? Can you hear me? We found a way to put the kibosh on that giant missile base."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What was that? Are you sure?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("There are 8 properties surrounding the launch tower. They're supplying the power for the base to launch the missile. If you can capture those 8 properties, the tower should be helpless. It's up to you, Frosty!"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'll take care of this! Easier said than done... This accursed pipe is preventing my units from advancing!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If you blow up that seam, you can pass through the hole in the pipe. That's what Nell told me!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, yes... Of course! I already knew that! I thank you all the same, though."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

