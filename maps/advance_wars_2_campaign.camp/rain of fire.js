var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("They've made it this far. We should withdraw for now. "),
                        "co_hawke", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Tell the troops they fought well under these terrible conditions. Tell them I'm proud of them. "),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, ma'am. "),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Jess, that was well done. "),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It was all the soldiers' doing. I just nudged them along. "),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("That's exactly right. "),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Eagle, stow that bilge. "),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("However, you weren't terrible, Red. Try to continue not screwing up. "),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Now, hold on a minute, Eagle... Where did he go? "),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Jess, believe it or not, he meant that as a compliment. "),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I know that. I was just wondering if he couldn't find a better way to do it. "),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
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
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var geList = campaignScript.getGEBuildList();
        map.getPlayer(0).setBuildList(geList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
        // disable random volcan fire
        var volcan = map.getTerrain(10, 5).getBuilding();
        volcan.setFireCount(-1);
    };
    this.actionDone = function(action)
    {
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
        if (player === 0 && turn > 1)
        {
            var volcan = map.getTerrain(10, 5).getBuilding();
            var targets = [];
            if (turn % 2 === 0)
            {
                // even day targets
                targets = [Qt.point(2, 3),
                           Qt.point(5, 0),
                           Qt.point(3, 13),
                           Qt.point(6, 10),
                           Qt.point(18, 1),
                           Qt.point(19, 12)];
            }
            else
            {
                // odd day targets
                targets = [Qt.point(1, 1),
                           Qt.point(1, 9),
                           Qt.point(3, 8),
                           Qt.point(4, 6),
                           Qt.point(6, 2),
                           Qt.point(9, 13),
                           Qt.point(13, 0),
                           Qt.point(14, 12),
                           Qt.point(17, 11),
                           Qt.point(19, 4),
                           Qt.point(20, 1),
                           Qt.point(22, 8),
                           Qt.point(23, 0),];
            }
            ZVOLCAN.volcanFire(volcan, targets);
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hawke, Flak's here. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Why? He's supposed to be implementing my plan elsewhere. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("He said he's got something to talk to you about."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Very well. Send him in. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Heard you were launching an attack. "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Near the volcano. "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's correct. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sounds dangerous. "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Which is why we're doing it. The volcano disrupts the air currents. Our enemies will be unable to use their best troops, their air force. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But there's lava flying everywhere. Our own troops are being injured, too. "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Which means what to us, exactly? Any losses we incur are acceptable in the name of victory. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But the strength of the enemy forces attacking my front... "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Now I see. This is all about you. Pretending to be concerned about the troops when it's your own skin... "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Whuh!? No, that's-- "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Before you question my plans again, I suggest you win a battle with one of your own devising. "),
                    "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("...... "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" The lava's going to make little people pancakes! Tee hee hee! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The enemy's preparing an attack near the volcano? "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What're you going to do, Eagle? You can't fly near that thing. "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("In that case, we'll meet the foe on the ground. "),
                    "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hold on, Eagle! You know you're not at your best on the ground. Let me take care of this. Ground combat's my specialty. "),
                    "co_jess", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! Jess! Stop right-- "),
                    "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("She's gone. "),
                    "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Does she think she can decide who's going? Who does she... "),
                    "co_jess", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Eagle... Jess is right. Let her take care of this. "),
                    "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, I know she's right. I'm just letting off steam. "),
                    "co_eagle", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
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
    };

    this.day2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Jess, can you hear me? "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Eagle? What is it? "),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It seems that the lava from the volcano falls in a set pattern. If you can decipher it, you might be able to use it to your advantage. "),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's good intel. Thanks. "),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't endanger yourself by trying anything foolish. Luck be with you."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

