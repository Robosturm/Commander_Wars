// Counterpoint production tunables. This file loads after the strategy, so a copy dropped in an
// install's resources/aidata/normal/ folder wins over the one compiled into the exe.
// These assignments are the only place the values are defined. Change the numbers, do not delete
// lines: a missing key leaves the strategy reading undefined, which lands on 0.001 for a
// multiplier, 0 for a percent chance, or NaN for a funds amount.
;(function()
{
    if (typeof COUNTERPOINTAI !== "object" || COUNTERPOINTAI === null)
    {
        throw new Error("COUNTERPOINTAI must load before its tunables");
    }

    var strategy = COUNTERPOINTAI;

    // ----- Main knobs -----
    // Score multipliers here always move a unit the same direction: above 1 makes it more
    // likely to be built, below 1 less, even when its score has gone negative.

    // Percent chance a factory rolls to prioritise capture units. 0 stops capper builds entirely.
    strategy.CAPTURE_BASE_CHANCE = 30;
    // That roll only happens while the factory's reserved budget is this low, so rich factories
    // build combat units instead.
    strategy.CAP_ROLL_MAX_BUDGET = 5000;
    // On turn 1, narrow every factory to cappers (plus transports on island maps) when any exist.
    strategy.TURN1_FORCE_CAPPERS = true;
    // Lets the capper roll consider cappers this factory cannot afford, paid for out of another
    // factory's reserve. Does nothing unless RECYCLE_UNUSED_BUDGET is on.
    strategy.CAPPER_BORROW_FROM_RESERVE = true;
    // Percent chance a factory deliberately builds nothing and banks the money.
    strategy.BASE_SKIP_CHANCE = 0;
    // No skipping before this day.
    strategy.BASE_SKIP_MIN_DAY = 2;
    // No skipping unless the treasury is at least this large.
    strategy.BASE_SKIP_MIN_FUNDS = 15000;
    // Lets a factory spend funds no one reserved instead of passing when its own budget is short.
    strategy.AVOID_BUDGET_BASE_SKIPS = false;
    // Hands a finished factory's unspent budget to the factories that have not built yet.
    strategy.RECYCLE_UNUSED_BUDGET = true;
    // Looks for factories again once the planned ones are done. Without this, a factory that had a
    // unit parked on it when the turn's plan was drawn up sits idle even after the unit moves off.
    strategy.LATE_FACTORY_RESCAN = true;
    // Reserved for every factory before the surplus is dealt out, so no factory is left broke.
    strategy.FACTORY_FLOOR = 1000;
    // Score multiplier for tank class units, meaning ground, direct, no capture, no cargo, and
    // past both TANK_MIN_BASE_COST and TANK_MIN_MOVEMENT.
    strategy.TANK_BONUS = 1.5;
    // Score multiplier for indirects, applied unless a CO is granting them extra range.
    strategy.INDIRECT_TAX = 0.70;
    // Extra multiplier per indirect of that type already owned, compounding with each one.
    strategy.INDIRECT_STACK_PENALTY = 0.5;
    // Island maps: score multiplier for air units that are not transports.
    strategy.ISLAND_AIR_BONUS = 1.5;
    // Island maps: score multiplier for warships.
    strategy.ISLAND_NAVAL_BONUS = 1.60;
    // Island maps: score multiplier for sea and hover transports.
    strategy.ISLAND_NAVAL_TRANSPORT_BONUS = 3.0;
    // Island maps: score multiplier for tanks, which cannot reach another landmass unaided.
    strategy.ISLAND_GROUND_TANK_PENALTY = 0.50;
    // How much a cheap enemy capture unit counts when deciding what needs countering. Cheap means
    // at or below CHEAP_CAPPER_MAX_COST.
    strategy.CAP_THREAT_MULTIPLIER = 0.25;
    // Randomness of the final pick. Above 1 flattens the odds across candidates, below 1 favours
    // the top scorer. Clamped to TEMPERATURE_MIN..TEMPERATURE_MAX.
    strategy.TEMPERATURE = 1.0;
    // Starting ceiling on how much surplus a single factory may absorb. Raised automatically to
    // the priciest unit any factory can see, so it never blocks a build outright.
    strategy.FACTORY_BUDGET_CAP_FLOOR = 30000;
    // Before this turn the cheapest capper always gets the TIGHT share below, regardless of pool.
    strategy.CHEAPEST_CAPPER_TURN_GATE = 4;
    // Share of the pick weight handed to the cheapest capper when few are on offer.
    strategy.CHEAPEST_CAPPER_BIAS_TIGHT = 0.90;
    // Its share once the roster is mid sized.
    strategy.CHEAPEST_CAPPER_BIAS_BROAD = 0.50;
    // Its share once the roster is large, spreading picks across the other cappers.
    strategy.CHEAPEST_CAPPER_BIAS_FLOOD = 0.20;
    // Pool sizes the three biases interpolate between. At or below NARROW the bias stays TIGHT,
    // at WIDE it is BROAD, at or above VERY_WIDE it is FLOOD.
    strategy.CAPPER_POOL_NARROW = 3;
    strategy.CAPPER_POOL_WIDE = 6;
    strategy.CAPPER_POOL_VERY_WIDE = 10;
    // Weights for a unit's best, second best and third best matchup. Further matchups are ignored,
    // so a unit is judged on what it beats, not on averaging over the whole enemy army.
    strategy.TOP_N_WEIGHTS = [1.0, 0.7, 0.4];
    // Replaces INDIRECT_TAX when a CO is granting that indirect extra range.
    strategy.INDIRECT_SPECIALIST_BONUS = 1.5;
    // Non island maps: effective range a warship needs before it stops being penalised for
    // shooting land units. Effective range includes CO range bonuses.
    strategy.NAVAL_VS_LAND_RANGE_CUTOFF = 4;
    // Damage multiplier for a short ranged warship shooting land.
    strategy.NAVAL_VS_LAND_SHORT_PENALTY = 0.25;
    // Damage multiplier for a range 1 warship shooting land.
    strategy.NAVAL_VS_LAND_DIRECT_PENALTY = 0.05;
    // How much an enemy indirect's damage counts against a tank when scoring that tank.
    strategy.TANK_VS_INDIRECT_DEF_MUL = 0.4;
    // A ground unit dealing less than this against armour is treated as AA and never as a tank.
    strategy.TANK_AA_ARMORED_THRESHOLD = 30;
    // Enemy capture unit HP above this is compressed logarithmically, so a wall of infantry does
    // not drown out every other threat.
    strategy.CAP_HP_SOFT_CAP = 4;
    // Total enemy direct attacker HP above this scales the retaliation estimate down.
    strategy.ADJACENCY_RETAL_CAP = 4;
    // Island maps: score multiplier for ground units that neither capture, carry, nor count as tanks.
    strategy.ISLAND_GROUND_SUPPORT_PENALTY = 0.05;
    // Island maps: multiplier per transport already owned, compounding with each one.
    strategy.ISLAND_TRANSPORT_DIVERSITY = 0.70;
    // Island maps: multiplies how hard the enemy's air and naval share pulls the build order.
    strategy.ISLAND_ROLE_NUDGE_MULTIPLIER = 5;
    // Island maps: boost for tank capable transports when tanks outnumber ferry capacity.
    strategy.ISLAND_TANK_FERRY_DEMAND_BOOST = 2.5;

    // ----- Internal scales and safety limits -----
    // These shape the scoring maths rather than expressing a preference. Most players never need
    // them, and the MAX_ entries exist to keep a bad save or a huge roster from hanging the AI.

    // Denominator for every percent roll. Raising it makes all percent chances proportionally rarer.
    strategy.PERCENT_MAX = 100;
    // Lower clamp on every score multiplier in this file.
    strategy.FACTOR_MIN = 0.001;
    // Upper clamp on every score multiplier in this file.
    strategy.FACTOR_MAX = 1000;
    // HP scale treated as full health.
    strategy.NORMALIZED_HP_MAX = 10;
    // Divisor converting 0 to 100 style HP onto the 0 to 10 scale.
    strategy.PERCENT_HP_DIVISOR = 10;
    // Minimum cost for a ground unit to count as a tank. Raising it excludes cheap chassis.
    strategy.TANK_MIN_BASE_COST = 6000;
    // Minimum movement for a ground unit to count as a tank.
    strategy.TANK_MIN_MOVEMENT = 5;
    // Scales the logarithmic tail above CAP_HP_SOFT_CAP. Raising it softens the compression.
    strategy.CAP_HP_LOG_SCALE = 1.5;
    // Fraction of a transport's cost used as its island mode score, before the island bonuses.
    strategy.TRANSPORT_BASE_SCORE_FACTOR = 0.1;
    // Discounts the defensive half of an indirect's score, since it is rarely the one shot at.
    strategy.INDIRECT_DEFENSE_FACTOR = 0.8;
    // How strongly the enemy's air and naval share boosts units that counter them.
    strategy.ROLE_SHARE_FACTOR = 0.5;
    // The same, for the enemy's indirect share.
    strategy.INDIRECT_ROLE_SHARE_FACTOR = 0.3;
    // Currency per cost unit. Threat and efficiency maths work in these units, not raw funds.
    strategy.COST_SCALE = 1000;
    // Cheap enemies are treated as at least this valuable when weighing threats.
    strategy.THREAT_VALUE_FLOOR = 1000;
    // Cost ceiling under which an enemy capturer gets CAP_THREAT_MULTIPLIER.
    strategy.CHEAP_CAPPER_MAX_COST = 3000;
    // Damage percentage at which a matchup counts as a hard counter.
    strategy.HARD_COUNTER_DAMAGE = 70;
    // Score multiplier for a hard counter, offensive or defensive.
    strategy.HARD_COUNTER_FACTOR = 2.0;
    // Candidate to enemy cost ratio above which a hard counter is judged overpriced.
    strategy.EXPENSIVE_COUNTER_COST_RATIO = 3.5;
    // Reduced multiplier used for an overpriced hard counter.
    strategy.EXPENSIVE_COUNTER_FACTOR = 1.3;
    // Converts enemy HP into the damage needed to cover it. Raising it makes existing coverage
    // look thinner, so the AI keeps building answers for longer.
    strategy.COVERAGE_DAMAGE_SCALE = 100;
    // Score a candidate keeps even when its target is already fully covered.
    strategy.GAP_FLOOR_BASE = 0.2;
    // Added to that floor per cost unit, so expensive units retain more score when saturated.
    strategy.GAP_FLOOR_COST_FACTOR = 0.01;
    // Ceiling on the computed floor.
    strategy.GAP_FLOOR_MAX = 0.5;
    // Coverage to need ratio above which a threat counts as oversaturated.
    strategy.COVERAGE_SATURATION_RATIO = 1.5;
    // Hard minimum on the floor once saturation applies.
    strategy.SATURATED_GAP_FLOOR_MIN = 0.05;
    // Subtracted from the floor once saturation applies.
    strategy.SATURATED_GAP_REDUCTION = 0.2;
    // Cap on the uncovered threat bonus. Raising it rewards unanswered threats more.
    strategy.GAP_FACTOR_MAX = 1.5;
    // Mobility multiplier floor when the enemy is the faster one.
    strategy.MOBILITY_BASE_FACTOR = 0.5;
    // How much of the movement ratio is added on top of that floor.
    strategy.MOBILITY_RATIO_FACTOR = 0.5;
    // Movement at which a candidate starts earning the fast movement bonus.
    strategy.FAST_MOVEMENT_THRESHOLD = 6;
    // Movement subtracted before the bonus is scaled. Raising it shrinks the bonus.
    strategy.FAST_MOVEMENT_BASE = 5;
    // Bonus gained per movement point above that base.
    strategy.FAST_MOVEMENT_BONUS_STEP = 0.05;
    // Cap on the total fast movement bonus.
    strategy.FAST_MOVEMENT_BONUS_MAX = 0.25;
    // Per duplicate score multiplier applied to every unit type, keeping armies mixed.
    strategy.UNIT_DIVERSITY_FACTOR = 0.85;
    // Exponent on cost in the cost biased ordering. Raising it favours expensive units harder.
    strategy.COST_WEIGHT_EXPONENT = 1.5;
    // Exponent on score per cost. Raising it sharpens picks towards the single best scorer.
    strategy.SCORE_EFFICIENCY_EXPONENT = 1.5;
    // Exponent on raw cost in the same weight. Raising it tilts picks towards pricier units.
    strategy.SCORE_VALUE_EXPONENT = 0.3;
    // Used when TEMPERATURE is missing or not a number.
    strategy.DEFAULT_TEMPERATURE = 1.0;
    // Lower clamp on TEMPERATURE.
    strategy.TEMPERATURE_MIN = 0.1;
    // Upper clamp on TEMPERATURE.
    strategy.TEMPERATURE_MAX = 10.0;
    // Deadband around 1.0 within which the temperature step is skipped entirely.
    strategy.TEMPERATURE_EPSILON = 0.001;
    // Cap on the summed random weights before a roll.
    strategy.MAX_RANDOM_WEIGHT_TOTAL = 1000000000;
    // Most buildable units weighed in one pass. Past this the pass gives up rather than crawl.
    strategy.MAX_CANDIDATE_COUNT = 65536;
    // Most production plans tracked in a turn, one per building or special action.
    strategy.MAX_PLAN_COUNT = 512;
    // Most candidates stored on a single plan.
    strategy.MAX_PLAN_CANDIDATES = 512;
    // Most candidates across all plans in saved planner state.
    strategy.MAX_TOTAL_PLAN_CANDIDATES = 32768;
    // Largest RNG draw counter accepted from a restored save.
    strategy.MAX_PLANNER_DRAW_COUNT = 1000000000;
    // Longest serialized planner state accepted from a save. Anything longer is discarded.
    strategy.MAX_PLANNER_STATE_LENGTH = 8388608;

    // ----- Transport build chances -----
    // With no transport of that kind yet, the chance is BASE + turn * TURN_STEP, capped at MAX.
    // Once one is owned the turn ramp is dropped and a flat per count chance applies, descending
    // so extra transports get progressively harder to justify.

    // Island maps, ground transports: ceiling on the first transport chance.
    strategy.ISLAND_GROUND_TRANSPORT_MAX_CHANCE = 20;
    // Its turn 1 starting chance.
    strategy.ISLAND_GROUND_TRANSPORT_BASE_CHANCE = 3;
    // Percentage points added per turn.
    strategy.ISLAND_GROUND_TRANSPORT_TURN_STEP = 2;
    // Flat chance for a second one.
    strategy.ISLAND_GROUND_TRANSPORT_ONE_CHANCE = 3;
    // Flat chance with two or more already owned.
    strategy.ISLAND_GROUND_TRANSPORT_MANY_CHANCE = 1;
    // Island maps, sea and air transports: ceiling on the first transport chance.
    strategy.ISLAND_TRANSPORT_MAX_CHANCE = 95;
    // Its turn 1 starting chance.
    strategy.ISLAND_TRANSPORT_BASE_CHANCE = 25;
    // Percentage points added per turn.
    strategy.ISLAND_TRANSPORT_TURN_STEP = 12;
    // Flat chance with exactly one owned.
    strategy.ISLAND_TRANSPORT_ONE_CHANCE = 60;
    // Flat chance with exactly two owned.
    strategy.ISLAND_TRANSPORT_TWO_CHANCE = 40;
    // Owned count that selects the third bracket.
    strategy.ISLAND_TRANSPORT_THREE_COUNT = 3;
    // Flat chance at that bracket.
    strategy.ISLAND_TRANSPORT_THREE_CHANCE = 25;
    // Owned count that selects the fourth bracket.
    strategy.ISLAND_TRANSPORT_FOUR_COUNT = 4;
    // Flat chance at that bracket.
    strategy.ISLAND_TRANSPORT_FOUR_CHANCE = 15;
    // Flat chance beyond the fourth bracket.
    strategy.ISLAND_TRANSPORT_MANY_CHANCE = 8;
    // Non island maps: ceiling on the first transport chance.
    strategy.CONNECTED_TRANSPORT_MAX_CHANCE = 75;
    // Its turn 1 starting chance.
    strategy.CONNECTED_TRANSPORT_BASE_CHANCE = 5;
    // Percentage points added per turn.
    strategy.CONNECTED_TRANSPORT_TURN_STEP = 7;
    // Flat chance with exactly one owned.
    strategy.CONNECTED_TRANSPORT_ONE_CHANCE = 2;
    // Flat chance with two or more owned.
    strategy.CONNECTED_TRANSPORT_MANY_CHANCE = 1;
}());
