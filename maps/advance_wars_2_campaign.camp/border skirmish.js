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
                        qsTr("Grrrr! Not bad... for a girl!"),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You're the Black Hole CO!? You're just a thug! Who do you think--"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yap, yap, yap! Shut your trap! I'm leaving for now... Next time, I'll put you in your place."),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("He escaped... And he was so rude, too!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Oooh!! That makes me so angry! If I get the chance, I'm gonna run him down!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I'm glad we can depend on you, Sami. "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
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
        var mech = map.getTerrain(3, 8).getUnit();
        var inf = map.getTerrain(3, 7).getUnit();
        var art = map.getTerrain(2, 8).getUnit();
        if (mech === null || mech.getHasMoved() === true)
        {
            // read the tower variable or create it
            var mechDialog = variables.createVariable("mechDialog");
            if (mechDialog.readDataBool() === false)
            {
                var dialog = GameAnimationFactory.createGameAnimationDialog(
                            qsTr("That was a mech unit. Mech units are stronger than infantry units. As foot solderis, they can capture, use transports, and cross mountains. And, as it says under CO on the Map menu, you're tops with infantry. Infantry under your command have superior firepower. This allows them to hold their own even against tanks units. Be careful not to let the enemy fire on you first, though."),
                            "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
                mechDialog.writeDataBool(true);
            }
        }
        if (art === null || art.getHasMoved() === true)
        {
            var artDialog = variables.createVariable("artDialog");
            if (artDialog.readDataBool() === false)
            {
                gameScript.artDialog();
                artDialog.writeDataBool(true);
            }
        }
        if (inf === null || inf.getHasMoved() === true)
        {
            var captureDialog = variables.createVariable("captureDialog");
            if (captureDialog.readDataBool() === false)
            {
                gameScript.captureDialog();
                captureDialog.writeDataBool(true);
            }
        }
    };

    this.artDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The unit you had selected is an artillery unit. It's an indirect- combat unit that can fire on enemies from a distance. Attacking from a distance means you have no fear of counterstrikes. However, indirect-combat units cannot fire on adjacent units. That means that indirect-combat units cannot counterattack."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK! I'll try not to expose them to direct fire!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If you're able to do that, I'm sure they'll treat you right! One more thing, indirect-combat units cannot fire after moving. They can only attack from their current locations."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood! I'll use these units to lie in wait and ambush the foe."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Perfect! Now, do you remember how to check a unit's range of fire? Place the cursor on a unit and press the B (default) or the Right Mouse Button. See the lights? Artillery units have a range of fire of 2-3 spaces. You can confirm this intel by selecting the unit."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };

    this.captureDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm always amazed at your ability to capture properties, Sami! That extra speed is a useful CO Power to have in combat."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Thanks! Being good at capturing and troop transport is fine... but I'm not very comfortable attacking with non-infantry units. I guess that's something I'll have to work on."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, that is important. However, it's vital that you utilize your unique talents and develop your own fighting style, too."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("My... My own fighting style? You're right! I'll do my best, ma'am!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's the spirit! I'm counting on you! Let's get back to my explanation. I'd like to discuss that city you've started capturing. Unlike the enemy HQ, capturing this property won't win you the battle. However, all bases are vital to the success of any campaign. Can you tell me why?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Um... because they provide defensive cover as well as funds... right?"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Absolutely right! Nicely done, Sami! Your army receives funding daily from all allied properties. If you have deployment properties, use your funds to ready new troops. As you have no deployment properties, your funds will only go toward repairing your units. Still, having lots of properties is never a bad thing, so capture as many as you can!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
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
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Grrrr! Two women COs! Good country! I must have it! "),
                        "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        }
        else if (turn === 2 && player === 0)
        {
            gameScript.turn2Dialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Lord Flak! Orange Star forces sighted on the opposite shore..."),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What?!? Is that all they got?! Hah! Send out the tanks! Tanks are powerful, and they move far. Tanks are the best! Orange Star COs? HAH! I will crush them! Crush them all!"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander, enemy troops have been spotted on the far side of the river."),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Are you sure?!? Here? In this rural area?"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are your orders, ma'am? Should we cease ops and attack? "),
                    "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's affirmative, soldier. We can't allow the enemy a foothold this deep in Orange Star territory."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Good assessment, Sami! That's the tactical sense I expect from a special-forces commander!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Nell! What are you doing here?"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Tracking down the CO who's invading Orange Star, of course! Sami, can I trust you to take care of this situation?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am! But if you had some advice you could offer, I'd feel much more confident."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'd be happy to, Sami! OK, troops! Let's move out!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Lord Flak, our infantry has reported finding Orange Star properties."),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? Enemy cities? Capturing bases is grunt work, but I was told we need cash to fund the invasion... Listen up! If it ain't nailed down, steal it! Now, cities, bases, or whatever, they all got Capture Numbers of 20. Any infantry with an HP of 10 can reduce the Capture Number by 10. So, that means you can secure a property in... uh... 2 days! If you move or get taken out while capturing, you gotta start over again! You got that? Well then, get moving! Black Hole needs cash!!!"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sami, you already know the basics of troop deployment, so... Please look at the Cursor Intel window below. There are some stars below the HQ icon, right? Those illustrate the level of defensive cover offered by this terrain. The higher the defensive cover, the less damage you take in combat. So try choosing terrain that allows you to mitigate battle damage. If you need terrain intel, just move the cursor over the terrain. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So that means it's better to engage those tanks from the woods rather than on the plains or roads, right?"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You've got it! Go get 'em, Sami!"),
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
    };

    this.turn2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ouch! That unit's been hurt. I need someplace to rest it."),
                    "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't panic, Sami. There's a way to help that unit out. Units that have taken damage can rest on any allied property. While they're resting, they recover 2 HP at the start of each turn. Plus, resting units also replenish all of their fuel and ammo!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Bases also have the advantage of high terrain cover! Spectacular!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You're right! However, enemies receive the same benefits from their bases. So, capturing properties is vital in any successful combat plan!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood! Capturing is something I excel at!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's our turn. Move any damaged units onto secure properties and rest them. Be sure to continue capturing any bases that are not yet secure. Now, maybe this is something you've been wondering about... There are some numbers diplayed next to your picture, right? Those numbers are your current funds. You have 6,000 G. Funds are used to deploy troops, but don't worry about that now. Depending on the unit's cost, some funds might be needed for repairs. That's it for my lessons today. Can I leave the rest up to you?"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am! I feel confident of victory!"),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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

