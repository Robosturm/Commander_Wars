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
        return qsTr("Destroy all 8 Minicannons of the Fortress.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander! The minicannons... They're all..."),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("They've been destroyed? That's it then. We shall withdraw."),
                        "co_hawke", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Look at that! The enemy's abandoning the fortress!"),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It appears that we've won."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Shall we launch a pursuit, sir?"),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("No, we deal with the fortress first."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, sir."),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You have my gratitude. It was your bravery that led me... that led US to victory."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Thank you, sir. We appreciate that."),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The war has just begun. The road before us will be long and hard."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("No problem, sir. As long as you're in command, we can't lose!"),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
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
    };
    this.actionDone = function(action)
    {
        var miniCannon1 = map.getTerrain(20, 1).getBuilding();
        var miniCannon2 = map.getTerrain(20, 8).getBuilding();
        var miniCannon3 = map.getTerrain(23, 3).getBuilding();
        var miniCannon4 = map.getTerrain(23, 6).getBuilding();
        var miniCannon5 = map.getTerrain(28, 3).getBuilding();
        var miniCannon6 = map.getTerrain(28, 6).getBuilding();
        var miniCannon7 = map.getTerrain(31, 1).getBuilding();
        var miniCannon8 = map.getTerrain(31, 8).getBuilding();
        if (miniCannon1 === null &&
            miniCannon2 === null &&
            miniCannon3 === null &&
            miniCannon4 === null &&
            miniCannon5 === null &&
            miniCannon6 === null &&
            miniCannon7 === null &&
            miniCannon8 === null)
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
                    qsTr("Adder, will the fortress be completed on schedule?"),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes. We're proceeding without delays."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" And the minicannons?"),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("All 8 of them have been constructed on the sites you selected."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Then all is well."),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! We've reports of a large Green Earth force heading this way!"),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I estimated they'd be appearing about this time. They're all air units?"),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hold on... Er... I think so, sir. Nothing but air units, sir."),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How predictable. Deploy our troops immediately."),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir."),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Attention all units! Target that fortress and open fire!"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh no! Hold on, sir! This is no good! There are anti-aircraft units everywhere!"),
                    "co_officier_ge", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What?! They knew we were coming... (What should I do? If we allow them to complete that factory, Green Earth will suffer. If we press our attack, we might win, but the casualties will be enormous.)"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! Let's continue with the attack."),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But..."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We're Green Earth's strongest force. We cannot lose. That's what you always tell us, isn't it? "),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("He's right, sir. We won't be beaten by the likes of those Black Hole thugs. And we can't sit by and let them complete that fortress! We can't!"),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Let's hit 'em, Commander!"),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("C'mon, sir!"),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("My soldiers... All right, we proceed as planned. Aim for the heart of their offensive power, the minicannons. Let's give them a show of power they won't soon forget!"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
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

