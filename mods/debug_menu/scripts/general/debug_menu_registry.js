// Extension registry for the debug menu: data only, consumed lazily by the
// action, so registration order between mods is free.
var DEBUG_MENU =
{
    CATEGORY_MODS: "DEBUG_CAT_MODS",
    STEP_MENU: "MENU",
    STEP_FIELD: "FIELD",
    LOG_PREFIX: "DEBUG_MENU|",
    // GameConsole ERROR level: prints regardless of the configured log level
    LOG_LEVEL_ERROR: 3,
    T_I32: "i32",
    T_U32: "u32",
    T_F32: "f32",
    T_BOOL: "bool",
    T_STR: "str",
    T_LI32: "li32",
    T_LSTR: "lstr",
    CONTAINER_UNIT: "unit",
    CONTAINER_BUILDING: "building",
    CONTAINER_TERRAIN: "terrain",
    CONTAINER_PLAYER: "player",
    CONTAINER_CO: "co",
    CONTAINER_SYSTEM: "system",
    CONTAINER_RULE: "rule",
    CONTAINER_MAP: "map",
    // caps runaway prefix families to a sane probe count per dump
    PREFIX_SPAN_MAX: 32,

    commands: [],
    variables: {},
    flags: [],

    error: function(message)
    {
        GameConsole.print(DEBUG_MENU.LOG_PREFIX + message,
                          DEBUG_MENU.LOG_LEVEL_ERROR);
    },

    validateSteps: function(steps)
    {
        if (!Array.isArray(steps))
        {
            return false;
        }
        for (var i = 0; i < steps.length; ++i)
        {
            var step = steps[i];
            if (step === null || step === undefined)
            {
                return false;
            }
            if (step.type === DEBUG_MENU.STEP_MENU)
            {
                if (typeof step.options !== "function")
                {
                    return false;
                }
            }
            else if (step.type === DEBUG_MENU.STEP_FIELD)
            {
                if (typeof step.points !== "function")
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    },

    // command: {id, text(map), icon?, category?, steps?, perform(map, args)}
    registerCommand: function(command)
    {
        if (command === null || command === undefined ||
            typeof command.id !== "string" ||
            typeof command.text !== "function" ||
            typeof command.perform !== "function")
        {
            DEBUG_MENU.error("registerCommand rejected: id, text and " +
                             "perform are required");
            return false;
        }
        if (command.steps === undefined)
        {
            command.steps = [];
        }
        if (!DEBUG_MENU.validateSteps(command.steps))
        {
            DEBUG_MENU.error("registerCommand rejected " + command.id +
                             ": steps must be MENU entries with options() " +
                             "or FIELD entries with points()");
            return false;
        }
        var builtins = DEBUG_MENU.resolveNamespace("ACTION_DEBUG_MENU");
        if (DEBUG_MENU.findCommand(command.id) !== null ||
            (builtins !== undefined && builtins !== null &&
             builtins.commands !== undefined &&
             builtins.commands[command.id] !== undefined))
        {
            DEBUG_MENU.error("registerCommand rejected: duplicate id " +
                             command.id);
            return false;
        }
        if (command.category === undefined)
        {
            command.category = DEBUG_MENU.CATEGORY_MODS;
        }
        DEBUG_MENU.commands.push(command);
        return true;
    },

    // entries: {name, type} for exact names, {prefix, from, to, type} for
    // indexed families probed as prefix + i.
    registerVariables: function(container, entries)
    {
        if (DEBUG_MENU.variables[container] === undefined)
        {
            DEBUG_MENU.error("registerVariables rejected: unknown " +
                             "container " + container);
            return false;
        }
        if (!Array.isArray(entries))
        {
            DEBUG_MENU.error("registerVariables rejected: entries must " +
                             "be an array");
            return false;
        }
        var target = DEBUG_MENU.variables[container];
        for (var i = 0; i < entries.length; ++i)
        {
            var entry = entries[i];
            if (entry === null || entry === undefined ||
                DEBUG_MENU.VAR_TYPES.indexOf(entry.type) < 0)
            {
                DEBUG_MENU.error("registerVariables skipped a malformed " +
                                 "entry for " + container);
                continue;
            }
            if (typeof entry.name === "string")
            {
                target.push({name: entry.name, type: entry.type});
            }
            else if (typeof entry.prefix === "string" &&
                     typeof entry.from === "number" &&
                     typeof entry.to === "number" &&
                     entry.to >= entry.from &&
                     (entry.to - entry.from) < DEBUG_MENU.PREFIX_SPAN_MAX)
            {
                target.push({prefix: entry.prefix, from: entry.from,
                             to: entry.to, type: entry.type});
            }
            else
            {
                DEBUG_MENU.error("registerVariables skipped a malformed " +
                                 "entry for " + container);
            }
        }
        return true;
    },

    // flag: {id, label, isPresent()?, get(), set(value)}
    registerFlag: function(flag)
    {
        if (flag === null || flag === undefined ||
            typeof flag.id !== "string" ||
            typeof flag.label !== "string" ||
            typeof flag.get !== "function" ||
            typeof flag.set !== "function")
        {
            DEBUG_MENU.error("registerFlag rejected: id, label, get and " +
                             "set are required");
            return false;
        }
        if (flag.isPresent === undefined)
        {
            flag.isPresent = function() { return true; };
        }
        DEBUG_MENU.flags.push(flag);
        return true;
    },

    findCommand: function(id)
    {
        for (var i = 0; i < DEBUG_MENU.commands.length; ++i)
        {
            if (DEBUG_MENU.commands[i].id === id)
            {
                return DEBUG_MENU.commands[i];
            }
        }
        return null;
    },

    resolveNamespace: function(name)
    {
        if (typeof Global !== "undefined" && Global !== null)
        {
            return Global[name];
        }
        return undefined;
    },

    // factory for the common "boolean switch on a mod namespace" flag
    namespaceBoolFlag: function(id, label, namespaceName, propName)
    {
        return {
            id: id,
            label: label,
            isPresent: function()
            {
                var owner = DEBUG_MENU.resolveNamespace(namespaceName);
                return (owner !== undefined && owner !== null &&
                        typeof owner[propName] === "boolean");
            },
            get: function()
            {
                return DEBUG_MENU.resolveNamespace(namespaceName)[propName];
            },
            set: function(value)
            {
                DEBUG_MENU.resolveNamespace(namespaceName)[propName] =
                    value;
            }
        };
    }
};

DEBUG_MENU.VAR_TYPES = [DEBUG_MENU.T_I32, DEBUG_MENU.T_U32,
                        DEBUG_MENU.T_F32, DEBUG_MENU.T_BOOL,
                        DEBUG_MENU.T_STR, DEBUG_MENU.T_LI32,
                        DEBUG_MENU.T_LSTR];
DEBUG_MENU.CONTAINERS = [DEBUG_MENU.CONTAINER_UNIT,
                         DEBUG_MENU.CONTAINER_BUILDING,
                         DEBUG_MENU.CONTAINER_TERRAIN,
                         DEBUG_MENU.CONTAINER_PLAYER,
                         DEBUG_MENU.CONTAINER_CO,
                         DEBUG_MENU.CONTAINER_SYSTEM,
                         DEBUG_MENU.CONTAINER_RULE,
                         DEBUG_MENU.CONTAINER_MAP];
for (var debugMenuContainerIndex = 0;
     debugMenuContainerIndex < DEBUG_MENU.CONTAINERS.length;
     ++debugMenuContainerIndex)
{
    DEBUG_MENU.variables[DEBUG_MENU.CONTAINERS[debugMenuContainerIndex]] =
        [];
}

// Seed registry: known hidden state across widely used mods so the inspector
// works out of the box. Types come from the owning mod's source. A narrowing
// read (bool on int, li32 on lstr) prints garbage harmlessly, but a widening
// read (i32 on bool, lstr on li32) trips the engine's sticky QDataStream
// status and silently breaks later writes to that variable, so unverified
// flags ship as bool and unverified scalars as i32, never str or lstr.
(function()
{
    var I32 = DEBUG_MENU.T_I32;
    var F32 = DEBUG_MENU.T_F32;
    var BOOL = DEBUG_MENU.T_BOOL;
    var STR = DEBUG_MENU.T_STR;
    var LI32 = DEBUG_MENU.T_LI32;
    var LSTR = DEBUG_MENU.T_LSTR;
    var SCRIBE_DEBT_MAX_INDEX = 9;
    var MAX_PLAYER_INDEX = 7;

    DEBUG_MENU.registerVariables(DEBUG_MENU.CONTAINER_UNIT, [
        {name: "radiationSickness", type: I32},
        {name: "daze", type: I32},
        {name: "empTurns", type: I32},
        {name: "weapon3", type: I32},
        {name: "C4_PLANTED", type: BOOL},
        {name: "HEAVY_CHARGE_PLANTED", type: BOOL},
        {name: "HEAVY_CHARGE_ACTIVE", type: BOOL},
        {name: "launchReady", type: BOOL},
        {name: "defense_charges", type: I32},
        {name: "UNITDEFENDED", type: BOOL},
        {name: "commanded_turn", type: BOOL},
        {name: "discharged", type: BOOL},
        {name: "TURN_COUNT", type: I32},
        {name: "interceptRange", type: I32},
        {name: "apsActive", type: BOOL},
        {name: "apsCharges", type: I32},
        {name: "apAmmoCharges", type: I32},
        {name: "gbuAmmo", type: I32},
        {name: "antiRadAmmo", type: I32},
        {name: "sonobuoyCharges", type: I32},
        {name: "justFired", type: BOOL},
        {name: "SECOND_ATTACK", type: BOOL},
        {name: "LAST_POSITION_X", type: I32},
        {name: "LAST_POSITION_Y", type: I32},
        {name: "infantryMaterials", type: I32},
        {name: "trainMaterials", type: I32},
        {name: "ehp_bonus", type: I32},
        {name: "atkBoost", type: I32},
        {name: "defBoost", type: I32},
        {name: "moveBoost", type: I32},
        {name: "PowerBonus", type: I32},
        {name: "Energy", type: I32},
        {name: "Firepower Bonus", type: I32},
        {name: "Defense Bonus", type: I32},
        {name: "Existing Firepower Bonus", type: I32},
        {name: "Existing Defense Bonus", type: I32},
        {name: "CHROMIS_BURN_LEVEL", type: I32},
        {name: "CHROMIS_BURN_TURNS", type: I32},
        {name: "CHROMIS_POISON", type: BOOL},
        {name: "CHROMIS_SLOW", type: BOOL},
        {name: "CHROMIS_IMMOBILE", type: BOOL},
        {name: "CHROMIS_HEAL_BLOCK", type: BOOL},
        {name: "PYRO_BURN_LEVEL", type: I32},
        {name: "PYRO_BURN_TURNS", type: I32},
        {name: "PYRO_NAPALM_TIMER", type: I32},
        {name: "poisoned", type: BOOL},
        {name: "healingDisabled", type: BOOL},
        {name: "PALLADION_MALUS_IMMUNE", type: BOOL},
        {name: "APOTHEMA_MALUS_IMMUNE", type: BOOL},
        {name: "Saphira Transformation", type: BOOL},
        {name: "Temporary Mech", type: BOOL},
        {name: "Drayke Attacked", type: BOOL},
        {name: "Hijacked Unit", type: BOOL},
        {name: "Original Owner", type: I32},
        {name: "Exodia Formed", type: BOOL},
        {name: "Delayed Death", type: BOOL},
        {name: "X position", type: I32},
        {name: "Y position", type: I32},
        {name: "HP", type: F32},
        {name: "Ammo 1", type: I32},
        {name: "Ammo 2", type: I32},
        {name: "Fuel", type: I32},
        {name: "TIMELORD_SCHEDULED", type: LSTR},
        {name: "TIMELORD_CONSUMABLE", type: BOOL},
        {name: "TIMELORD_CONSUMED", type: LI32},
        {name: "Spaces Moved", type: I32},
        {name: "Placed Marker", type: BOOL},
        {name: "Atlas Warped", type: BOOL},
        {prefix: "SCRIBE_DEBT_ID_", from: 0, to: SCRIBE_DEBT_MAX_INDEX,
         type: I32},
        {prefix: "SCRIBE_DEBT_AMOUNT_", from: 0, to: SCRIBE_DEBT_MAX_INDEX,
         type: F32},
        {name: "DIPLOMACY_SCRIPT", type: STR},
        {name: "DIPLOMACY_SCRIPT_P1", type: I32},
        {name: "DIPLOMACY_SCRIPT_P2", type: I32},
        {name: "DIPLOMACY_SCRIPT_P3", type: I32},
        {name: "DIPLOMACY_SCRIPT_URG", type: I32},
        {name: "DIPLOMACY_PROJ_X", type: I32},
        {name: "DIPLOMACY_PROJ_Y", type: I32},
        {name: "DIPLOMACY_PREV_ROLE", type: STR},
        {name: "DIPLOMACY_PREV_OBJ_X", type: I32},
        {name: "DIPLOMACY_PREV_OBJ_Y", type: I32},
        {name: "DIPLOMACY_LAST_OBJ_DIST", type: I32},
        {name: "DIPLOMACY_STALE_LOCK_COUNT", type: I32},
        {name: "PROC_ENCOUNTER_UNIT", type: BOOL},
        {name: "ROSIE_BLOOD", type: I32},
        {name: "ROSIE_KILL", type: I32},
        {name: "Grigori in Range", type: BOOL},
        {name: "Refreshed Unit", type: BOOL}
    ]);

    DEBUG_MENU.registerVariables(DEBUG_MENU.CONTAINER_BUILDING, [
        {name: "Infantry Master", type: BOOL},
        {name: "DB_TITHE_PAID", type: I32},
        {name: "DB_DOUBLE_SHIFT_USED", type: BOOL},
        {name: "BuiltAlready", type: BOOL},
        {name: "FactoryBuild", type: BOOL},
        {name: "Adjacent Buildings List", type: LSTR},
        {name: "Lumi's Port", type: BOOL},
        {name: "CAN_BUILD", type: BOOL}
    ]);

    DEBUG_MENU.registerVariables(DEBUG_MENU.CONTAINER_TERRAIN, [
        {name: "isSmoked", type: I32},
        {name: "isBurning", type: I32},
        {name: "isPhosphorus", type: I32},
        {name: "isMustardGas", type: I32},
        {name: "isIrradiated", type: I32},
        {name: "originalTerrain", type: STR},
        {name: "scorched_uncapturable", type: BOOL},
        {name: "buildingType", type: STR}
    ]);

    DEBUG_MENU.registerVariables(DEBUG_MENU.CONTAINER_PLAYER, [
        {name: "DIPLOMACY_NAP", type: LI32},
        {name: "DIPLOMACY_GPT_OUT", type: LI32},
        {name: "DIPLOMACY_BOUNTY_OUT", type: LI32},
        {name: "DIPLOMACY_LEASE_OUT", type: LI32},
        {name: "DIPLOMACY_PENDING_PROPOSALS", type: LI32},
        {name: "RELAY_LINKS", type: LSTR},
        {name: "WARDEN_BARRAGES", type: LSTR},
        {name: "SCRIBE_REFLECTION_STAGE", type: I32},
        // atlas markers: li32 only; doctrine's lstr variant reads harmlessly
        // as garbage ints, the reverse direction would poison the stream
        {name: "Marker Count", type: I32},
        {name: "Marker Positions", type: LI32},
        {name: "DECK_POWERS", type: LSTR},
        {name: "DB_PICK_COUNT", type: I32},
        {name: "DB_LAST_PICK_DAY", type: I32},
        {name: "DB_BONUS_PENDING", type: I32},
        {name: "DB_DEEDS", type: I32},
        {name: "DB_DAMAGE_PREV", type: I32},
        {name: "DB_FUNDS_PREV", type: I32},
        {name: "DB_OPTED_OUT", type: BOOL},
        {name: "DB_PERKS_ATTACHED", type: BOOL},
        {name: "DB_AI_ARCHETYPE_IDX", type: I32},
        {name: "DB_OFFER_A", type: STR},
        {name: "DB_OFFER_B", type: STR},
        {name: "DB_OFFER_C", type: STR},
        {name: "DB_BONUS_OFFER_A", type: STR},
        {name: "DB_BONUS_OFFER_B", type: STR},
        {name: "DB_BONUS_OFFER_C", type: STR},
        {name: "DB_CURSE_OFFER_A", type: STR},
        {name: "DB_CURSE_OFFER_B", type: STR},
        {name: "DB_CURSE_OFFER_C", type: STR},
        {name: "DB_AWAITING_PICK", type: BOOL},
        {name: "DB_BONUS_AWAITING", type: BOOL},
        {name: "DB_CURSE_AWAITING", type: BOOL},
        {name: "DB_CURSE_MUTINY_UNTIL", type: I32},
        {name: "DB_CURSE_EMBARGO_UNTIL", type: I32},
        {name: "DB_CURSE_POWER_DRAIN_UNTIL", type: I32},
        {name: "DB_CURSE_FAMINE_UNTIL", type: I32},
        {name: "DB_CURSE_SIGNAL_JAM_UNTIL", type: I32},
        {name: "DB_CURSE_CURSED_COFFERS_UNTIL", type: I32}
    ]);

    DEBUG_MENU.registerVariables(DEBUG_MENU.CONTAINER_CO, [
        {name: "AVALON_ATTACK_COUNT", type: I32},
        {name: "AVALON_NULLIFY_STAGE", type: I32},
        {name: "Kills This Turn", type: I32},
        {name: "grigFireCount", type: I32},
        {name: "CHROMIS_COUNTERS", type: I32},
        {name: "Reset Count", type: I32},
        {name: "scorchedTiles", type: I32},
        {name: "blackoutTurns", type: I32},
        {name: "previousFunds", type: I32},
        {name: "SANJURO_COST_MOD", type: I32},
        {name: "SANJURO_POWER_COST_MOD", type: I32},
        {name: "Discount", type: I32}
    ]);

    DEBUG_MENU.registerVariables(DEBUG_MENU.CONTAINER_SYSTEM, [
        {name: "NAVALBATTLE", type: BOOL},
        {name: "AIRBATTLE", type: BOOL},
        {name: "COMPOSITION_PERFACTORY", type: I32},
        {name: "SAVINGSTARGET", type: I32},
        {name: "NEED_INF_TRANSPORT", type: BOOL},
        {name: "NEED_TANK_TRANSPORT", type: BOOL},
        {name: "INF_TRANSPORT_CROSS_ISLAND", type: BOOL},
        {name: "LASTTRANSPORTERDAY", type: I32},
        {name: "LASTGROUNDSCOUTDAY", type: I32},
        {name: "TURNPRODUCEDUNITS", type: I32}
    ]);

    DEBUG_MENU.registerVariables(DEBUG_MENU.CONTAINER_RULE, [
        {name: "CMODE_KOTH_HOLDER", type: I32},
        {name: "CMODE_KOTH_STREAK", type: I32},
        {name: "CMODE_KOTH_INIT", type: BOOL},
        {name: "CMODE_KOTH_DRAWN", type: BOOL},
        {name: "CMODE_SHRINK_RING", type: I32},
        {name: "CMODE_GROW_RADIUS", type: I32},
        {name: "CMODE_CHAOS_PLAYER", type: I32},
        {name: "CMODE_REVGRAV_REDIRECT_DAY", type: I32},
        {name: "CMODE_LAST_TICK_KOTH", type: I32},
        {name: "CMODE_LAST_TICK_SUSTAIN", type: I32},
        {name: "CMODE_LAST_TICK_SUDDEN", type: I32},
        {name: "CMODE_LAST_TICK_GROW", type: I32},
        {name: "CMODE_LAST_TICK_SHRINK", type: I32},
        {name: "CMODE_LAST_TICK_REVGRAV", type: I32},
        {name: "PROC_INITIALIZED", type: BOOL},
        {name: "PROC_MAP_SALT", type: I32},
        {name: "PROC_ENCOUNTER_PLAYER", type: I32},
        {name: "PROC_LAST_TIMED_DAY", type: I32},
        {prefix: "CMODE_SUSTAIN_STREAK_", from: 0, to: MAX_PLAYER_INDEX,
         type: I32},
        {prefix: "DB_LAST_TICK_PICK_", from: 0, to: MAX_PLAYER_INDEX,
         type: I32},
        {name: "CHESS_ROUND_STARTED", type: BOOL},
        {name: "CHESS_INITIAL_INCOME_DONE", type: BOOL},
        {prefix: "CHESS_PASSED_P", from: 0, to: MAX_PLAYER_INDEX,
         type: BOOL},
        {prefix: "CHESS_BUDGET_P", from: 0, to: MAX_PLAYER_INDEX,
         type: I32},
        {prefix: "CHESS_LOCKED_P", from: 0, to: MAX_PLAYER_INDEX,
         type: BOOL},
        {name: "STD_ROUND_STARTED", type: BOOL},
        {name: "STD_INITIAL_INCOME_DONE", type: BOOL},
        {prefix: "STD_PASSED_P", from: 0, to: MAX_PLAYER_INDEX,
         type: BOOL},
        {prefix: "STD_BUDGET_P", from: 0, to: MAX_PLAYER_INDEX, type: I32},
        {prefix: "STD_LOCKED_P", from: 0, to: MAX_PLAYER_INDEX,
         type: BOOL},
        {name: "TRUE_ROUND_STARTED", type: BOOL},
        {name: "TRUE_INITIAL_INCOME_DONE", type: BOOL},
        {prefix: "TRUE_PASSED_P", from: 0, to: MAX_PLAYER_INDEX,
         type: BOOL},
        {prefix: "TRUE_BUDGET_P", from: 0, to: MAX_PLAYER_INDEX,
         type: I32},
        {prefix: "TRUE_LOCKED_P", from: 0, to: MAX_PLAYER_INDEX,
         type: BOOL}
    ]);

    DEBUG_MENU.registerFlag(DEBUG_MENU.namespaceBoolFlag(
        "DEBUG_FLAG_COREAI", "COREAI.DEBUG (AI mod tracing)",
        "COREAI", "DEBUG"));
    DEBUG_MENU.registerFlag(DEBUG_MENU.namespaceBoolFlag(
        "DEBUG_FLAG_DIPLOMACY", "Diplomacy.DEBUG",
        "Diplomacy", "DEBUG"));
    DEBUG_MENU.registerFlag(DEBUG_MENU.namespaceBoolFlag(
        "DEBUG_FLAG_DIPLOMACY_TLM", "Diplomacy.MATCH_TELEMETRY",
        "Diplomacy", "MATCH_TELEMETRY"));
    DEBUG_MENU.registerFlag(DEBUG_MENU.namespaceBoolFlag(
        "DEBUG_FLAG_DB", "DB.DEBUG (deckbuilder)",
        "DB", "DEBUG"));
})();
