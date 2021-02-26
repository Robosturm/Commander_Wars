var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return false;
    };

    this.getVictoryInfo = function()
    {
        return qsTr("Destroy the Deathray.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("My... My Deathray..."),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The enemy's weakening! Attack the platform, and halt that launch!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" Listen up, y'all! Let's mop up and skedaddle for home!"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We're almost there. Stand tall, sailors!"),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Let's put an end to this once and for all. All together now."),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Lord Sturm! The missile platform... It won't hold!"),
                        "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The missile must be launched."),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" It's impossible... Eeeeek!"),
                        "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The missile launch platform is completely inoperable."),
                        "co_officier_os", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("YES!!"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I have come so far..."),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You're next on this list, Sturm!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("...only to be stopped by you again. I will not let it end here. I will sacrifice my own flesh, and all of you..."),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("...will join me in death!!!"),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Whoa! Hold it! What're you planning?"),
                        "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" A self-destruct device?"),
                        "co_sonja", GameEnums.COMood_Sad, PLAYER.getDefaultColor(3));
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Possum spit! Maxie! Back off!"),
                        "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You jerk!"),
                        "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Andy!"),
                        "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You think for one second that we're going to let you get away with this!?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("This... This worm..."),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I... am... not... a... WORM!!! I'm Andy! I'm a CO in the Orange Star Army!!!"),
                        "co_andy", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("No! Andy! Run away!"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Andy! Everyone! Get back!"),
                        "co_nell", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Chance is a fickle thing. You never know when it will come your way."),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Who? Hawke?"),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Sturm... I think it's time for you to go! Black Storm!!!"),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Gaaaaaah!!! Hawke...you trai... tor..."),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog29 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Good riddance."),
                        "co_hawke", GameEnums.COMood_Happy, PLAYER.getDefaultColor(4));
            var dialog30 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("But why? Why would you..."),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog31 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("......"),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog32 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Lord... Sturm... Hawke, what have you done?"),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog33 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("As of now, I am the commander-in-chief of the Black Hole Army. If you have anything to say, come forward, or hold your peace."),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog34 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("......"),
                        "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog35 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What's this all about? Weren't you in league with Sturm?"),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog36 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I wanted to test my power. To see how strong I could become. I'm interested in nothing save myself. That was my purpose. Nothing more."),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog37 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("In any case, you saved us all."),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog38 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Does that eliminate his sins? I won't forget what you did to Green Earth!"),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog39 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hah... I didn't ask you to. I'm through with all of you. I'm satisfied with your performances."),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog40 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What? How about this... We continue this fight until I'm satisfied!"),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog41 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Eagle! That's enough! Our troops are on the brink of collapse. There's nothing to be gained by fighting any longer."),
                        "co_drake", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog42 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I may be out of line, sir... But I believe he's correct."),
                        "co_colin", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            var dialog43 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("......"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog44 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The Black Hole Army is in ruins. They're no match for you now. I'll take them and withdraw."),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog45 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hawke! I'm with you all the way!"),
                        "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog46 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Me, too! This looks like it could be fun!"),
                        "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

            var dialog47 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I... I follow the strong."),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog48 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Then prepare your troops. We're leaving this place."),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog49 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hold it, Hawke! What you've done to Macro Land is unforgivable! But... You saved me... You saved all of us. Thanks."),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog50 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hah... Though the Black Hole Army's gone, you would do well to remember me. Until next time!"),
                        "co_hawke", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog51 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("He had his say and left... I wonder if he'll attack again someday."),
                        "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog52 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It's hard to say. If he does, though, we'll just have to drive him back."),
                        "co_eagle", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog53 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We'll have to spend some time preparing if we're going to do that."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog54 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We must help heal the battle-ravaged soil of Macro Land as well."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog55 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("All of that can wait! We won! ...Well, we did, didn't we?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog56 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, we did."),
                        "co_nell", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog57 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well, let's celebrate! It's party time! Paaaarty!"),
                        "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog58 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Figures. We barely escape death, and he wants cake and ice cream."),
                        "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog59 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("PARTY!!!"),
                        "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog60 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey, there! You all look like you're in good spirits!"),
                        "co_hachi", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog61 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Isn't that... Hey, it's Hachi! What brings you all the way out here?"),
                        "co_nell", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog62 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well... I saw all of you in the heat of battle, and it got to me! I just had to come out and walk a battlefield again."),
                        "co_hachi", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog63 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The battle's over, sir..."),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog64 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Don't you believe it, laddie! There's always the War Room, and other modes to boot!"),
                        "co_hachi", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog65 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wo ho ho ho. Long time, no see, Hachi."),
                        "co_sensei", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
            var dialog66 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well I'll be... Sensei! Good to see you looking so fit."),
                        "co_hachi", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog67 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Do you know Sensei?"),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog68 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes. Hachi and I were both pupils under Sensei's tutelage. I remember, Sensei was at times gentle, at times harsher than..."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog69 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Oh, c'mon, Kanbei! No one wants to hear about that! Tell me, what are everyone's plans from here on out?"),
                        "co_hachi", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog70 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" I will return to my demolished hometown and start rebuilding."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog71 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well, in that case, I'm going to help out, too."),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog72 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yep, it's about time for me to mosey along as well."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog73 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yo, Grit. Are you ever coming back to Orange Star?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog74 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hmm... Can't say for sure. Olaf's been pretty good to me, you know? And I do enjoy the slow pace and the fine cookin'. Hey, Maxie... Give me regards to Nell, will ya?"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog75 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yeah..."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog76 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Let's see... I guess I'm off to the academy."),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog77 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("With this out of the way, maybe you can work on improving your grades."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog78 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! Just because we attended the same school doesn't give you the right to say stuff like that! Besides, my diligence may have made the difference here today! Right?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog79 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hee hee... Right."),
                        "co_sonja", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
            var dialog80 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("As for us... We're going to be busy setting things right for Green Earth."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog81 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, we are. I'll go with you now so we can get started right away."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog82 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Afterward, we strengthen our air force! We have to do better in the future to avoid things like this. And that means you, too, Jess. I'm going to work you harder than ever!"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog83 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("That's fine. But there's something else we really need to do. And that's to make you into something other than a one-dimensional flyboy!"),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog84 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I've been wanting to do this for a long time. You and me, one on one. As soon as we get back, I want you on the training field. We'll see who's better once and for all!!!"),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog85 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It will be my pleasure."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog86 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Ohhhh... Not this again. Somebody save me."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

            var dialog87 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Ha ha ha... Everyone sure has a lot of work to do."),
                        "co_nell", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog88 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey... what about the party?"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog89 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("All right, everybody. Nell, Sami, Andy... Oh! And you, too, Hachi! Let's head for home!"),
                        "co_max", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog90 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("OK. When we get back, make sure you stop by the store and see me!"),
                        "co_hachi", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog91 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Good-bye, everyone! Thanks! Thanks for everything!"),
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
            dialog28.queueAnimation(dialog29);
            dialog29.queueAnimation(dialog30);
            dialog30.queueAnimation(dialog31);
            dialog31.queueAnimation(dialog32);
            dialog32.queueAnimation(dialog33);
            dialog33.queueAnimation(dialog34);
            dialog34.queueAnimation(dialog35);
            dialog35.queueAnimation(dialog36);
            dialog36.queueAnimation(dialog37);
            dialog37.queueAnimation(dialog38);
            dialog38.queueAnimation(dialog39);
            dialog39.queueAnimation(dialog40);
            dialog40.queueAnimation(dialog41);
            dialog41.queueAnimation(dialog42);
            dialog42.queueAnimation(dialog43);
            dialog43.queueAnimation(dialog44);
            dialog44.queueAnimation(dialog45);
            dialog45.queueAnimation(dialog46);
            dialog46.queueAnimation(dialog47);
            dialog47.queueAnimation(dialog48);
            dialog48.queueAnimation(dialog49);
            dialog49.queueAnimation(dialog50);
            dialog50.queueAnimation(dialog51);
            dialog51.queueAnimation(dialog52);
            dialog52.queueAnimation(dialog53);
            dialog53.queueAnimation(dialog54);
            dialog54.queueAnimation(dialog55);
            dialog55.queueAnimation(dialog56);
            dialog56.queueAnimation(dialog57);
            dialog57.queueAnimation(dialog58);
            dialog58.queueAnimation(dialog59);
            dialog59.queueAnimation(dialog60);
            dialog60.queueAnimation(dialog61);
            dialog61.queueAnimation(dialog62);
            dialog62.queueAnimation(dialog63);
            dialog63.queueAnimation(dialog64);
            dialog64.queueAnimation(dialog65);
            dialog65.queueAnimation(dialog66);
            dialog66.queueAnimation(dialog67);
            dialog67.queueAnimation(dialog68);
            dialog68.queueAnimation(dialog69);
            dialog69.queueAnimation(dialog70);
            dialog70.queueAnimation(dialog71);
            dialog71.queueAnimation(dialog72);
            dialog72.queueAnimation(dialog73);
            dialog73.queueAnimation(dialog74);
            dialog74.queueAnimation(dialog75);
            dialog75.queueAnimation(dialog76);
            dialog76.queueAnimation(dialog77);
            dialog77.queueAnimation(dialog78);
            dialog78.queueAnimation(dialog79);
            dialog79.queueAnimation(dialog80);
            dialog80.queueAnimation(dialog81);
            dialog81.queueAnimation(dialog82);
            dialog82.queueAnimation(dialog83);
            dialog83.queueAnimation(dialog84);
            dialog84.queueAnimation(dialog85);
            dialog85.queueAnimation(dialog86);
            dialog86.queueAnimation(dialog87);
            dialog87.queueAnimation(dialog88);
            dialog88.queueAnimation(dialog89);
            dialog89.queueAnimation(dialog90);
            dialog90.queueAnimation(dialog91);
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
        turnLimit.setRuleValue(31, 0);

        // set army stuff
        campaignScript.setArmyData(0, campaignScript.getArmy(map.getPlayer(0).getCO(0)));
        campaignScript.setArmyData(1, campaignScript.getArmy(map.getPlayer(1).getCO(0)));
        campaignScript.setArmyData(2, campaignScript.getArmy(map.getPlayer(2).getCO(0)));
        // set bh stuff
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(3).setBuildList(bhList);
        // disable attacking of the weld
        map.getPlayer(3).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };

    this.actionDone = function(action)
    {
        var deathRay = map.getTerrain(16, 5).getBuilding();
        if ((deathRay === null || deathRay.getBuildingID() === "ZBLACK_BUILDING_DESTROYED"))
        {
            map.getPlayer(3).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
			var deathRay = map.getTerrain(16, 5).getBuilding();
			deathRay.setFireCount(deathRay.getFireCount() - 7);
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Lash, is construction on the missile platform completed?"),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I think it's going to take a little more time."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How much?"),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("About 3... No, about 40 days. Maybe..."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I will not wait that long. Finish it in 30 days."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("......"),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Do you understand?"),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK, I'll try..."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("And how is the Deathray?"),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's fine. You can use it now. I even fixed it, too! Now it won't damage our troops when it's fired. It does need 7 days to build up enough energy to fire, though."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm. 7 days... I'll not forget."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! I think I see it! The missile platform!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Here we go..."),
                    "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Look what I've found."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You... You're Sturm!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("And you're the little girl who likes playing with soldiers. Here you are again, sneaking around where you don't belong."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sonja's not alone. We're all here. Different place, same cast."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You're all here? Then you shall all taste despair. For you shall witness the destruction of your world when my missile strikes."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("As long as we're alive, that will never happen."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Empty words, worm. The missile platform is beyond your reach."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What is that supposed to mean?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Enough talk. I would see your power."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hey! Wait a.... Beyond our reach? It's right..."),
                    "co_drake", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ohmigosh! We've just received report of a giant laser blocking the platform!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That would explain why the old boy was so doggone sure of himself."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("In that case, we should strike now, while we have the chance."),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It doesn't look like we'll be able to avoid that contraption."),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So we weaken his defenses, attack the platform, and stop the launch..."),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog29 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That sounds like a plan I can get behind!"),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog30 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let us aim for the laser. Move out, everyone!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
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
        dialog28.queueAnimation(dialog29);
        dialog29.queueAnimation(dialog30);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

