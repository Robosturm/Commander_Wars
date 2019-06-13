var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function()
    {
        // called when a player wins

    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // no co power
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // map.getGameRules().changeWeatherChance("WEATHER_1SUN", 90); // sets the weather chance of sun to 90. The actual chance is the the value divided through the sum of all chances
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
    };
    this.actionDone = function()
    {

    };
    this.turnStart = function(turn, player)
    {
        //
        if (turn === 1 && player === 0)
        {

        }
    };
    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Tee hee! Epoch i have a new Beta-Release for you. This new Version will be so much fun to play with!!"),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No Lash please no update. I work perfectly."),
                    "co_epoch", GameEnums.COMood_Sad, PLAYER.getDefaultColor(5));
        dialog1.queueAnimation(dialog2);

    };
}

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
