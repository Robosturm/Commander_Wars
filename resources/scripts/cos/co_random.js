var Constructor = function()
{
    this.init = function(co)
    {
        
    };

    this.getCOArmy = function()
    {
        return "TI";
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A random CO will appear.");
    };
    this.getHits = function()
    {
        return qsTr("Randomness");
    };
    this.getMiss = function()
    {
        return qsTr("Prediction");
    };
    this.getCODescription = function()
    {
        return qsTr("Selects a random CO from all avaiable CO's.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Random Effect");
    };
    this.getPowerName = function()
    {
        return qsTr("Random Power");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Random Effect");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Random Super Power");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("")];
    };
    this.getName = function()
    {
        return qsTr("Random");
    };
}

Constructor.prototype = CO;
var CO_RANDOM = new Constructor();
