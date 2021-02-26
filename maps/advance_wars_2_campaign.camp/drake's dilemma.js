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
        return qsTr("Destroy both Black Cannons.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The Black Cannons were destroyed. Useless hunks of--"),
                        "co_hawke", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We'll withdraw for the moment. Adjust our preparations."),
                        "co_hawke", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander Kanbei!"),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Ah, Commander Drake. I am pleased to see you unharmed."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You have our gratitude for the reinforcements. I believe we might have gone down with our ships if you hadn't come."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Nonsense. Even surrounded, I believe you would have turned the tables and emerged victorious. We simply sped the process up a bit."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Thanks for the vote of confidence."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I would like to battle at your side again someday."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The feeling's mutual. Maybe when we're storming Black Hole's fortress..."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("May that day come soon. Until then, good-bye."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Good-bye!"),
                        "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
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
        map.getGameRules().setFogMode(GameEnums.Fog_OfWar); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var geList = campaignScript.getGEBuildList();
        map.getPlayer(1).setBuildList(geList);
        var ycList = campaignScript.getYCBuildList();
        map.getPlayer(1).setBuildList(ycList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(2).setBuildList(bhList);
    };
    this.actionDone = function(action)
    {
        var blackCannon1 = map.getTerrain(2, 23).getBuilding();
        var blackCannon2 = map.getTerrain(6, 23).getBuilding();
        if ((blackCannon1 === null || blackCannon1.getBuildingID() === "ZBLACK_BUILDING_DESTROYED") &&
            (blackCannon2 === null || blackCannon2.getBuildingID() === "ZBLACK_BUILDING_DESTROYED"))
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
        else if (turn === 2 && player === 2)
        {
            gameScript.initDialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are the enemy troops doing? "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("They appear to be gathering on a small island on the inland sea. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("And our troops have surrounded the sea?"),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir. A complete wall encircles them. All that remains is to destroy them unit by unit with the Black Cannon. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's too early to relax. Much could happen before they are annihilated. Tell the troops to keep on their toes. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Enemy troops everywhere. I've got to find a way to turn the tides, or we'll all be swept away. Should I weather that cannon's fire in the reefs or transport troops to the HQ with landers? "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! Incoming call from Commander Eagle! "),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("From Eagle? Patch him through. "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Drake, are you OK? "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Eagle? We've hit a patch of rough sea here. The enemy's surrounded us. We've got to find a way to save the troops... "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hold on. I've got some good news for you. "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What is it? "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I've received a communique from Yellow Comet. They've sent reinforcements! And that's not all. They're being led by the emperor himself, Kanbei! "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That IS good news! OK, I'll launch a counteroffensive timed with Kanbei's attack. "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You'll be aiming to take out those 2 Black Cannons, right? "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yep. We can mop up the rest of the enemy once that's done. "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood. I'll request that Kanbei's troops target the Black Cannons, too. "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Thanks, Eagle. You have this old sailor's gratitude. "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Save it for the victory celebration. Good luck to you, Drake. "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander. Black Hole appears to have the inland sea surrounded. "),
                    "co_officier_yc", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What do we know of Green Earth's Commander Drake? "),
                    "co_kanbei", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("He's unharmed, sir. "),
                    "co_officier_yc", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let's establish the field HQ. When that's done, we'll send out the troops. Our mission is to eradicate the 2 Black Cannons. You think you've got Drake where you want him, Black Hole? Prepare to taste steel from both sides! "),
                    "co_kanbei", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
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
        dialog20.queueAnimation(dialog21);
        dialog21.queueAnimation(dialog22);
        dialog22.queueAnimation(dialog23);
    };

    this.day2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So now the Yellow Comet Army has arrived. If they use those missile silos, it may prove problematic. Can the cannons withstand direct missile fire? "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir. Mistress Lash says the missiles won't even leave scratches. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If that's true, we're safe. They don't have many troops deployed yet, do they? Attack immediately. An ounce of prevention as they say. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

