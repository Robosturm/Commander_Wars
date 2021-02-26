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
        return qsTr("Destroy all four lasers.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Are you all right, Sami?"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Eagle! So it was you who saved us. I apologize. I never mean to be a burden on Green Earth."),
                        "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Don't be foolish."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We were trying to bring you supplies, and we let ourselves get surrounded..."),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Everyone makes mistakes. It turned out all right. Let it go."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, but..."),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Sami, I'm not just saying this to save your wounded pride. Orange Star came here to fight alongside Green Earth. That alone has raised the morale of our troops more than you know."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Thank you, Eagle."),
                        "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I should be thanking you. We're very grateful for the supplies. I think they'll ease our burden a bit."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Eagle..."),
                        "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
            dialog8.queueAnimation(dialog9);
            dialog9.queueAnimation(dialog10);
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
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var geList = campaignScript.getGEBuildList();
        map.getPlayer(0).setBuildList(geList);
        var osList = campaignScript.getOSBuildList();
        map.getPlayer(1).setBuildList(osList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(2).setBuildList(bhList);
    };
    this.actionDone = function(action)
    {
        var laser1 = map.getTerrain(5, 5).getBuilding();
        var laser2 = map.getTerrain(4, 17).getBuilding();
        var laser3 = map.getTerrain(16, 6).getBuilding();
        var laser4 = map.getTerrain(17, 16).getBuilding();
        if (laser1 === null &&
            laser2 === null &&
            laser3 === null &&
            laser4 === null)
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
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Lash, how's the laser cannon coming?"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Full steam ahead! I've tweaked it so it can fire every single day! It's sooo dangerous!!!"),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Heh heh heh... Delicious... How's Flak?"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("My troops've been deployed, like you wanted."),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("At last, we will neutralize the Orange Star Army completely."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Why is Orange Star even here? We're in Green Earth now, aren't we?"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It seems they're here bearing supplies. Meddlers! They shouldn't have come!"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So are we going to attack soon?"),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No. First, we suggest they surrender. Their CO's a young girl, you know. Perhaps she can be convinced to come along quietly. Heh heh heh..."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("C'mon everyone! Stay tough! Green Earth's reinforcements are on their way. Just hang on!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander, we have a communique from the Black Hole Army."),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What does it say?"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("They recommend we surrender. They're awaiting our response."),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Surrender?! Right, here's my reply, and I want you to take this down word for word..."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! Black Hole has demanded that Orange Star surrender."),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm... I believe Sami's commanding the Orange Star forces... What was her reply?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, in a word, she turned them down flat."),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What exactly did she say?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Um... I'd rather not say. It wasn't the most... polite response."),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Be that as it may."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Er... How about I write it down on this paper, and then you can read it?"),
                    "co_officier_ge", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));

        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm... ... ...? Ha... Ha ha ha ha!!!"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander?"),
                    "co_officier_ge", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("She certainly is outspoken! It would be a shame to let her get captured by Black Hole."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Er... Yes, sir, it would."),
                    "co_officier_ge", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Get anyone who's available ready for combat. We go now!"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sir, the enemy outnumbers us by..."),
                    "co_officier_ge", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("There's no reason for us to attack them head-on. Either we pour everything at their laser cannon or we capture their properties. One of the two. Our goal is to break their siege. That's all. For the pride of Green Earth, let's save the Orange Star Army!"),
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
        dialog20.queueAnimation(dialog21);
        dialog21.queueAnimation(dialog22);
        dialog22.queueAnimation(dialog23);
        dialog23.queueAnimation(dialog24);
        dialog24.queueAnimation(dialog25);
        dialog25.queueAnimation(dialog26);
        dialog26.queueAnimation(dialog27);
        dialog27.queueAnimation(dialog28);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

