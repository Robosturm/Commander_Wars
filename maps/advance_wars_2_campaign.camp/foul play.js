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
                        qsTr("Our minicannons have been destroyed? All of them? There's been some sort of mistake... Under these conditions... there's no way... we should have lost... "),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Now listen here, Adder. You should really try hard to avoid making me angry. "),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What? What did you do? "),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" I'm an old man. I'm not looking for a fight. I want nothing more than to relax and bake my bones in the sun. So I'm warning you, stop now, while you still can. If not, I don't think I'll be able to restrain myself... "),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hss... Hssssssss! "),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Wow! He sure left in a hurry. "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Vintage Sensei! Well done! Well done indeed! "),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
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
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var ycList = campaignScript.getYCBuildList();
        map.getPlayer(0).setBuildList(ycList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
        map.getPlayer(1).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };
    this.actionDone = function(action)
    {
        var miniCannon1 = map.getTerrain(2, 5).getBuilding();
        var miniCannon2 = map.getTerrain(4, 5).getBuilding();
        var miniCannon3 = map.getTerrain(6, 5).getBuilding();
        if (miniCannon1 === null &&
            miniCannon2 === null &&
            miniCannon3 === null)
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
                    qsTr("A peace treaty? Did Black Hole really suggest that we form a lasting peace with them? "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So it would seem, Daughter. Our consecutive victories have taken their toll on their army. They're proposing a peace treaty that is favorable to Yellow Comet. "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... How do you intend to respond, Kanbei? "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Father, it's a trap! It must be. You must ignore it. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I admit the possibility that this might all be a trap. However, I intend to meet with them and hear their proposal. "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Father! You mustn't... "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I will not raise my sword against one who has thrown his down. That is not the way of the samurai."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This is the location we agreed on. There's no one here. It must be a trap. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No, wait... There's someone... There! "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("......"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We came as promised, Adder. I would hear your plan for peace. "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("3... "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" What? "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("2... "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That-- "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("1..."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That isn't Adder! It's a dummy! "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No! Everyone! Retreat!! "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("0"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Heh heh heh... Right on schedule. The road has been cleared of all obstacles. A leisurely march to the enemy HQ would seem to be in order. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Hate to tell you this, sonny, but that's not going to happen. "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" ...Hsss... So you survived. It matters not. You're too late. Our preparations are complete. This pipe is the route by which materials are transported to our factory. It's also an impenetrable, indestructable wall. So why don't you twiddle your thumbs or something while I go and capture your HQ. Heh heh heh... "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("There's no scheme too base for these scoundrels. "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Heh heh heh... Base? It's not base, it's brilliant! Now out of my way, you senile old fool! I've no time to spare for you. You may watch, though, while I claim my prize! "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sensei! Listen. It's as Adder said. The pipe is indestructible. However, if we concentrate our fire on the seam, we might be able to open a hole in it that will allow our units to pass through. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Mm... Well then, let's give it a shot. "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are you attempting to prove? There's no way that you can turn the table! It's too late! "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This takes me back... Compared to the battles of old, this is nothing but a skirmish. Shall we get started then? Listen to me! Our goal is to destroy the 3 minicannons. If we can take care of them, the foe will lose heart. Move out! "),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
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

