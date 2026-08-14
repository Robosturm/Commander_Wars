// Inserts ACTION_DEBUG_MENU into the empty-field menu directly above End
// Turn. Runs at script load: ACTION and PLAYER come from base general
// scripts, which load before any mod. finalizeLoading is NOT a reliable
// hook here, mods shipping a full copy of global.js (e.g. Turtle_Mod)
// clobber it, so it is only used as a defensive re-check.
var DEBUG_MENU_INSTALL =
{
    ACTION_ID: "ACTION_DEBUG_MENU",
    END_TURN_ID: "ACTION_NEXT_PLAYER",

    install: function()
    {
        DEBUG_MENU_INSTALL.installMenuEntry();
        DEBUG_MENU_INSTALL.installTurnHook();
        DEBUG_MENU_INSTALL.installNextPlayerHook();
    },

    // called by the engine via scripts/mapstart.js at every new-game start,
    // after all script loading; re-applies everything a later-loading mod
    // may have wiped by redefining PLAYER, ACTION or ACTION_NEXT_PLAYER
    // wholesale (Turtle_Mod ships full copies of Player.js and global.js)
    onGameStart: function(map)
    {
        DEBUG_MENU_INSTALL.install();
        DEBUG_MENU_INSTALL.ensureAllowed(map);
    },

    installMenuEntry: function()
    {
        var list = ACTION.emptyFieldActions;
        if (list.indexOf(DEBUG_MENU_INSTALL.ACTION_ID) < 0)
        {
            var endTurn = list.indexOf(DEBUG_MENU_INSTALL.END_TURN_ID);
            if (endTurn >= 0)
            {
                list.splice(endTurn, 0, DEBUG_MENU_INSTALL.ACTION_ID);
            }
            else
            {
                list.push(DEBUG_MENU_INSTALL.ACTION_ID);
            }
        }
    },

    // tagged so a later mod assigning PLAYER.startOfTurn can be re-wrapped
    installTurnHook: function()
    {
        var current = PLAYER.startOfTurn;
        if (typeof current === "function" && current.__debugMenuHook === true)
        {
            return;
        }
        var previousStartOfTurn = current;
        var hook = function(player, map)
        {
            // base game ships PLAYER.startOfTurn as null
            if (typeof previousStartOfTurn === "function")
            {
                previousStartOfTurn(player, map);
            }
            DEBUG_MENU_INSTALL.ensureAllowed(map);
            ACTION_DEBUG_MENU.onStartOfTurn(player, map);
        };
        hook.__debugMenuHook = true;
        PLAYER.startOfTurn = hook;
    },

    // ACTION_NEXT_PLAYER.perform runs on every turn change in lockstep on
    // all clients, so injecting here is symmetric; it survives mods that
    // replace the PLAYER object because it hangs off the action instance
    installNextPlayerHook: function()
    {
        if (typeof ACTION_NEXT_PLAYER === "undefined" ||
            ACTION_NEXT_PLAYER === null)
        {
            return;
        }
        var current = ACTION_NEXT_PLAYER.perform;
        if (typeof current !== "function" ||
            current.__debugMenuHook === true)
        {
            return;
        }
        var previousPerform = current;
        var hook = function(action, map)
        {
            previousPerform(action, map);
            DEBUG_MENU_INSTALL.ensureAllowed(map);
            ACTION_DEBUG_MENU.onStartOfTurn(map.getCurrentPlayer(), map);
        };
        hook.__debugMenuHook = true;
        ACTION_NEXT_PLAYER.perform = hook;
    },

    // startOfTurn is symmetric, so this runs identically on every client;
    // mutating allowed actions from a UI getter would desync the map hash.
    // The entry does persist into saved games, which is harmless. New games
    // need no injection: GameRules defaults allowed actions to every loaded
    // action, which includes this one whenever the mod is enabled.
    ensureAllowed: function(map)
    {
        if (map === null || map === undefined)
        {
            return;
        }
        var rules = map.getGameRules();
        var allowed = rules.getAllowedActions();
        if (allowed.indexOf(DEBUG_MENU_INSTALL.ACTION_ID) < 0)
        {
            allowed.push(DEBUG_MENU_INSTALL.ACTION_ID);
            rules.setAllowedActions(allowed);
        }
    }
};

DEBUG_MENU_INSTALL.install();

// second pass in case a later-loading mod replaced PLAYER.startOfTurn;
// harmless no-op when another mod's global.js copy stomps finalizeLoading
DEBUG_MENU_INSTALL.previousFinalizeLoading = finalizeLoading;
finalizeLoading = function()
{
    DEBUG_MENU_INSTALL.previousFinalizeLoading();
    DEBUG_MENU_INSTALL.install();
};
