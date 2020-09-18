var Constructor = function()
{
    this.init = function(co)
    {
        
    };

    this.getCOArmy = function()
    {
        return "OS";
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A random CO will appear.");
    };
    this.getHits = function(co)
    {
        return qsTr("Randomness");
    };
    this.getMiss = function(co)
    {
        return qsTr("Prediction");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Selects a random CO from all available CO's.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Random Effect");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Random Power");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Random Effect");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Random Super Power");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("")];
    };
    this.getDefeatSentences = function(co)
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
