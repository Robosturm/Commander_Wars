// Counterpoint production tunables. This file loads after the strategy, so a copy dropped in an
// install's resources/aidata/normal/ folder wins over the one compiled into the exe.
// These assignments are the only place most of the values are defined. Change the numbers, do not
// delete lines: a missing key leaves the strategy reading undefined, which lands on 0.001 for a
// multiplier, 0 for a percent chance, or NaN for a funds amount. The exceptions are the keys the
// strategy carries a compiled fallback for, named X_DEFAULT there, which survive a stale copy of
// this file at their shipped value.
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
    // Percent chance a factory randomly builds nothing. Strategic holds are configured below.
    strategy.RANDOM_BASE_SKIP_CHANCE = 0;
    // No random skipping before this day.
    strategy.RANDOM_BASE_SKIP_MIN_DAY = 2;
    // No random skipping unless the treasury is at least this large.
    strategy.RANDOM_BASE_SKIP_MIN_FUNDS = 15000;
    // Lets a factory spend funds no one reserved instead of passing when its own budget is short.
    strategy.AVOID_BUDGET_BASE_SKIPS = false;
    // Hands a finished factory's unspent budget to the factories that have not built yet.
    strategy.RECYCLE_UNUSED_BUDGET = true;
    // Minimum affordable-to-missed score ratio, 0 disables strategic holding.
    strategy.HOLD_FOR_BETTER_RATIO = 0.25;
    // Limits factories that bank surplus while still buying their cheapest candidate.
    strategy.MAX_STRATEGIC_HOLDS_PER_TURN = 3;
    // Weight on the worst affordable next-turn reinforcement retaliation.
    strategy.PHANTOM_RETAL_WEIGHT = 0.5;
    // Weight on the best hit against an affordable next-turn reinforcement.
    strategy.PHANTOM_OFFENSE_WEIGHT = 0.5;
    // True derives reproducible daily seeds, false draws fresh daily seeds.
    strategy.PLANNER_DETERMINISTIC_SEED = false;
    // Looks for factories again once the planned ones are done. Without this, a factory that had a
    // unit parked on it when the turn's plan was drawn up sits idle even after the unit moves off.
    strategy.LATE_FACTORY_RESCAN = true;
    // Holds back the cheapest ground capturer for each factory that is currently blocked by a unit
    // parked on it, so the factories that can build right now cannot spend its money first. Turning
    // this off lets an expensive transport be bought sooner at the cost of idling those factories.
    strategy.RESERVE_BLOCKED_FACTORIES = true;
    // Prioritize surplus for higher-value builds without idling factories.
    strategy.DYNAMIC_FLOOR = true;
    // Raises the reservation of a factory that can build a capturer; it never lowers it below that
    // capturer's own cost, so anything under the cheapest capturer price does nothing. 0 means
    // reserve exactly the cheapest capturer, which keeps ground factories from sitting idle without
    // hardcoding a price. Factories with no capturer reserve nothing and stay skippable.
    strategy.FACTORY_FLOOR = 0;
    // Include known air, naval, and hover factories in cheapest-unit floors.
    strategy.FLOOR_ALL_DOMAINS = false;
    // Randomly choose which factories receive surplus instead of value ordering.
    strategy.RANDOM_SURPLUS_FUNDING = false;
    // Turns of income a surplus target may cost beyond what is spare now; 0 means this turn only.
    strategy.VALUE_TARGET_MAX_TURNS = 1;
    // Lower values fund more factories with surplus.
    strategy.SURPLUS_FUNDED_DIVISOR = 3;
    strategy.SURPLUS_FUNDED_MIN = 1;
    // Zero removes turn-to-turn variation in the funded count.
    strategy.SURPLUS_FUNDED_JITTER = 1;
    // How many factories nearest the fighting take turns holding the surplus; 1 always funds the closest.
    strategy.SURPLUS_ROTATION_SLOTS = 3;
    // Score multiplier for tank class units, meaning ground, direct, no capture, no cargo, and
    // past both TANK_MIN_BASE_COST and TANK_MIN_MOVEMENT.
    strategy.TANK_BONUS = 1.5;
    // Raise to answer uncovered enemy air more aggressively; zero disables the boost.
    strategy.AA_COVERAGE_URGENCY = 4;
    // Lower values demote anti-air when no attack-capable enemy air exists.
    strategy.AA_NO_AIR_DISCOUNT = 0.35;
    // Cap the boost when a small air share creates a large coverage ratio.
    strategy.AA_COVERAGE_BOOST_MAX = 25;
    // Higher values permit fewer anti-air builds per turn.
    strategy.AA_ENEMY_AIR_PER_UNIT = 2.5;
    // Score multiplier for indirects, applied unless a CO is granting them extra range.
    strategy.INDIRECT_TAX = 0.70;
    // Extra multiplier per indirect of that type already owned, compounding with each one.
    strategy.INDIRECT_STACK_PENALTY = 0.5;
    strategy.INDIRECT_SATURATION_FREE_COUNT = 2;
    // Let saturated indirect scores cross below competing negative scores.
    strategy.INDIRECT_SATURATION_COST_WEIGHT = 60;
    // Negative disables the fielded indirect cap.
    strategy.MAX_INDIRECT_UNITS = -1;
    // Ignore the indirect cap for COs with a positive range bonus.
    strategy.MAX_INDIRECT_IGNORE_INDIRECT_CO = true;
    // Island maps: score multiplier for air units that are not transports.
    strategy.ISLAND_AIR_BONUS = 1.5;
    // Island maps: score multiplier for warships.
    strategy.ISLAND_NAVAL_BONUS = 1.60;
    // Island maps: score multiplier for any transport that can cross water, sea, hover or air.
    strategy.ISLAND_NAVAL_TRANSPORT_BONUS = 3.0;
    // Island maps: score multiplier for tanks, which cannot reach another landmass unaided.
    strategy.ISLAND_GROUND_TANK_PENALTY = 0.50;
    // How much a cheap enemy capture unit counts when deciding what needs countering. Cheap means
    // at or below CHEAP_CAPPER_MAX_COST.
    strategy.CAP_THREAT_MULTIPLIER = 0.25;
    // Randomness of the final pick. Above 1 flattens the odds across candidates, below 1 favours
    // the top scorer. Clamped to TEMPERATURE_MIN..TEMPERATURE_MAX.
    strategy.TEMPERATURE = 1.0;
    // Reject negative combat picks when an affordable alternative exists.
    strategy.SKIP_NEGATIVE_SCORERS = true;
    // Reject fallback transports when an affordable alternative exists.
    strategy.SKIP_UNWANTED_TRANSPORTERS = true;
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
    // Demote slower, costlier capper alternatives without changing the cheapest capper's bias.
    strategy.CAPPER_VARIETY_MOVE_WEIGHT = 1.0;
    // Preserve a minimum draw share for dominated capper alternatives.
    strategy.CAPPER_VARIETY_MIN_SHARE = 0.1;
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
    // Damage weighted enemy direct attacker HP above this scales the retaliation estimate down.
    strategy.ADJACENCY_RETAL_CAP = 4;
    // Damage percent at which an attacker counts its full HP toward retaliation.
    strategy.ADJACENCY_DAMAGE_NORM = 100;
    // Island maps: score multiplier for ground units that neither capture, carry, nor count as tanks.
    strategy.ISLAND_GROUND_SUPPORT_PENALTY = 0.05;
    // Island maps: multiplier per transport already owned, compounding with each one.
    strategy.ISLAND_TRANSPORT_DIVERSITY = 0.70;
    // Island maps: multiplies how hard the enemy's air and naval share pulls the build order.
    strategy.ISLAND_ROLE_NUDGE_MULTIPLIER = 5;
    // Island maps: boost for tank capable transports when tanks outnumber ferry capacity.
    strategy.ISLAND_TANK_FERRY_DEMAND_BOOST = 2.5;
    // Share of capture targets that must be unwalkable before a transport that can actually deliver
    // one is offered immediately rather than waiting for the per turn chance to climb. 0 means
    // always offer it, 1 means only when nothing at all can be captured on foot.
    strategy.FERRY_URGENT_STRANDED_SHARE = 0.5;
    // How many buildings are sampled when measuring what is unwalkable, counted separately for
    // enemy and for neutral so a map full of enemy property cannot crowd the neutrals out.
    strategy.FERRY_TARGET_SAMPLE = 24;
    // Ceiling on island maps built per turn. Each one is a full flood fill of the map, and each
    // distinct transport movement type needs its own, so this bounds the worst case.
    strategy.MAX_ISLAND_MAPS = 6;
    // Most transports the urgency above may buy outright. Past this count no more are bought at all
    // on an island map that strands its targets; anywhere else the ordinary per turn chance below
    // still applies. Raise it for maps where one boat cannot keep up, lower it to 1 to stop a
    // second hull being rushed.
    strategy.FERRY_MAX_HULLS = 2;
    // Ground capturers owned per transport wanted, up to the ceiling above. Lower means each new
    // batch of capturers asks for another hull sooner.
    strategy.FERRY_CAPPERS_PER_HULL = 2;
    // Whether the AI may leave money unspent to afford a transport it cannot buy this turn. With
    // this off it always spends what it has, so an expensive hull on a poor map is never reached.
    strategy.SAVE_FOR_FERRY = true;
    // How many turns of income the saving above may plan across. The test is money already spare
    // plus this many turns of income, not income alone, so 1 still reaches a hull far dearer than
    // one turn earns: the spare money accumulates until it does. Raising it only matters when
    // income is thin, where it trades a longer gamble for arriving a turn sooner.
    // Capturer budgets are reserved before any of this, so saving never idles a ground factory.
    strategy.FERRY_SAVE_MAX_TURNS = 1;
    // Whether the AI may bank for a unit that answers a threat it is currently short against, the
    // way a player holds funds for a tank after seeing one built. Off means it always spends what it
    // has and only ever counters with whatever this turn's money reaches.
    strategy.SAVE_FOR_COUNTERS = true;
    // How short of an enemy unit class the AI must be before banking for an answer is considered.
    // This is existing damage against that class over the damage needed to deal with it, so 0.5
    // means "less than half covered". Raise it to bank more readily, lower it to bank only when
    // nearly helpless. This is the main dial on how cautious the AI is with its money.
    strategy.COUNTER_GAP_RATIO = 0.5;
    // How much better the banked-for unit must be than the best answer this turn's money already
    // buys, as a damage multiple. 1.5 means it must hit half again as hard, which stops the AI
    // waiting a turn for a marginal upgrade over something it could build right now.
    strategy.COUNTER_WORTH_RATIO = 1.5;
    // Turns of income the counter saving may plan across, same meaning as the ferry setting above.
    // 1 keeps it to answers that are one turn away, which is what a player usually does.
    strategy.COUNTER_SAVE_MAX_TURNS = 1;
    // First day the AI may bank for a counter. Before the enemy army is on the board the coverage
    // reading is mostly noise, and holding money that early costs opening presence to answer a
    // threat that does not exist yet. Ferry saving is not affected by this.
    strategy.COUNTER_SAVE_MIN_DAY = 3;

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
    // Island mode transport score, as a fraction of COST_SCALE rather than of the hull's own price,
    // so that cheaper transports win on efficiency instead of losing for being cheap.
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
    // Lower-damage counters contribute quadratically less coverage.
    strategy.COVERAGE_QUALITY_DAMAGE = 55;
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
    // Fallback movement reference when no candidate roster is available.
    strategy.MOBILITY_VALUE_REFERENCE = 5;
    // Higher values reward above-median movement more sharply.
    strategy.MOBILITY_VALUE_SLOPE = 0.5;
    // Prevent slow units from losing all intrinsic value.
    strategy.MOBILITY_VALUE_FLOOR = 0.5;
    // Limit reach bonuses for unusually fast units.
    strategy.MOBILITY_VALUE_CEIL = 1.5;
    // Credit air movement for ignoring terrain; one disables the bonus.
    strategy.MOBILITY_AIR_REACH_BONUS = 1.25;
    // Per duplicate score multiplier applied to every unit type, keeping armies mixed.
    strategy.UNIT_DIVERSITY_FACTOR = 0.85;
    // Exponent on cost in the cost biased ordering. Raising it favours expensive units harder.
    strategy.COST_WEIGHT_EXPONENT = 1.5;
    // Exponent on score per cost. Raising it sharpens picks towards the single best scorer.
    strategy.SCORE_EFFICIENCY_EXPONENT = 1.5;
    // Exponent on raw cost in the same weight. Raising it tilts picks towards pricier units.
    strategy.SCORE_VALUE_EXPONENT = 0.3;
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
    // Its base before the per turn ramp; turn 1 already adds one step.
    strategy.ISLAND_GROUND_TRANSPORT_BASE_CHANCE = 3;
    // Percentage points added per turn.
    strategy.ISLAND_GROUND_TRANSPORT_TURN_STEP = 2;
    // Flat chance for a second one.
    strategy.ISLAND_GROUND_TRANSPORT_ONE_CHANCE = 3;
    // Flat chance with two or more already owned.
    strategy.ISLAND_GROUND_TRANSPORT_MANY_CHANCE = 1;
    // Island maps, sea and air transports: ceiling on the first transport chance.
    strategy.ISLAND_TRANSPORT_MAX_CHANCE = 95;
    // Its base before the per turn ramp; turn 1 already adds one step.
    strategy.ISLAND_TRANSPORT_BASE_CHANCE = 25;
    // Percentage points added per turn.
    strategy.ISLAND_TRANSPORT_TURN_STEP = 12;
    // Flat chance with exactly one owned.
    strategy.ISLAND_TRANSPORT_ONE_CHANCE = 60;
    // Flat chance with exactly two owned.
    strategy.ISLAND_TRANSPORT_TWO_CHANCE = 40;
    // Flat chance with exactly three owned.
    strategy.ISLAND_TRANSPORT_THREE_CHANCE = 25;
    // Flat chance with exactly four owned.
    strategy.ISLAND_TRANSPORT_FOUR_CHANCE = 15;
    // Flat chance beyond the fourth bracket.
    strategy.ISLAND_TRANSPORT_MANY_CHANCE = 8;
    // Non island maps: ceiling on the first transport chance.
    strategy.CONNECTED_TRANSPORT_MAX_CHANCE = 75;
    // Its base before the per turn ramp; turn 1 already adds one step.
    strategy.CONNECTED_TRANSPORT_BASE_CHANCE = 5;
    // Percentage points added per turn.
    strategy.CONNECTED_TRANSPORT_TURN_STEP = 7;
    // Flat chance with exactly one owned.
    strategy.CONNECTED_TRANSPORT_ONE_CHANCE = 2;
    // Flat chance with two or more owned.
    strategy.CONNECTED_TRANSPORT_MANY_CHANCE = 1;
}());
