var HEAVY_AI =
{
    getName : function()
    {
        return qsTr("Heavy");
    },

    // for modding implement a function named after the action to modify or implement the behaviour for
    // it will given the following input on which you can return the score for the function
    // the score is capped at 1 and actions with a to low score won't be considered to be executed
    // example for capture
    // ACTION_CAPTURE : function(action)
    // {
    //     return 0.99;
    // },
};
