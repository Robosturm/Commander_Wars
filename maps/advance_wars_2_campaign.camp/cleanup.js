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
                        qsTr("The Orange Star Army is here! I order you to surrender! "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You did it, Andy! This area has been liberated! "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hmmm... But it was kinda too easy... "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Don't worry. I've a feeling that we're in for a whole slew of combat. Today, however, we celebrate your victory! Keep up the good work! "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(true); // no co power
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
    };
    this.actionDone = function(action)
    {
        var variables = map.getGameScript().getVariables();
        // check if the buildings changed there owner to a certain player and call a dialog on it
        var apc = map.getTerrain(0, 5).getUnit();
        var inf0 = map.getTerrain(2, 7).getUnit();
        var inf1 = map.getTerrain(1, 8).getUnit();
        var inf2 = map.getTerrain(2, 9).getUnit();
        var lastDialog = null;
        if (apc === null || apc.getHasMoved() === true)
        {
            // read the tower variable or create it
            var apcDialiog = variables.createVariable("apcDialiog");

            if (apcDialiog.readDataBool() === false)
            {
                var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("That's an APC unit. APCs have no attack capabilities. "),
                            "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("Oh, weak! "),
                            "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("Weak in combat, perhaps, but they're incredibly useful. They can load and transport infantry units and supply your units. "),
                            "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("Load, huh? I don't get it, but it sounds useful! "),
                            "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("If you move infantry units onto APCs, the Load command appears. Then use the Drop command to unload those units. Transporting troops in APCs like this gets your infantry where they need to be in a hurry! "),
                            "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("I get it... Infantry units can move 3 spaces, but APCs can move 6! I've gotta give that a try! "),
                            "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("Be careful: units that have been dropped can only be ordered to Wait. Also, you can only drop units onto terrain they normally can traverse. "),
                            "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("If I dropped infantry in the sea, they might drown, huh? OK. I won't forget. By the way, Nell, what's 'Supply'? "),
                            "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("Units lose both fuel and primary-weapon ammo as they fight. When they run out of primary-weapon ammo, your units use weaker attacks. When they run out of gas, your units can't move. APCs can supply fuel and rounds to units in adjacent spaces. APCs automatically supply any nearby units at the start of each turn. However, you can use the Supply command if you need supplies quickly. "),
                            "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("OK, sooooo... APCs are used to help out other units. Got it! "),
                            "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("All right, now take a close look at movement range. Do you see how the APC can't travel in the mountains? All units have a movement type, and this affects their movement costs. APCs move on treads, like tanks, so they can't cross mountains or rivers. To see intel move the mouse over it and look at the bottom right. "),
                            "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                lastDialog = dialog11;
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
                apcDialiog.writeDataBool(true);
            }
        }
        var turnEndDialog = variables.createVariable("turnEndDialog");
        if (turnEndDialog.readDataBool() === false &&
            (apc === null || apc.getHasMoved() === true) &&
            (inf0 === null || inf0.getHasMoved() === true) &&
            (inf1 === null || inf1.getHasMoved() === true) &&
            (inf2 === null || inf2.getHasMoved() === true))
        {
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("When you've finished deploying all of your units, your turn's finished. Place the cursor in an empty space and press the Left Mouse Button or Space(default). On the Command menu, select End to complete your turn. "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            if (lastDialog !== null)
            {
                lastDialog.queueAnimation(dialog12);
            }
            turnEndDialog.writeDataBool(true);
        }
    };
    this.turnStart = function(turn, player)
    {
        //
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 1 && player === 1)
        {
            GameAnimationFactory.createGameAnimationDialog(
                                        qsTr("That woman's tough... Can I win with these units? Hah! This is where the fun starts. "),
                                        "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! Why aren't the Orange Star lines broken?!? "),
                    "co_random", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("They're... They're stronger than we had anticipated..."),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I don't want excuses! I want victory!"),
                    "co_random", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("More trouble, sir! We have reports of a new Orange Star CO arriving... "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("......... "),
                    "co_random", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Lord Flak, if we continue, we're only going to take more casualties. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Nothing else to do. Use the troops we've got left. This Orange Star CO... What's he like? "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        // orange star dialog
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Whew! OK, we've taken care of the majority of Black Hole's forces here. This Black Hole Army is proving to be quite dangerous."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! We've located the remaining enemy units. We're preparing to launch pursuit. "),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood. I'll take it from here! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Nell! What's the matter?! Are you OK? "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, Andy! Sorry for the sudden call."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Where are the bad guys?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Settle down. I've wiped out most of the enemy. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh... I was hoping I'd get a chance to show off for you. "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't be foolish, Andy! This is no training exercise! You might be a mechanical wizard, but you still need practice with soldiers. I don't think you're as combat ready as you'd like to believe. You can take over for me, but I'm going to give you instructions as you go, and I want you to listen! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am! Let's get started! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The orange units are under your command. Your enemies are those black units in the top-right corner of the map. Your goal is simple: reclaim our land. Do that by capturing the enemy HQ or by defeating all enemy units. Move out, and be careful not to let the enemy defeat your troops. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK! I'm ready to go! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Rushing into battle only to get beaten wins us nothing. Let's review some basic troop deployment first. First, use this cursor to give orders. Move it with the Mouse or W, A, S, D (default). Here, try giving orders to a unit. Move the cursor onto a unit and press the Left Mouse Button or Space(default). That's an infantry unit. They have low firepower, but they're essential to combat. Now, do you remember what our goals are in this map? "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You bet! Get rid of all the bad guys, or capture their HQ! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You did remember! Well done! Now, this is important: only foot soldiers can capture properties. Of course, infantry units are a type of foot soldier. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's right! If I don't have any foot soldiers, I can't capture anything. "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Right! Now, don't forget: you can only capture enemy and neutral properties. The Capture command appears when you move an infantry unit onto these. OK, let's move a unit. Did you notice how some of the spaces around that unit changed color? That's the unit's movement range. When you've finished moving a unit, the Command menu will appear. Select Wait if you have no further orders for that unit. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

