# Debug Menu

An in-game debug toolbox for Commander Wars modders and map makers. It adds a
"Debug..." action to the empty-field menu, directly above End Turn. Every
command is a real game action; keep the mod out of ranked or multiplayer
matches.

## Menu layout

Step 0 picks a category, step 1 picks a command, further steps depend on the
command (a submenu or a tile pick on the map).

### Inspect...
- **Inspect tile**: pick any tile; dumps the terrain, building and unit on it
  together with every known script variable, plus the unit owner's player and
  CO variables. This is the main tool for looking at hidden mod state.
- **Inspect current player**: funds, team, control type, player variables,
  and per CO the raw power charge (a double measured in stars), star counts,
  attached perks and CO variables.
- **Inspect AI state**: which AI script family is loaded, per player the
  control type and, for AI players, the production system state including
  script variables such as SAVINGSTARGET.
- **Inspect victory rules**: every active victory rule with its rule
  variables, plus which chain-patch sentinels are installed.
- **Dump full map state to log**: day, per player funds and units.

### Player...
- Add funds, set funds to 0, set funds to a large value.
- **Set CO charge...**: empty, just below power (tests the almost-charged
  edge), power threshold, or full. Applies to both COs and prints the raw
  charge double afterwards. The engine clamps charge to the star total, so
  there is deliberately no overfill preset.
- **Defeat player...**: instantly defeats a chosen player, for testing
  victory rules.

### Unit...
- Heal + resupply all own units; refresh all own units (move again).
- Tile-targeted: set HP, refresh one unit, promote, kill (fires death
  triggers), delete (silent), change owner, spawn a unit.

### Match...
- Fog toggle that remembers and restores the map's original fog mode.
- **Set weather...**: pick any loaded weather directly, including modded
  weathers, applied for one full round.

### Perks...
- List attached perks with their isActive result per CO (catches the CO1
  silent no-op under rulesets that override isActive).
- Grant or revoke any loaded perk on either CO mid-game.
- Lift the perk budget caps.

### Logging...
- Set the engine log level.
- Toggle the eAI and eJavaScript log modules; without eJavaScript, JS prints
  are dropped, and without eAI all engine AI tracing is invisible.
- Toggle mod debug flags at runtime (COREAI.DEBUG, Diplomacy.DEBUG,
  Diplomacy.MATCH_TELEMETRY, DB.DEBUG, plus anything mods register).

### Toggles...
- Keep power full and keep funds topped up, re-applied every turn for the
  player who switched them on.
- AI-mode order icons overlay; developer mode.

## Extending the menu from another mod

The registry global `DEBUG_MENU` is defined in
`scripts/general/debug_menu_registry.js`. Registration is data only, so call
it from any script after that file has loaded (inside `finalizeLoading` is
always safe).

```js
DEBUG_MENU.registerCommand({
    id: "MYMOD_CMD_RESET",
    text: function(map) { return qsTr("MyMod: reset counters"); },
    steps: [],
    perform: function(map, args)
    {
        // your debug logic
    }
});
```

Commands appear under "Mods...". `steps` may contain
`{type: DEBUG_MENU.STEP_MENU, options: function(map, args)}` entries
returning `[{id, text}]`, or `{type: DEBUG_MENU.STEP_FIELD,
points: function(map, args)}` entries returning `[{x, y}]`; picked values
arrive in `args` in step order (menu picks as strings, field picks as
`{x, y}`).

Register your hidden state so the inspector can show it:

```js
DEBUG_MENU.registerVariables("unit", [
    {name: "MYMOD_STACKS", type: "i32"},
    {prefix: "MYMOD_SLOT_", from: 0, to: 4, type: "i32"}
]);
```

Containers: `unit`, `building`, `terrain`, `player`, `co`, `system` (AI
production system), `rule` (victory rule), `map`. Types: `i32`, `u32`,
`f32`, `bool`, `str`, `li32`, `lstr`.

Register a runtime debug flag so it shows under Logging:

```js
DEBUG_MENU.registerFlag({
    id: "MYMOD_DEBUG",
    label: "MyMod verbose logging",
    isPresent: function() { return typeof MYMOD !== "undefined"; },
    get: function() { return MYMOD.DEBUG; },
    set: function(value) { MYMOD.DEBUG = value; }
});
```
