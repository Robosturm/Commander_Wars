var DIALOG_ANIMATION_BUILDER = function(co, powerMode, map) {
    ANIMATION_BUILDER.call(this, map);
    this.co = co;
    this.powerMode = powerMode;

    this.buildAnimation = function(parentAnimation = null)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(this.powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);
        return powerNameAnimation;
    };
};
DIALOG_ANIMATION_BUILDER.prototype = new ANIMATION_BUILDER(null);