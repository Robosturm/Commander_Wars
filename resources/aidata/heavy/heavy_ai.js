var HEAVY_AI =
{
    getName : function()
    {
        return qsTr("Heavy");
    },

    // for modding implement a function named after the action to modify or implement the behaviour for
    // it will given the following input on which you can return the score for the function
    // the score is capped and actions with a to low score won't be considered to be executed
    // see the ai-ini file for the values used for capping etc.
    // example for capture
    // ACTION_CAPTURE : function(ai, action)
    // {
    //     return 0.99;
    // },
    // ACTION_CAPTUREGetBestField : function(ai, action)
    // {
    //     // return the best field for an action in case all map fields are selectable
    //     return Qt.point(0, 0);
    // },
};
