#if defined _INC_WEAPON_CONFIG
	#endinput
#endif
#define _INC_WEAPON_CONFIG

#include <a_samp>

// Print debug messages in the chat and server log
#if !defined WC_DEBUG
	#define WC_DEBUG false
#endif

// Print debug messages to the console but not the chat
#if !defined WC_DEBUG_SILENT
	#define WC_DEBUG_SILENT false
#endif

// Max number of rejected hits (GetRejectedHit)
#if !defined WC_MAX_REJECTED_HITS
	#define WC_MAX_REJECTED_HITS 15
#endif

// Max ranges for DAMAGE_TYPE_RANGE(_MULTIPLIER)
#if !defined WC_MAX_DAMAGE_RANGES
	#define WC_MAX_DAMAGE_RANGES 5
#endif

// The world a player has after the death animation finished until he respawns or enters class selection
#if !defined WC_DEATH_WORLD
	#define WC_DEATH_WORLD 0x00DEAD00
#endif

// The max number of entries in the damage feeds
#if !defined WC_FEED_HEIGHT
	#define WC_FEED_HEIGHT 5
#endif

// Use Incognito's streamer for custom vending machines
#if !defined WC_USE_STREAMER
	#define WC_USE_STREAMER false
#endif

// For SetWeaponName
#if !defined WC_MAX_WEAPON_NAME
	#define WC_MAX_WEAPON_NAME 21
#endif

// For modes that always have negative cash
#if !defined WC_CUSTOM_VENDING_MACHINES
	#define WC_CUSTOM_VENDING_MACHINES true
#endif

#if !defined WC_HEALTH_BAR_FG_COLOR
	#define WC_HEALTH_BAR_FG_COLOR 0xB4191DFF //red
#endif

#if !defined WC_HEALTH_BAR_BG_COLOR
	#define WC_HEALTH_BAR_BG_COLOR 0x5A0C0EFF //dark red
#endif

#if WC_USE_STREAMER && !defined Streamer_IncludeFileVersion
	#error streamer.inc is required when WC_USE_STREAMER=true
#endif

// https://github.com/oscar-broman/SKY/blob/master/SKY.inc
#if !defined _INC_SKY
	#tryinclude <SKY>

	#if !defined _INC_SKY
		#error The SKY plugin is required, get it here: github.com/oscar-broman/sky
	#endif
#endif

// y_iterate and standalone foreach support
#if !defined _Y_ITERATE_LOCAL_VERSION && !defined _FOREACH_LOCAL_VERSION
	#tryinclude <YSI_Data\y_iterate>

	#if !defined _Y_ITERATE_LOCAL_VERSION
		#tryinclude <foreach>
	#endif
#endif

// Pre-hooks for hooking callbacks
#if !defined CHAIN_ORDER
	#define CHAIN_ORDER() 0
#endif

#define CHAIN_HOOK(%0) forward @CO_%0();public @CO_%0(){return CHAIN_ORDER()+1;}
#define CHAIN_NEXT(%0) @CO_%0

#define CHAIN_FORWARD:%0_%2(%1)=%3; \
	forward %0_%2(%1); \
	public %0_%2(%1) <_ALS : _ALS_x0, _ALS : _ALS_x1> { return (%3); } \
	public %0_%2(%1) <> { return (%3); }

#define CHAIN_PUBLIC:%0(%1) %0(%1) <_ALS : _ALS_go>

CHAIN_HOOK(WC)
#undef CHAIN_ORDER
#define CHAIN_ORDER CHAIN_NEXT(WC)

static stock _WC_IncludeStates() <_ALS : _ALS_x0, _ALS : _ALS_x1, _ALS : _ALS_x2, _ALS : _ALS_x3> {}
static stock _WC_IncludeStates() <_ALS : _ALS_go> {}

// Provides a way for const correctness support
// when used with the newer standard libraries
// https://github.com/sampctl/samp-stdlib/
// https://github.com/sampctl/pawn-stdlib
#if !defined SAMP_CONST_CORRECT
	#define WC_CONST
#else
	#define WC_CONST const
#endif

// Given in OnInvalidWeaponDamage
enum {
	WC_NO_ERROR,
	WC_NO_ISSUER,
	WC_NO_DAMAGED,
	WC_INVALID_DAMAGE,
	WC_INVALID_DISTANCE
}

// Used in SetWeaponDamage
enum {
	DAMAGE_TYPE_MULTIPLIER,
	DAMAGE_TYPE_STATIC,
	DAMAGE_TYPE_RANGE_MULTIPLIER,
	DAMAGE_TYPE_RANGE
}

// Hits displayed in the damage feeds
enum E_DAMAGE_FEED_HIT {
	      e_Issuer,
	      e_Name[MAX_PLAYER_NAME],
	Float:e_Amount,
	      e_Weapon,
	      e_Tick
}

// Given in OnRejectedHit
enum E_REJECTED_HIT {
	e_Time,
	e_Hour,
	e_Minute,
	e_Second,
	e_Weapon,
	e_Reason,
	e_Info1,
	e_Info2,
	e_Info3,
	e_Name[MAX_PLAYER_NAME]
}

// e_Reason in E_REJECTED_HIT
enum {
	HIT_NO_DAMAGEDID,
	HIT_INVALID_WEAPON,
	HIT_LAST_SHOT_INVALID,
	HIT_MULTIPLE_PLAYERS,
	HIT_MULTIPLE_PLAYERS_SHOTGUN,
	HIT_DYING_PLAYER,
	HIT_SAME_TEAM,
	HIT_UNSTREAMED,
	HIT_INVALID_HITTYPE,
	HIT_BEING_RESYNCED,
	HIT_NOT_SPAWNED,
	HIT_OUT_OF_RANGE,
	HIT_TOO_FAR_FROM_SHOT,
	SHOOTING_RATE_TOO_FAST,
	SHOOTING_RATE_TOO_FAST_MULTIPLE,
	HIT_RATE_TOO_FAST,
	HIT_RATE_TOO_FAST_MULTIPLE,
	HIT_TOO_FAR_FROM_ORIGIN,
	HIT_INVALID_DAMAGE,
	HIT_SAME_VEHICLE,
	HIT_OWN_VEHICLE,
	HIT_INVALID_VEHICLE,
	HIT_DISCONNECTED
}

// Must be in sync with the enum above
// Used in debug messages and GetRejectedHit
stock const g_HitRejectReasons[][] = {
	"None or invalid player shot",
	"Invalid weapon",
	"Last shot invalid",
	"One bullet hit %d players",
	"Hit too many players with shotgun: %d",
	"Hit a dying player",
	"Hit a teammate",
	"Hit someone that can't see you (not streamed in)",
	"Invalid hit type: %d",
	"Hit while being resynced",
	"Hit when not spawned or dying",
	"Hit out of range (%f > %f)",
	"Hit player too far from hit position (dist %f)",
	"Shooting rate too fast: %d (%d samples, max %d)",
	"Shooting rate too fast: %d (%d samples, multiple weapons)",
	"Hit rate too fast: %d (%d samples, max %d)",
	"Hit rate too fast: %d (%d samples, multiple weapons)",
	"Damage inflicted too far from current position (dist %f)",
	"Invalid weapon damage (%.4f)",
	"Hit a player in the same vehicle",
	"Hit the vehicle you're in",
	"Hit invalid vehicle: %d",
	"Hit a disconnected player ID: %d"
};

// Used for SetCustomVendingMachines
#if WC_CUSTOM_VENDING_MACHINES
	enum E_VENDING_MACHINE {
		      e_Model,
		      e_Interior,
		Float:e_PosX,
		Float:e_PosY,
		Float:e_PosZ,
		Float:e_RotX,
		Float:e_RotY,
		Float:e_RotZ,
		Float:e_FrontX,
		Float:e_FrontY
	}
#endif

// Used to resync players that got team-knifed in lagshot mode
enum E_RESYNC_DATA {
	Float:e_Health,
	Float:e_Armour,
	      e_Skin,
	      e_Team,
	Float:e_PosX,
	Float:e_PosY,
	Float:e_PosZ,
	Float:e_PosA,
	      e_Weapon,
	      e_WeaponId[13],
	      e_WeaponAmmo[13]
}

// From OnPlayerWeaponShot
enum E_SHOT_INFO {
	      e_Tick,
	      e_Weapon,
	      e_HitType,
	      e_HitId,
	      e_Hits,
	Float:e_X,
	Float:e_Y,
	Float:e_Z,
	Float:e_OX,
	Float:e_OY,
	Float:e_OZ,
	Float:e_HX,
	Float:e_HY,
	Float:e_HZ,
	Float:e_Length,
	 bool:e_Valid
}

enum E_HIT_INFO {
	      e_Tick,
	      e_Issuer,
	      e_Weapon,
	Float:e_Amount,
	Float:e_Health,
	Float:e_Armour,
	      e_Bodypart
}

enum E_SPAWN_INFO {
	      e_Skin,
	      e_Team,
	Float:e_PosX,
	Float:e_PosY,
	Float:e_PosZ,
	Float:e_Rot,
	      e_Weapon1,
	      e_Ammo1,
	      e_Weapon2,
	      e_Ammo2,
	      e_Weapon3,
	      e_Ammo3
}

// When a player takes or gives invalid damage (WC_* errors above)
forward OnInvalidWeaponDamage(playerid, damagedid, Float:amount, weaponid, bodypart, error, bool:given);
// Before damage is inflicted
forward OnPlayerDamage(&playerid, &Float:amount, &issuerid, &weapon, &bodypart);
// After OnPlayerDamage
forward OnPlayerDamageDone(playerid, Float:amount, issuerid, weapon, bodypart);
// Before the death animation is applied
forward OnPlayerPrepareDeath(playerid, animlib[32], animname[32], &anim_lock, &respawn_time);
// When the death animation is finished and the player has been sent to respawn
forward OnPlayerDeathFinished(playerid, bool:cancelable);
// When a shot or damage given is rejected
forward OnRejectedHit(playerid, hit[E_REJECTED_HIT]);

#if WC_CUSTOM_VENDING_MACHINES
	// When a player is about to use a vending machine
	forward OnPlayerUseVendingMachine(playerid, &Float:health_given);
#endif

// If you have your own definitions, remove them and use these instead
#define BODY_PART_UNKNOWN 0
#define WEAPON_UNARMED 0
#define WEAPON_VEHICLE_M4 19
#define WEAPON_VEHICLE_MINIGUN 20
#define WEAPON_VEHICLE_ROCKETLAUNCHER 21
#define WEAPON_PISTOLWHIP 48
#define WEAPON_HELIBLADES 50
#define WEAPON_EXPLOSION 51
#define WEAPON_CARPARK 52
#define WEAPON_UNKNOWN 55

#if WC_DEBUG_SILENT
	static s_DebugMsgBuf[512];

	#define DebugMessage(%1,%2) \
		format(s_DebugMsgBuf,512,%2),printf("(wc:%d) %s",%1,s_DebugMsgBuf)

	#define DebugMessageRed(%1,%2) \
		format(s_DebugMsgBuf,512,%2),printf("(wc:%d) WARN: %s",%1,s_DebugMsgBuf)

	#define DebugMessageAll(%1) \
		printf(s_DebugMsgBuf,"(wc) " %1)

	#define DebugMessageRedAll(%1) \
		printf(s_DebugMsgBuf,"(wc) WARN: " %1)
#elseif WC_DEBUG
	static s_DebugMsgBuf[512];

	#define DebugMessage(%1,%2) \
		format(s_DebugMsgBuf,512,"(wc) " %2),SendClientMessage(%1,-1,s_DebugMsgBuf), \
		format(s_DebugMsgBuf,512,%2),printf("(wc:%d) %s",%1,s_DebugMsgBuf)

	#define DebugMessageRed(%1,%2) \
		format(s_DebugMsgBuf,512,"(wc) " %2),SendClientMessage(%1,0xcc0000ff,s_DebugMsgBuf), \
		format(s_DebugMsgBuf,512,%2),printf("(wc:%d) WARN: %s",%1,s_DebugMsgBuf)

	#define DebugMessageAll(%1) \
		format(s_DebugMsgBuf,512,"(wc) " %1),SendClientMessageToAll(-1,s_DebugMsgBuf), \
		printf(s_DebugMsgBuf,"(wc) " %1)

	#define DebugMessageRedAll(%1) \
		format(s_DebugMsgBuf,512,"(wc) " %1),SendClientMessageToAll(0xcc0000ff,s_DebugMsgBuf), \
		printf(s_DebugMsgBuf,"(wc) WARN: " %1)
#else
	#define DebugMessage(%1);
	#define DebugMessageRed(%1);
	#define DebugMessageAll(%1);
	#define DebugMessageRedAll(%1);
#endif

// Weapons allowed in OnPlayerGiveDamage
static const s_ValidDamageGiven[] = {
	1, // 0 - Fist
	1, // 1 - Brass knuckles
	1, // 2 - Golf club
	1, // 3 - Nitestick
	1, // 4 - Knife
	1, // 5 - Bat
	1, // 6 - Shovel
	1, // 7 - Pool cue
	1, // 8 - Katana
	1, // 9 - Chainsaw
	1, // 10 - Dildo
	1, // 11 - Dildo 2
	1, // 12 - Vibrator
	1, // 13 - Vibrator 2
	1, // 14 - Flowers
	1, // 15 - Cane
	0, // 16 - Grenade
	0, // 17 - Teargas
	0, // 18 - Molotov
	0, // 19 - Vehicle M4 (custom)
	0, // 20 - Vehicle minigun (custom)
	0, // 21 - Vehicle rocket (custom)
	1, // 22 - Colt 45
	1, // 23 - Silenced
	1, // 24 - Deagle
	1, // 25 - Shotgun
	1, // 26 - Sawed-off
	1, // 27 - Spas
	1, // 28 - UZI
	1, // 29 - MP5
	1, // 30 - AK47
	1, // 31 - M4
	1, // 32 - Tec9
	1, // 33 - Cuntgun
	1, // 34 - Sniper
	0, // 35 - Rocket launcher
	0, // 36 - Heatseeker
	0, // 37 - Flamethrower
	1, // 38 - Minigun
	0, // 39 - Satchel
	0, // 40 - Detonator
	1, // 41 - Spraycan
	1, // 42 - Fire extinguisher
	0, // 43 - Camera
	0, // 44 - Night vision
	0, // 45 - Infrared
	1  // 46 - Parachute
};

// Weapons allowed in OnPlayerTakeDamage
// 2 = valid in both OnPlayerGiveDamage and OnPlayerTakeDamage
static const s_ValidDamageTaken[] = {
	1, // 0 - Fist
	1, // 1 - Brass knuckles
	1, // 2 - Golf club
	1, // 3 - Nitestick
	1, // 4 - Knife
	1, // 5 - Bat
	1, // 6 - Shovel
	1, // 7 - Pool cue
	1, // 8 - Katana
	1, // 9 - Chainsaw
	1, // 10 - Dildo
	1, // 11 - Dildo 2
	1, // 12 - Vibrator
	1, // 13 - Vibrator 2
	1, // 14 - Flowers
	1, // 15 - Cane
	0, // 16 - Grenade
	0, // 17 - Teargas
	0, // 18 - Molotov
	0, // 19 - Vehicle M4 (custom)
	0, // 20 - Vehicle minigun (custom)
	0, // 21 - Vehicle rocket (custom)
	1, // 22 - Colt 45
	1, // 23 - Silenced
	1, // 24 - Deagle
	1, // 25 - Shotgun
	1, // 26 - Sawed-off
	1, // 27 - Spas
	1, // 28 - UZI
	1, // 29 - MP5
	1, // 30 - AK47
	1, // 31 - M4
	1, // 32 - Tec9
	1, // 33 - Cuntgun
	1, // 34 - Sniper
	0, // 35 - Rocket launcher
	0, // 36 - Heatseeker
	2, // 37 - Flamethrower
	1, // 38 - Minigun
	0, // 39 - Satchel
	0, // 40 - Detonator
	1, // 41 - Spraycan
	1, // 42 - Fire extinguisher
	0, // 43 - Camera
	0, // 44 - Night vision
	0, // 45 - Infrared
	1, // 46 - Parachute
	0, // 47 - Fake pistol
	0, // 48 - Pistol whip (custom)
	2, // 49 - Vehicle
	2, // 50 - Helicopter blades
	2, // 51 - Explosion
	0, // 52 - Car park (custom)
	2, // 53 - Drowning
	2  // 54 - Splat
};

// Default weapon damage. Connected to s_DamageType.
// Melee weapons are multipliers because the damage differs
// depending on type of punch/kick and fight style.
static Float:s_WeaponDamage[] = {
	1.0, // 0 - Fist
	1.0, // 1 - Brass knuckles
	1.0, // 2 - Golf club
	1.0, // 3 - Nitestick
	1.0, // 4 - Knife
	1.0, // 5 - Bat
	1.0, // 6 - Shovel
	1.0, // 7 - Pool cue
	1.0, // 8 - Katana
	1.0, // 9 - Chainsaw
	1.0, // 10 - Dildo
	1.0, // 11 - Dildo 2
	1.0, // 12 - Vibrator
	1.0, // 13 - Vibrator 2
	1.0, // 14 - Flowers
	1.0, // 15 - Cane
	82.5, // 16 - Grenade
	0.0, // 17 - Teargas
	1.0, // 18 - Molotov
	9.9, // 19 - Vehicle M4 (custom)
	46.2, // 20 - Vehicle minigun (custom)
	82.5, // 21 - Vehicle rocket (custom)
	8.25, // 22 - Colt 45
	13.2, // 23 - Silenced
	34.0, // 24 - Deagle
	1.6, // 25 - Shotgun
	1.4, // 26 - Sawed-off
	2.0, // 27 - Spas
	4.0, // 28 - UZI
	8.25, // 29 - MP5
	15.0, // 30 - AK47
	12.0, // 31 - M4
	6.6, // 32 - Tec9
	24.75, // 33 - Cuntgun
	60.0, // 34 - Sniper
	82.5, // 35 - Rocket launcher
	82.5, // 36 - Heatseeker
	1.0, // 37 - Flamethrower
	46.2, // 38 - Minigun
	82.5, // 39 - Satchel
	0.0, // 40 - Detonator
	0.33, // 41 - Spraycan
	0.33, // 42 - Fire extinguisher
	0.0, // 43 - Camera
	0.0, // 44 - Night vision
	0.0, // 45 - Infrared
	0.0, // 46 - Parachute
	0.0, // 47 - Fake pistol
	2.64, // 48 - Pistol whip (custom)
	9.9, // 49 - Vehicle
	330.0, // 50 - Helicopter blades
	82.5, // 51 - Explosion
	1.0, // 52 - Car park (custom)
	1.0, // 53 - Drowning
	165.0  // 54 - Splat
};

#assert DAMAGE_TYPE_MULTIPLIER == 0
#assert DAMAGE_TYPE_STATIC == 1

// Whether the damage is multiplied by the given/taken value (0) or always the same value (1)
static s_DamageType[] = {
	0, // 0 - Fist
	0, // 1 - Brass knuckles
	0, // 2 - Golf club
	0, // 3 - Nitestick
	0, // 4 - Knife
	0, // 5 - Bat
	0, // 6 - Shovel
	0, // 7 - Pool cue
	0, // 8 - Katana
	0, // 9 - Chainsaw
	0, // 10 - Dildo
	0, // 11 - Dildo 2
	0, // 12 - Vibrator
	0, // 13 - Vibrator 2
	0, // 14 - Flowers
	0, // 15 - Cane
	0, // 16 - Grenade
	1, // 17 - Teargas
	0, // 18 - Molotov
	1, // 19 - Vehicle M4 (custom)
	1, // 20 - Vehicle minigun (custom)
	0, // 21 - Vehicle rocket (custom)
	1, // 22 - Colt 45
	1, // 23 - Silenced
	1, // 24 - Deagle
	1, // 25 - Shotgun
	1, // 26 - Sawed-off
	1, // 27 - Spas
	1, // 28 - UZI
	1, // 29 - MP5
	1, // 30 - AK47
	1, // 31 - M4
	1, // 32 - Tec9
	1, // 33 - Cuntgun
	1, // 34 - Sniper
	0, // 35 - Rocket launcher
	0, // 36 - Heatseeker
	0, // 37 - Flamethrower
	1, // 38 - Minigun
	0, // 39 - Satchel
	0, // 40 - Detonator
	1, // 41 - Spraycan
	1, // 42 - Fire extinguisher
	0, // 43 - Camera
	0, // 44 - Night vision
	0, // 45 - Infrared
	0, // 46 - Parachute
	0, // 47 - Fake pistol
	1, // 48 - Pistol whip (custom)
	1, // 49 - Vehicle
	1, // 50 - Helicopter blades
	0, // 51 - Explosion
	0, // 52 - Car park (custom)
	0, // 53 - Drowning
	0  // 54 - Splat
};

// The default weapon range (from weapon.dat)
// Note that due to various bugs, these can be exceeded, but
// this include blocks out-of-range values.
static Float:s_WeaponRange[] = {
	1.6, // 0 - Fist
	1.6, // 1 - Brass knuckles
	1.6, // 2 - Golf club
	1.6, // 3 - Nitestick
	1.6, // 4 - Knife
	1.6, // 5 - Bat
	1.6, // 6 - Shovel
	1.6, // 7 - Pool cue
	1.6, // 8 - Katana
	1.6, // 9 - Chainsaw
	1.6, // 10 - Dildo
	1.6, // 11 - Dildo 2
	1.6, // 12 - Vibrator
	1.6, // 13 - Vibrator 2
	1.6, // 14 - Flowers
	1.6, // 15 - Cane
	40.0, // 16 - Grenade
	40.0, // 17 - Teargas
	40.0, // 18 - Molotov
	90.0, // 19 - Vehicle M4 (custom)
	75.0, // 20 - Vehicle minigun (custom)
	0.0, // 21 - Vehicle rocket (custom)
	35.0, // 22 - Colt 45
	35.0, // 23 - Silenced
	35.0, // 24 - Deagle
	40.0, // 25 - Shotgun
	35.0, // 26 - Sawed-off
	40.0, // 27 - Spas
	35.0, // 28 - UZI
	45.0, // 29 - MP5
	70.0, // 30 - AK47
	90.0, // 31 - M4
	35.0, // 32 - Tec9
	100.0, // 33 - Cuntgun
	320.0, // 34 - Sniper
	55.0, // 35 - Rocket launcher
	55.0, // 36 - Heatseeker
	5.1, // 37 - Flamethrower
	75.0, // 38 - Minigun
	40.0, // 39 - Satchel
	25.0, // 40 - Detonator
	6.1, // 41 - Spraycan
	10.1, // 42 - Fire extinguisher
	100.0, // 43 - Camera
	100.0, // 44 - Night vision
	100.0, // 45 - Infrared
	1.6  // 46 - Parachute
};

// The fastest possible gap between weapon shots in milliseconds
static s_MaxWeaponShootRate[] = {
	250, // 0 - Fist
	250, // 1 - Brass knuckles
	250, // 2 - Golf club
	250, // 3 - Nitestick
	250, // 4 - Knife
	250, // 5 - Bat
	250, // 6 - Shovel
	250, // 7 - Pool cue
	250, // 8 - Katana
	30, // 9 - Chainsaw
	250, // 10 - Dildo
	250, // 11 - Dildo 2
	250, // 12 - Vibrator
	250, // 13 - Vibrator 2
	250, // 14 - Flowers
	250, // 15 - Cane
	0, // 16 - Grenade
	0, // 17 - Teargas
	0, // 18 - Molotov
	90, // 19 - Vehicle M4 (custom)
	20, // 20 - Vehicle minigun (custom)
	0, // 21 - Vehicle rocket (custom)
	160, // 22 - Colt 45
	120, // 23 - Silenced
	120, // 24 - Deagle
	800, // 25 - Shotgun
	120, // 26 - Sawed-off
	120, // 27 - Spas
	50, // 28 - UZI
	90, // 29 - MP5
	90, // 30 - AK47
	90, // 31 - M4
	70, // 32 - Tec9
	800, // 33 - Cuntgun
	900, // 34 - Sniper
	0, // 35 - Rocket launcher
	0, // 36 - Heatseeker
	0, // 37 - Flamethrower
	20, // 38 - Minigun
	0, // 39 - Satchel
	0, // 40 - Detonator
	10, // 41 - Spraycan
	10, // 42 - Fire extinguisher
	0, // 43 - Camera
	0, // 44 - Night vision
	0, // 45 - Infrared
	0, // 46 - Parachute
	0, // 47 - Fake pistol
	400 // 48 - Pistol whip (custom)
};

// Whether the damage is applied directly to health (1) or is distributed between health and armour (0), and whether this rule applies only to the torso (1) or not (0)
static s_DamageArmour[][2] = {
	{0, 0}, // 0 - Fist
	{0, 0}, // 1 - Brass knuckles
	{0, 0}, // 2 - Golf club
	{0, 0}, // 3 - Nitestick
	{0, 0}, // 4 - Knife
	{0, 0}, // 5 - Bat
	{0, 0}, // 6 - Shovel
	{0, 0}, // 7 - Pool cue
	{0, 0}, // 8 - Katana
	{0, 0}, // 9 - Chainsaw
	{0, 0}, // 10 - Dildo
	{0, 0}, // 11 - Dildo 2
	{0, 0}, // 12 - Vibrator
	{0, 0}, // 13 - Vibrator 2
	{0, 0}, // 14 - Flowers
	{0, 0}, // 15 - Cane
	{0, 0}, // 16 - Grenade
	{0, 0}, // 17 - Teargas
	{0, 0}, // 18 - Molotov
	{1, 1}, // 19 - Vehicle M4 (custom)
	{1, 1}, // 20 - Vehicle minigun (custom)
	{0, 0}, // 21 - Vehicle rocket (custom)
	{1, 1}, // 22 - Colt 45
	{1, 1}, // 23 - Silenced
	{1, 1}, // 24 - Deagle
	{1, 1}, // 25 - Shotgun
	{1, 1}, // 26 - Sawed-off
	{1, 1}, // 27 - Spas
	{1, 1}, // 28 - UZI
	{1, 1}, // 29 - MP5
	{1, 1}, // 30 - AK47
	{1, 1}, // 31 - M4
	{1, 1}, // 32 - Tec9
	{1, 1}, // 33 - Cuntgun
	{1, 1}, // 34 - Sniper
	{0, 0}, // 35 - Rocket launcher
	{0, 0}, // 36 - Heatseeker
	{0, 0}, // 37 - Flamethrower
	{1, 1}, // 38 - Minigun
	{0, 0}, // 39 - Satchel
	{0, 0}, // 40 - Detonator
	{0, 0}, // 41 - Spraycan
	{0, 0}, // 42 - Fire extinguisher
	{1, 0}, // 43 - Camera
	{1, 0}, // 44 - Night vision
	{1, 0}, // 45 - Infrared
	{1, 0}, // 46 - Parachute
	{1, 0}, // 47 - Fake pistol
	{0, 0}, // 48 - Pistol whip (custom)
	{0, 0}, // 49 - Vehicle
	{0, 1}, // 50 - Helicopter blades
	{0, 0}, // 51 - Explosion
	{0, 0}, // 52 - Car park (custom)
	{0, 0}, // 53 - Drowning
	{0, 0}  // 54 - Splat
};

// That's right, it's called cuntgun
stock const g_WeaponName[57][WC_MAX_WEAPON_NAME] = {
	{"Soco"             }, {"Soco ingles"   }, {"Golf club"           },
	{"Cacetete"         }, {"Faca"          }, {"Bat"                 },
	{"Pa"               }, {"Pool cue"      }, {"Katana"              },
	{"Chainsaw"         }, {"Purple dildo"  }, {"Dildo"               },
	{"Vibrador"         }, {"Vibrator"      }, {"Flowers"             },
	{"Cano"             }, {"Grenade"       }, {"Tear gas"            },
	{"Molotov"          }, {"Arma de veiculo"}, {"Arma de veiculo"         },
	{"Arma de veiculo"  }, {"Colt 45"       }, {"Silenced pistol"     },
	{"Deagle"           }, {"Shotgun"       }, {"Sawn-off shotgun"    },
	{"Combat shotgun"   }, {"Mac-10"        }, {"MP5"                 },
	{"AK-47"            }, {"M4"            }, {"Tec-9"               },
	{"Cuntgun"          }, {"Sniper"        }, {"Rocket launcher"     },
	{"Heat seeking RPG" }, {"Flamethrower"  }, {"Minigun"             },
	{"Satchel"          }, {"Detonator"     }, {"Spraycan"            },
	{"Extintor"         }, {"Camera"        }, {"Night vision goggles"},
	{"Infrared goggles" }, {"Parachute"     }, {"Fake pistol"         },
	{"Pistol whip"      }, {"Veiculo"       }, {"Helicopter blades"   },
	{"Explosao"         }, {"Car parking"   }, {"Drowning"            },
	{"Colisao"          }, {"Splat"         }, {"Unknown"             }
};

// Yes - this is every single one of them
#if WC_CUSTOM_VENDING_MACHINES
	static const Float:sc_VendingMachines[][E_VENDING_MACHINE] = {
		{955, 0, -862.82, 1536.60, 21.98, 0.00, 0.00, 180.00, -862.84, 1537.60},
		{956, 0, 2271.72, -76.46, 25.96, 0.00, 0.00, 0.00, 2271.72, -77.46},
		{955, 0, 1277.83, 372.51, 18.95, 0.00, 0.00, 64.00, 1278.73, 372.07},
		{956, 0, 662.42, -552.16, 15.71, 0.00, 0.00, 180.00, 662.41, -551.16},
		{955, 0, 201.01, -107.61, 0.89, 0.00, 0.00, 270.00, 200.01, -107.63},
		{955, 0, -253.74, 2597.95, 62.24, 0.00, 0.00, 90.00, -252.74, 2597.95},
		{956, 0, -253.74, 2599.75, 62.24, 0.00, 0.00, 90.00, -252.74, 2599.75},
		{956, 0, -76.03, 1227.99, 19.12, 0.00, 0.00, 90.00, -75.03, 1227.99},
		{955, 0, -14.70, 1175.35, 18.95, 0.00, 0.00, 180.00, -14.72, 1176.35},
		{1977, 7, 316.87, -140.35, 998.58, 0.00, 0.00, 270.00, 315.87, -140.36},
		{1775, 17, 373.82, -178.14, 1000.73, 0.00, 0.00, 0.00, 373.82, -179.14},
		{1776, 17, 379.03, -178.88, 1000.73, 0.00, 0.00, 270.00, 378.03, -178.90},
		{1775, 17, 495.96, -24.32, 1000.73, 0.00, 0.00, 180.00, 495.95, -23.32},
		{1776, 17, 500.56, -1.36, 1000.73, 0.00, 0.00, 0.00, 500.56, -2.36},
		{1775, 17, 501.82, -1.42, 1000.73, 0.00, 0.00, 0.00, 501.82, -2.42},
		{956, 0, -1455.11, 2591.66, 55.23, 0.00, 0.00, 180.00, -1455.13, 2592.66},
		{955, 0, 2352.17, -1357.15, 23.77, 0.00, 0.00, 90.00, 2353.17, -1357.15},
		{955, 0, 2325.97, -1645.13, 14.21, 0.00, 0.00, 0.00, 2325.97, -1646.13},
		{956, 0, 2139.51, -1161.48, 23.35, 0.00, 0.00, 87.00, 2140.51, -1161.53},
		{956, 0, 2153.23, -1016.14, 62.23, 0.00, 0.00, 127.00, 2154.03, -1015.54},
		{955, 0, 1928.73, -1772.44, 12.94, 0.00, 0.00, 90.00, 1929.73, -1772.44},
		{1776, 1, 2222.36, 1602.64, 1000.06, 0.00, 0.00, 90.00, 2223.36, 1602.64},
		{1775, 1, 2222.20, 1606.77, 1000.05, 0.00, 0.00, 90.00, 2223.20, 1606.77},
		{1775, 1, 2155.90, 1606.77, 1000.05, 0.00, 0.00, 90.00, 2156.90, 1606.77},
		{1775, 1, 2209.90, 1607.19, 1000.05, 0.00, 0.00, 270.00, 2208.90, 1607.17},
		{1776, 1, 2155.84, 1607.87, 1000.06, 0.00, 0.00, 90.00, 2156.84, 1607.87},
		{1776, 1, 2202.45, 1617.00, 1000.06, 0.00, 0.00, 180.00, 2202.43, 1618.00},
		{1776, 1, 2209.24, 1621.21, 1000.06, 0.00, 0.00, 0.00, 2209.24, 1620.21},
		{1776, 3, 330.67, 178.50, 1020.07, 0.00, 0.00, 0.00, 330.67, 177.50},
		{1776, 3, 331.92, 178.50, 1020.07, 0.00, 0.00, 0.00, 331.92, 177.50},
		{1776, 3, 350.90, 206.08, 1008.47, 0.00, 0.00, 90.00, 351.90, 206.08},
		{1776, 3, 361.56, 158.61, 1008.47, 0.00, 0.00, 180.00, 361.54, 159.61},
		{1776, 3, 371.59, 178.45, 1020.07, 0.00, 0.00, 0.00, 371.59, 177.45},
		{1776, 3, 374.89, 188.97, 1008.47, 0.00, 0.00, 0.00, 374.89, 187.97},
		{1775, 2, 2576.70, -1284.43, 1061.09, 0.00, 0.00, 270.00, 2575.70, -1284.44},
		{1775, 15, 2225.20, -1153.42, 1025.90, 0.00, 0.00, 270.00, 2224.20, -1153.43},
		{955, 0, 1154.72, -1460.89, 15.15, 0.00, 0.00, 270.00, 1153.72, -1460.90},
		{956, 0, 2480.85, -1959.27, 12.96, 0.00, 0.00, 180.00, 2480.84, -1958.27},
		{955, 0, 2060.11, -1897.64, 12.92, 0.00, 0.00, 0.00, 2060.11, -1898.64},
		{955, 0, 1729.78, -1943.04, 12.94, 0.00, 0.00, 0.00, 1729.78, -1944.04},
		{956, 0, 1634.10, -2237.53, 12.89, 0.00, 0.00, 0.00, 1634.10, -2238.53},
		{955, 0, 1789.21, -1369.26, 15.16, 0.00, 0.00, 270.00, 1788.21, -1369.28},
		{956, 0, -2229.18, 286.41, 34.70, 0.00, 0.00, 180.00, -2229.20, 287.41},
		{955, 256, -1980.78, 142.66, 27.07, 0.00, 0.00, 270.00, -1981.78, 142.64},
		{955, 256, -2118.96, -423.64, 34.72, 0.00, 0.00, 255.00, -2119.93, -423.40},
		{955, 256, -2118.61, -422.41, 34.72, 0.00, 0.00, 255.00, -2119.58, -422.17},
		{955, 256, -2097.27, -398.33, 34.72, 0.00, 0.00, 180.00, -2097.29, -397.33},
		{955, 256, -2092.08, -490.05, 34.72, 0.00, 0.00, 0.00, -2092.08, -491.05},
		{955, 256, -2063.27, -490.05, 34.72, 0.00, 0.00, 0.00, -2063.27, -491.05},
		{955, 256, -2005.64, -490.05, 34.72, 0.00, 0.00, 0.00, -2005.64, -491.05},
		{955, 256, -2034.46, -490.05, 34.72, 0.00, 0.00, 0.00, -2034.46, -491.05},
		{955, 256, -2068.56, -398.33, 34.72, 0.00, 0.00, 180.00, -2068.58, -397.33},
		{955, 256, -2039.85, -398.33, 34.72, 0.00, 0.00, 180.00, -2039.86, -397.33},
		{955, 256, -2011.14, -398.33, 34.72, 0.00, 0.00, 180.00, -2011.15, -397.33},
		{955, 2048, -1350.11, 492.28, 10.58, 0.00, 0.00, 90.00, -1349.11, 492.28},
		{956, 2048, -1350.11, 493.85, 10.58, 0.00, 0.00, 90.00, -1349.11, 493.85},
		{955, 0, 2319.99, 2532.85, 10.21, 0.00, 0.00, 0.00, 2319.99, 2531.85},
		{956, 0, 2845.72, 1295.04, 10.78, 0.00, 0.00, 0.00, 2845.72, 1294.04},
		{955, 0, 2503.14, 1243.69, 10.21, 0.00, 0.00, 180.00, 2503.12, 1244.69},
		{956, 0, 2647.69, 1129.66, 10.21, 0.00, 0.00, 0.00, 2647.69, 1128.66},
		{1209, 0, -2420.21, 984.57, 44.29, 0.00, 0.00, 90.00, -2419.21, 984.57},
		{1302, 0, -2420.17, 985.94, 44.29, 0.00, 0.00, 90.00, -2419.17, 985.94},
		{955, 0, 2085.77, 2071.35, 10.45, 0.00, 0.00, 90.00, 2086.77, 2071.35},
		{956, 0, 1398.84, 2222.60, 10.42, 0.00, 0.00, 180.00, 1398.82, 2223.60},
		{956, 0, 1659.46, 1722.85, 10.21, 0.00, 0.00, 0.00, 1659.46, 1721.85},
		{955, 0, 1520.14, 1055.26, 10.00, 0.00, 0.00, 270.00, 1519.14, 1055.24},
		{1775, 6, -19.03, -57.83, 1003.63, 0.00, 0.00, 180.00, -19.05, -56.83},
		{1775, 18, -16.11, -91.64, 1003.63, 0.00, 0.00, 180.00, -16.13, -90.64},
		{1775, 16, -15.10, -140.22, 1003.63, 0.00, 0.00, 180.00, -15.11, -139.22},
		{1775, 17, -32.44, -186.69, 1003.63, 0.00, 0.00, 180.00, -32.46, -185.69},
		{1775, 16, -35.72, -140.22, 1003.63, 0.00, 0.00, 180.00, -35.74, -139.22},
		{1776, 6, -36.14, -57.87, 1003.63, 0.00, 0.00, 180.00, -36.16, -56.87},
		{1776, 18, -17.54, -91.71, 1003.63, 0.00, 0.00, 180.00, -17.56, -90.71},
		{1776, 16, -16.53, -140.29, 1003.63, 0.00, 0.00, 180.00, -16.54, -139.29},
		{1776, 17, -33.87, -186.76, 1003.63, 0.00, 0.00, 180.00, -33.89, -185.76}
	};
#endif

// Sorry about the mess..
static s_LagCompMode;
static s_LastExplosive[MAX_PLAYERS];
static s_LastShot[MAX_PLAYERS][E_SHOT_INFO];
static s_LastShotTicks[MAX_PLAYERS][10];
static s_LastShotWeapons[MAX_PLAYERS][10];
static s_LastShotIdx[MAX_PLAYERS];
static s_LastHitTicks[MAX_PLAYERS][10];
static s_LastHitWeapons[MAX_PLAYERS][10];
static s_LastHitIdx[MAX_PLAYERS];
static s_ShotsFired[MAX_PLAYERS];
static s_HitsIssued[MAX_PLAYERS];
static s_MaxShootRateSamples = 3;
static s_MaxHitRateSamples = 3;
static Float:s_PlayerMaxHealth[MAX_PLAYERS] = {100.0, ...};
static Float:s_PlayerHealth[MAX_PLAYERS] = {100.0, ...};
static Float:s_PlayerMaxArmour[MAX_PLAYERS] = {100.0, ...};
static Float:s_PlayerArmour[MAX_PLAYERS] = {0.0, ...};
static s_LastSentHealth[MAX_PLAYERS];
static s_LastSentArmour[MAX_PLAYERS];
static bool:s_DamageArmourToggle[2] = {false, ...};
static s_PlayerTeam[MAX_PLAYERS] = {NO_TEAM, ...};
static s_IsDying[MAX_PLAYERS];
static s_DeathTimer[MAX_PLAYERS] = {-1, ...};
static bool:s_HealthBarVisible[MAX_PLAYERS];
static bool:s_SpawnForStreamedIn[MAX_PLAYERS];
static s_RespawnTime = 3000;
static s_CustomFallDamage = false;
static bool:s_CbugGlobal = true;
static bool:s_CbugAllowed[MAX_PLAYERS] = true;
static s_CbugFroze[MAX_PLAYERS];
static s_DamageFeed = true;
static s_DamageFeedPlayer[MAX_PLAYERS] = {-1, ...};
static s_DamageFeedHideDelay = 3000;
static s_DamageFeedMaxUpdateRate = 250;
static s_VehiclePassengerDamage = false;
static s_VehicleUnoccupiedDamage = false;
static Float:s_FallDeathVelocity = -0.6;
static s_DamageTakenSound[MAX_PLAYERS] = 1190;
static s_DamageGivenSound[MAX_PLAYERS] = 17802;
static s_RejectedHits[MAX_PLAYERS][WC_MAX_REJECTED_HITS][E_REJECTED_HIT];
static s_RejectedHitsIdx[MAX_PLAYERS];
static s_World[MAX_PLAYERS];
static s_LastAnim[MAX_PLAYERS] = {-1, ...};
static Float:s_LastZVelo[MAX_PLAYERS] = {0.0, ...};
static Float:s_LastZ[MAX_PLAYERS] = {0.0, ...};
static s_LastUpdate[MAX_PLAYERS] = {-1, ...};
static PlayerText:s_DamageFeedTaken[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
static PlayerText:s_DamageFeedGiven[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
static s_DamageFeedHitsGiven[MAX_PLAYERS][WC_FEED_HEIGHT][E_DAMAGE_FEED_HIT];
static s_DamageFeedHitsTaken[MAX_PLAYERS][WC_FEED_HEIGHT][E_DAMAGE_FEED_HIT];
static s_DamageFeedTimer[MAX_PLAYERS] = {-1, ...};
static s_DamageFeedLastUpdate[MAX_PLAYERS];
static s_Spectating[MAX_PLAYERS] = {INVALID_PLAYER_ID, ...};
static s_LastStop[MAX_PLAYERS];
static bool:s_FirstSpawn[MAX_PLAYERS] = {true, ...};
#if WC_CUSTOM_VENDING_MACHINES
	static bool:s_CustomVendingMachines = true;
	#if WC_USE_STREAMER
		static s_VendingMachineObject[sizeof(sc_VendingMachines)] = {INVALID_STREAMER_ID, ...};
	#else
		static s_VendingMachineObject[sizeof(sc_VendingMachines)] = {-1, ...};
	#endif
	static s_VendingUseTimer[MAX_PLAYERS] = {-1, ...};
#endif
static s_BeingResynced[MAX_PLAYERS];
static s_KnifeTimeout[MAX_PLAYERS] = {-1, ...};
static s_SyncData[MAX_PLAYERS][E_RESYNC_DATA];
static Text:s_HealthBarBorder = Text:INVALID_TEXT_DRAW;
static Text:s_HealthBarBackground[2] = {Text:INVALID_TEXT_DRAW, Text:INVALID_TEXT_DRAW};
static PlayerText:s_HealthBarForeground[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
static s_DamageRangeSteps[55];
static Float:s_DamageRangeRanges[55][WC_MAX_DAMAGE_RANGES];
static Float:s_DamageRangeValues[55][WC_MAX_DAMAGE_RANGES];
static s_LastVehicleShooter[MAX_VEHICLES + 1] = {INVALID_PLAYER_ID, ...};
static bool:s_InternalTextDraw[Text:MAX_TEXT_DRAWS];
static bool:s_InternalPlayerTextDraw[MAX_PLAYERS][PlayerText:MAX_PLAYER_TEXT_DRAWS];
static s_LastVehicleEnterTime[MAX_PLAYERS];
static s_TrueDeath[MAX_PLAYERS];
static s_InClassSelection[MAX_PLAYERS];
static s_ForceClassSelection[MAX_PLAYERS];
static s_ClassSpawnInfo[320][E_SPAWN_INFO];
static s_PlayerSpawnInfo[MAX_PLAYERS][E_SPAWN_INFO];
static s_PlayerFallbackSpawnInfo[MAX_PLAYERS][E_SPAWN_INFO];
static s_PlayerClass[MAX_PLAYERS] = {-2, ...};
static bool:s_SpawnInfoModified[MAX_PLAYERS];
static bool:s_AlreadyConnected[MAX_PLAYERS];
static s_DeathSkip[MAX_PLAYERS];
static s_DeathSkipTick[MAX_PLAYERS];
static s_LastDeathTick[MAX_PLAYERS];
static s_LastVehicleTick[MAX_PLAYERS];
static s_PreviousHits[MAX_PLAYERS][10][E_HIT_INFO];
static s_PreviousHitI[MAX_PLAYERS];
static Float:s_DamageDoneHealth[MAX_PLAYERS];
static Float:s_DamageDoneArmour[MAX_PLAYERS];
static s_DelayedDeathTimer[MAX_PLAYERS] = {-1, ...};
static bool:s_VehicleAlive[MAX_VEHICLES] = {false, ...};
static bool:s_EnableHealthBar[MAX_PLAYERS];
static s_VehicleRespawnTimer[MAX_VEHICLES] = {-1, ...};

native WC_IsValidVehicle(vehicleid) = IsValidVehicle;

/*
 * Public API
 */
stock IsBulletWeapon(weaponid)
{
	return (WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN;
}

stock IsHighRateWeapon(weaponid)
{
	switch (weaponid) {
		case WEAPON_FLAMETHROWER, WEAPON_SPRAYCAN, WEAPON_FIREEXTINGUISHER,
		     WEAPON_CARPARK, WEAPON_DROWN: {
			return true;
		}
	}

	return false;
}

stock IsMeleeWeapon(weaponid)
{
	return (WEAPON_UNARMED <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP;
}

stock WC_IsPlayerSpawned(playerid)
{
	if (s_IsDying[playerid] || s_BeingResynced[playerid]) {
		return false;
	}

	switch (GetPlayerState(playerid)) {
		case PLAYER_STATE_ONFOOT .. PLAYER_STATE_PASSENGER,
		     PLAYER_STATE_SPAWNED: {
			return true;
		}
	}

	return false;
}

stock AverageShootRate(playerid, shots, &multiple_weapons = 0)
{
	if (playerid == INVALID_PLAYER_ID || s_ShotsFired[playerid] < shots) {
		return -1;
	}

	new total = 0, idx = s_LastShotIdx[playerid];

	multiple_weapons = false;

	for (new i = shots - 2, prev, prev_weap, prev_idx, this_idx; i >= 0; i--) {
		prev_idx = (idx - i - 1) % sizeof(s_LastShotTicks[]);

		// JIT plugin fix
		if (prev_idx < 0) {
			prev_idx += sizeof(s_LastShotTicks[]);
		}

		prev = s_LastShotTicks[playerid][prev_idx];
		prev_weap = s_LastShotWeapons[playerid][prev_idx];
		this_idx = (idx - i) % sizeof(s_LastShotTicks[]);

		// JIT plugin fix
		if (this_idx < 0) {
			this_idx += sizeof(s_LastShotTicks[]);
		}

		if (prev_weap != s_LastShotWeapons[playerid][this_idx]) {
			multiple_weapons = true;
		}

		total += s_LastShotTicks[playerid][this_idx] - prev;
	}

	return shots == 1 ? 1 : (total / (shots - 1));
}

stock AverageHitRate(playerid, hits, &multiple_weapons = 0)
{
	if (playerid == INVALID_PLAYER_ID || s_HitsIssued[playerid] < hits) {
		return -1;
	}

	new total = 0, idx = s_LastHitIdx[playerid];

	multiple_weapons = false;

	for (new i = hits - 2, prev, prev_weap, prev_idx, this_idx; i >= 0; i--) {
		prev_idx = (idx - i - 1) % sizeof(s_LastHitTicks[]);

		// JIT plugin fix
		if (prev_idx < 0) {
			prev_idx += sizeof(s_LastHitTicks[]);
		}

		prev = s_LastHitTicks[playerid][prev_idx];
		prev_weap = s_LastHitWeapons[playerid][prev_idx];
		this_idx = (idx - i) % sizeof(s_LastHitTicks[]);

		// JIT plugin fix
		if (this_idx < 0) {
			this_idx += sizeof(s_LastHitTicks[]);
		}

		if (prev_weap != s_LastHitWeapons[playerid][this_idx]) {
			multiple_weapons = true;
		}

		total += s_LastHitTicks[playerid][this_idx] - prev;
	}

	return hits == 1 ? 1 : (total / (hits - 1));
}

stock SetRespawnTime(ms)
{
	s_RespawnTime = max(0, ms);
}

stock GetRespawnTime()
{
	return s_RespawnTime;
}

stock ReturnWeaponName(weaponid)
{
	new name[sizeof(g_WeaponName[])];

	WC_GetWeaponName(weaponid, name);

	return name;
}

stock EnableHealthBarForPlayer(playerid, bool:enable)
{
	if (IsPlayerConnected(playerid))
	{
		s_EnableHealthBar[playerid] = enable;
		SetHealthBarVisible(playerid, enable);
		#if WC_DEBUG
		DebugMessage(playerid, "health bar is %s for player", (s_EnableHealthBar[playerid]) ? ("enabled") : ("disabled"));
		#endif
		return true;
	}
	return false;
}

stock SetWeaponDamage(weaponid, damage_type, Float:amount, Float:...)
{
	if (weaponid < WEAPON_UNARMED || weaponid >= sizeof(s_WeaponDamage)) {
		return 0;
	}

	if (damage_type == DAMAGE_TYPE_RANGE || damage_type == DAMAGE_TYPE_RANGE_MULTIPLIER) {
		if (!IsBulletWeapon(weaponid)) {
			return 0;
		}

		new args = numargs();

		if (!(args & 0b1)) {
			return 0;
		}

		new steps = (args - 1) / 2;

		s_DamageType[weaponid] = damage_type;
		s_DamageRangeSteps[weaponid] = steps;

		for (new i = 0; i < steps; i++) {
			if (i) {
				s_DamageRangeRanges[weaponid][i] = Float:getarg(1 + i*2);
				s_DamageRangeValues[weaponid][i] = Float:getarg(2 + i*2);
			} else {
				s_DamageRangeValues[weaponid][i] = amount;
			}
		}

		return 1;
	} else if (damage_type == DAMAGE_TYPE_MULTIPLIER || damage_type == DAMAGE_TYPE_STATIC) {
		s_DamageType[weaponid] = damage_type;
		s_DamageRangeSteps[weaponid] = 0;
		s_WeaponDamage[weaponid] = amount;

		return 1;
	}

	return 0;
}

stock Float:GetWeaponDamage(weaponid)
{
	if (weaponid < WEAPON_UNARMED || weaponid >= sizeof(s_WeaponDamage)) {
		return 0.0;
	}

	return s_WeaponDamage[weaponid];
}

stock SetCustomArmourRules(bool:armour_rules, bool:torso_rules = false)
{
	s_DamageArmourToggle[0] = armour_rules;
	s_DamageArmourToggle[1] = torso_rules;
}

stock SetWeaponArmourRule(weaponid, bool:affects_armour, bool:torso_only = false)
{
	if (weaponid < WEAPON_UNARMED || weaponid >= sizeof(s_WeaponDamage)) {
		return 0;
	}

	s_DamageArmour[weaponid][0] = affects_armour;
	s_DamageArmour[weaponid][1] = torso_only;

	return 1;
}

stock SetDamageSounds(playerid, taken, given)
{
	s_DamageTakenSound[playerid] = taken;
	s_DamageGivenSound[playerid] = given;
}

stock SetCbugAllowed(bool:enabled, playerid = INVALID_PLAYER_ID)
{
	if (playerid == INVALID_PLAYER_ID) {
		s_CbugGlobal = enabled;
		#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
		foreach (new i : Player) {
		#else
		for (new i = GetPlayerPoolSize(); i != -1; i--) {
		#endif
			s_CbugAllowed[i] = enabled;
		}
	} else {
		s_CbugAllowed[playerid] = enabled;
	}

	return enabled;
}

stock bool:GetCbugAllowed(playerid = INVALID_PLAYER_ID)
{
	if (playerid == INVALID_PLAYER_ID) {
		return s_CbugGlobal;
	}

	return s_CbugAllowed[playerid];
}

stock SetCustomFallDamage(bool:toggle, Float:damage_multiplier = 25.0, Float:death_velocity = -0.6)
{
	s_CustomFallDamage = toggle;

	if (toggle) {
		s_WeaponDamage[WEAPON_COLLISION] = damage_multiplier;
		s_FallDeathVelocity = -floatabs(death_velocity);
	}
}

#if WC_CUSTOM_VENDING_MACHINES
	stock SetCustomVendingMachines(bool:toggle)
	{
		s_CustomVendingMachines = toggle;

		if (toggle) {
			CreateVendingMachines();
		} else {
			DestroyVendingMachines();
		}
	}
#endif

stock SetVehiclePassengerDamage(bool:toggle)
{
	s_VehiclePassengerDamage = toggle;
}

stock SetVehicleUnoccupiedDamage(bool:toggle)
{
	s_VehicleUnoccupiedDamage = toggle;
}

stock SetDamageFeedForPlayer(playerid, toggle = -1)
{
	if (IsPlayerConnected(playerid))
	{
		s_DamageFeedPlayer[playerid] = toggle;
		DamageFeedUpdate(playerid);
		return 1;
	}

	return 0;
}

stock IsDamageFeedActive(playerid = -1)
{
	if (playerid != -1) {
		return s_DamageFeedPlayer[playerid] == 1 || s_DamageFeed && s_DamageFeedPlayer[playerid] != 0;
	}

	return s_DamageFeed;
}

stock SetDamageFeed(bool:toggle)
{
	s_DamageFeed = toggle;

	#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
	foreach (new i : Player) {
	#else
	for (new i = GetPlayerPoolSize(); i != -1; i--) {
	#endif
		DamageFeedUpdate(i);
	}
}

stock SetWeaponShootRate(weaponid, max_rate)
{
	if (WEAPON_UNARMED <= weaponid < sizeof(s_MaxWeaponShootRate)) {
		s_MaxWeaponShootRate[weaponid] = max_rate;

		return 1;
	}

	return 0;
}

stock GetWeaponShootRate(weaponid)
{
	if (WEAPON_UNARMED <= weaponid < sizeof(s_MaxWeaponShootRate)) {
		return s_MaxWeaponShootRate[weaponid];
	}

	return 0;
}

stock IsPlayerDying(playerid)
{
	if (0 <= playerid < MAX_PLAYERS) {
		return s_IsDying[playerid];
	}

	return false;
}

stock SetWeaponMaxRange(weaponid, Float:range)
{
	if (!IsBulletWeapon(weaponid)) {
		return 0;
	}

	s_WeaponRange[weaponid] = range;

	return 1;
}

stock Float:GetWeaponMaxRange(weaponid)
{
	if (!IsBulletWeapon(weaponid)) {
		return 0.0;
	}

	return s_WeaponRange[weaponid];
}

stock SetPlayerMaxHealth(playerid, Float:value)
{
	if (0 <= playerid < MAX_PLAYERS) {
		s_PlayerMaxHealth[playerid] = value;
	}
}

stock SetPlayerMaxArmour(playerid, Float:value)
{
	if (0 <= playerid < MAX_PLAYERS) {
		s_PlayerMaxArmour[playerid] = value;
	}
}

stock Float:GetPlayerMaxHealth(playerid)
{
	if (0 <= playerid < MAX_PLAYERS) {
		return s_PlayerMaxHealth[playerid];
	}

	return 0.0;
}

stock Float:GetPlayerMaxArmour(playerid)
{
	if (0 <= playerid < MAX_PLAYERS) {
		return s_PlayerMaxArmour[playerid];
	}

	return 0.0;
}

stock Float:GetLastDamageHealth(playerid)
{
	if (0 <= playerid < MAX_PLAYERS) {
		return s_DamageDoneHealth[playerid];
	}

	return 0.0;
}

stock Float:GetLastDamageArmour(playerid)
{
	if (0 <= playerid < MAX_PLAYERS) {
		return s_DamageDoneArmour[playerid];
	}

	return 0.0;
}

stock DamagePlayer(playerid, Float:amount, issuerid = INVALID_PLAYER_ID, weaponid = WEAPON_UNKNOWN, bodypart = BODY_PART_UNKNOWN, bool:ignore_armour = false)
{
	if (playerid < 0 || playerid > MAX_PLAYERS || !IsPlayerConnected(playerid)) {
		return 0;
	}

	if (amount < 0.0) {
		return 0;
	}

	if (weaponid < WEAPON_UNARMED || weaponid > WEAPON_UNKNOWN) {
		weaponid = WEAPON_UNKNOWN;
	}

	if (issuerid < 0 || issuerid > MAX_PLAYERS || !IsPlayerConnected(issuerid)) {
		issuerid = INVALID_PLAYER_ID;
	}

	InflictDamage(playerid, amount, issuerid, weaponid, bodypart, ignore_armour);

	return 1;
}

stock GetRejectedHit(playerid, idx, output[], maxlength = sizeof(output))
{
	if (idx >= WC_MAX_REJECTED_HITS) {
		return 0;
	}

	new real_idx = (s_RejectedHitsIdx[playerid] - idx) % WC_MAX_REJECTED_HITS;

	// JIT plugin fix
	if (real_idx < 0) {
		real_idx += WC_MAX_REJECTED_HITS;
	}

	if (!s_RejectedHits[playerid][real_idx][e_Time]) {
		return 0;
	}

	new reason = s_RejectedHits[playerid][real_idx][e_Reason];
	new hour = s_RejectedHits[playerid][real_idx][e_Hour];
	new minute = s_RejectedHits[playerid][real_idx][e_Minute];
	new second = s_RejectedHits[playerid][real_idx][e_Second];
	new i1 = s_RejectedHits[playerid][real_idx][e_Info1];
	new i2 = s_RejectedHits[playerid][real_idx][e_Info2];
	new i3 = s_RejectedHits[playerid][real_idx][e_Info3];
	new weapon = s_RejectedHits[playerid][real_idx][e_Weapon];

	new weapon_name[32];

	WC_GetWeaponName(weapon, weapon_name);

	format(output, maxlength, "[%02d:%02d:%02d] (%s -> %s) %s", hour, minute, second, weapon_name, s_RejectedHits[playerid][real_idx][e_Name], g_HitRejectReasons[reason]);

	format(output, maxlength, output, i1, i2, i3);

	return 1;
}

stock ResyncPlayer(playerid)
{
	SaveSyncData(playerid);

	s_BeingResynced[playerid] = true;

	SpawnPlayerInPlace(playerid);
}

/*
 * Hooked natives
 */

stock WC_SpawnPlayer(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || s_IsDying[playerid]) {
		return 0;
	}

	if (s_PlayerHealth[playerid] == 0.0) {
		s_PlayerHealth[playerid] = s_PlayerMaxHealth[playerid];
	}

	SpawnPlayer(playerid);

	return 1;
}

stock WC_GetPlayerState(playerid)
{
	if (s_IsDying[playerid]) {
		return PLAYER_STATE_WASTED;
	}

	return GetPlayerState(playerid);
}

stock Float:WC_GetPlayerHealth(playerid, &Float:health = 0.0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		health = 0.0;

		return 0.0;
	}

	health = s_PlayerHealth[playerid];

	return health;
}

stock WC_SetPlayerHealth(playerid, Float:health, Float:armour = -1.0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (health <= 0.0) {
		s_PlayerArmour[playerid] = 0.0;
		s_PlayerHealth[playerid] = 0.0;

		InflictDamage(playerid, 0.0);
	} else {
		if (armour != -1.0) {
			if (armour > s_PlayerMaxArmour[playerid]) {
				armour = s_PlayerMaxArmour[playerid];
			}
			s_PlayerArmour[playerid] = armour;
		}

		if (health > s_PlayerMaxHealth[playerid]) {
			health = s_PlayerMaxHealth[playerid];
		}
		s_PlayerHealth[playerid] = health;
		UpdateHealthBar(playerid, true);
	}

	return 1;
}

stock Float:WC_GetPlayerArmour(playerid, &Float:armour = 0.0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		armour = 0.0;

		return 0.0;
	}

	armour = s_PlayerArmour[playerid];

	return armour;
}

stock WC_SetPlayerArmour(playerid, Float:armour)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (armour > s_PlayerMaxArmour[playerid]) {
		armour = s_PlayerMaxArmour[playerid];
	}
	s_PlayerArmour[playerid] = armour;
	UpdateHealthBar(playerid, true);

	return 1;
}

stock WC_GetPlayerTeam(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return NO_TEAM;
	}

	if (!IsPlayerConnected(playerid)) {
		return NO_TEAM;
	}

	return s_PlayerTeam[playerid];
}

stock WC_SetPlayerTeam(playerid, team)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	s_PlayerTeam[playerid] = team;
	SetPlayerTeam(playerid, team);

	return 1;
}

stock WC_SendDeathMessage(killer, killee, weapon)
{
	switch (weapon) {

		case WEAPON_VEHICLE_M4: {
			weapon = WEAPON_M4;
		}
		case WEAPON_VEHICLE_MINIGUN: {
			weapon = WEAPON_MINIGUN;
		}
		case WEAPON_VEHICLE_ROCKETLAUNCHER: {
			weapon = WEAPON_ROCKETLAUNCHER;
		}
		case WEAPON_PISTOLWHIP: {
			weapon = WEAPON_UNARMED;
		}
		case WEAPON_CARPARK: {
			weapon = WEAPON_VEHICLE;
		}
		case WEAPON_UNKNOWN: {
			weapon = WEAPON_DROWN;
		}
	}

	SendDeathMessage(killer, killee, weapon);

	return 1;
}

stock SetWeaponName(weaponid, const name[])
{
	if (weaponid < WEAPON_UNARMED || weaponid >= sizeof(g_WeaponName)) {
		return 0;
	}

	strunpack(g_WeaponName[weaponid], name, sizeof(g_WeaponName[]));

	return 1;
}

stock WC_GetWeaponName(weaponid, weapon[], len = sizeof(weapon))
{
	if (weaponid < WEAPON_UNARMED || weaponid >= sizeof(g_WeaponName)) {
		format(weapon, len, "Weapon %d", weaponid);
	} else {
		strunpack(weapon, g_WeaponName[weaponid], len);
	}

	return 1;
}

stock WC_ApplyAnimation(playerid, WC_CONST animlib[], WC_CONST animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync = 0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || s_IsDying[playerid]) {
		return 0;
	}

	return ApplyAnimation(playerid, animlib, animname, fDelta, loop, lockx, locky, freeze, time, forcesync);
}

stock WC_ClearAnimations(playerid, forcesync = 1)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || s_IsDying[playerid]) {
		return 0;
	}

	s_LastStop[playerid] = GetTickCount();

	return ClearAnimations(playerid, forcesync);
}

stock WC_AddPlayerClass(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	new classid = AddPlayerClass(modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);

	if (0 <= classid <= 319) {
		s_ClassSpawnInfo[classid][e_Skin] = modelid;
		s_ClassSpawnInfo[classid][e_Team] = 0x7FFFFFFF;
		s_ClassSpawnInfo[classid][e_PosX] = spawn_x;
		s_ClassSpawnInfo[classid][e_PosY] = spawn_y;
		s_ClassSpawnInfo[classid][e_PosZ] = spawn_z;
		s_ClassSpawnInfo[classid][e_Rot] = z_angle;
		s_ClassSpawnInfo[classid][e_Weapon1] = weapon1;
		s_ClassSpawnInfo[classid][e_Ammo1] = weapon1_ammo;
		s_ClassSpawnInfo[classid][e_Weapon2] = weapon2;
		s_ClassSpawnInfo[classid][e_Ammo2] = weapon2_ammo;
		s_ClassSpawnInfo[classid][e_Weapon3] = weapon3;
		s_ClassSpawnInfo[classid][e_Ammo3] = weapon3_ammo;
	}

	return classid;
}

stock WC_AddPlayerClassEx(teamid, modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	new classid = AddPlayerClassEx(teamid, modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);

	if (0 <= classid <= 319) {
		s_ClassSpawnInfo[classid][e_Skin] = modelid;
		s_ClassSpawnInfo[classid][e_Team] = teamid;
		s_ClassSpawnInfo[classid][e_PosX] = spawn_x;
		s_ClassSpawnInfo[classid][e_PosY] = spawn_y;
		s_ClassSpawnInfo[classid][e_PosZ] = spawn_z;
		s_ClassSpawnInfo[classid][e_Rot] = z_angle;
		s_ClassSpawnInfo[classid][e_Weapon1] = weapon1;
		s_ClassSpawnInfo[classid][e_Ammo1] = weapon1_ammo;
		s_ClassSpawnInfo[classid][e_Weapon2] = weapon2;
		s_ClassSpawnInfo[classid][e_Ammo2] = weapon2_ammo;
		s_ClassSpawnInfo[classid][e_Weapon3] = weapon3;
		s_ClassSpawnInfo[classid][e_Ammo3] = weapon3_ammo;
	}

	return classid;
}

stock WC_SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	if (SetSpawnInfo(playerid, team, skin, x, y, z, rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)) {
		s_PlayerClass[playerid] = -1;
		s_SpawnInfoModified[playerid] = false;

		s_PlayerSpawnInfo[playerid][e_Skin] = skin;
		s_PlayerSpawnInfo[playerid][e_Team] = team;
		s_PlayerSpawnInfo[playerid][e_PosX] = x;
		s_PlayerSpawnInfo[playerid][e_PosY] = y;
		s_PlayerSpawnInfo[playerid][e_PosZ] = z;
		s_PlayerSpawnInfo[playerid][e_Rot] = rotation;
		s_PlayerSpawnInfo[playerid][e_Weapon1] = weapon1;
		s_PlayerSpawnInfo[playerid][e_Ammo1] = weapon1_ammo;
		s_PlayerSpawnInfo[playerid][e_Weapon2] = weapon2;
		s_PlayerSpawnInfo[playerid][e_Ammo2] = weapon2_ammo;
		s_PlayerSpawnInfo[playerid][e_Weapon3] = weapon3;
		s_PlayerSpawnInfo[playerid][e_Ammo3] = weapon3_ammo;

		return 1;
	}

	return 0;
}

stock WC_TogglePlayerSpectating(playerid, bool:toggle)
{
	if (TogglePlayerSpectating(playerid, toggle)) {
		if (toggle) {
			if (s_DeathTimer[playerid] != -1) {
				KillTimer(s_DeathTimer[playerid]);
				s_DeathTimer[playerid] = -1;
			}

			#if WC_CUSTOM_VENDING_MACHINES
				if (s_VendingUseTimer[playerid] != -1) {
					KillTimer(s_VendingUseTimer[playerid]);
					s_VendingUseTimer[playerid] = -1;
				}
			#endif

			s_IsDying[playerid] = false;
		}

		return 1;
	}

	return 0;
}

stock WC_TogglePlayerControllable(playerid, toggle)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || s_IsDying[playerid]) {
		return 0;
	}

	s_LastStop[playerid] = GetTickCount();

	return TogglePlayerControllable(playerid, toggle);
}

stock WC_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || s_IsDying[playerid]) {
		return 0;
	}

	s_LastStop[playerid] = GetTickCount();

	return SetPlayerPos(playerid, x, y, z);
}

stock WC_SetPlayerPosFindZ(playerid, Float:x, Float:y, Float:z)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || s_IsDying[playerid]) {
		return 0;
	}

	s_LastStop[playerid] = GetTickCount();

	return SetPlayerPosFindZ(playerid, x, y, z);
}

stock WC_SetPlayerVelocity(playerid, Float:X, Float:Y, Float:Z)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || s_IsDying[playerid]) {
		return 0;
	}

	if (X == 0.0 && Y == 0.0 && Z == 0.0) {
		s_LastStop[playerid] = GetTickCount();
	}

	return SetPlayerVelocity(playerid, X, Y, Z);
}

stock WC_SetPlayerVirtualWorld(playerid, worldid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	s_World[playerid] = worldid;

	if (s_IsDying[playerid]) {
		return 1;
	}

	return SetPlayerVirtualWorld(playerid, worldid);
}

stock WC_GetPlayerVirtualWorld(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	new worldid = GetPlayerVirtualWorld(playerid);

	if (worldid == WC_DEATH_WORLD) {
		return s_World[playerid];
	}

	return worldid;
}

stock WC_PlayerSpectatePlayer(playerid, targetplayerid, mode = SPECTATE_MODE_NORMAL)
{
	if (PlayerSpectatePlayer(playerid, targetplayerid, mode)) {
		s_Spectating[playerid] = targetplayerid;
		return 1;
	}

	return 0;
}

stock WC_DestroyVehicle(vehicleid)
{
	if (DestroyVehicle(vehicleid)) {
		s_LastVehicleShooter[vehicleid] = INVALID_PLAYER_ID;
		s_VehicleAlive[vehicleid] = false;
		if (s_VehicleRespawnTimer[vehicleid] != -1) {
			KillTimer(s_VehicleRespawnTimer[vehicleid]);
			s_VehicleRespawnTimer[vehicleid] = -1;
		}

		return 1;
	}

	return 0;
}

stock WC_CreateVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2, respawn_delay, bool:addsiren = false)
{
	new id = CreateVehicle(modelid, x, y, z, angle, color1, color2, respawn_delay, addsiren);

	if (0 < id < MAX_VEHICLES) {
		s_VehicleAlive[id] = true;
	}

	return id;
}

stock WC_AddStaticVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2)
{
	new id = AddStaticVehicle(modelid, x, y, z, angle, color1, color2);

	if (0 < id < MAX_VEHICLES) {
		s_VehicleAlive[id] = true;
	}

	return id;
}

stock WC_AddStaticVehicleEx(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2, respawn_delay, bool:addsiren = false)
{
	new id = AddStaticVehicleEx(modelid, x, y, z, angle, color1, color2, respawn_delay, addsiren);

	if (0 < id < MAX_VEHICLES) {
		s_VehicleAlive[id] = true;
	}

	return id;
}

stock WC_IsPlayerInCheckpoint(playerid)
{
	if (!WC_IsPlayerSpawned(playerid)) {
		return 0;
	}

	return IsPlayerInCheckpoint(playerid);
}

stock WC_IsPlayerInRaceCheckpoint(playerid)
{
	if (!WC_IsPlayerSpawned(playerid)) {
		return 0;
	}

	return IsPlayerInRaceCheckpoint(playerid);
}

stock WC_SetPlayerSpecialAction(playerid, actionid)
{
	if (!WC_IsPlayerSpawned(playerid)) {
		return 0;
	}

	return SetPlayerSpecialAction(playerid, actionid);
}

stock Text:WC_TextDrawCreate(Float:x, Float:y, WC_CONST text[])
{
	new Text:td = TextDrawCreate(x, y, text);

	if (td != Text:INVALID_TEXT_DRAW) {
		s_InternalTextDraw[td] = false;
	}

	return td;
}

stock WC_TextDrawDestroy(Text:text)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawDestroy(text);
}

stock WC_TextDrawLetterSize(Text:text, Float:x, Float:y)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawLetterSize(text, x, y);
}

stock WC_TextDrawTextSize(Text:text, Float:x, Float:y)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawTextSize(text, x, y);
}

stock WC_TextDrawAlignment(Text:text, alignment)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawAlignment(text, alignment);
}

stock WC_TextDrawColor(Text:text, color)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawColor(text, color);
}

stock WC_TextDrawUseBox(Text:text, use)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawUseBox(text, use);
}

stock WC_TextDrawBoxColor(Text:text, color)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawBoxColor(text, color);
}

stock WC_TextDrawSetShadow(Text:text, size)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawSetShadow(text, size);
}

stock WC_TextDrawSetOutline(Text:text, size)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawSetOutline(text, size);
}

stock WC_TextDrawBackgroundColor(Text:text, color)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawBackgroundColor(text, color);
}

stock WC_TextDrawFont(Text:text, font)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawFont(text, font);
}

stock WC_TextDrawSetProportional(Text:text, set)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawSetProportional(text, set);
}

stock WC_TextDrawSetSelectable(Text:text, set)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawSetSelectable(text, set);
}

stock WC_TextDrawShowForPlayer(playerid, Text:text)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawShowForPlayer(playerid, text);
}

stock WC_TextDrawHideForPlayer(playerid, Text:text)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawHideForPlayer(playerid, text);
}

stock WC_TextDrawShowForAll(Text:text)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawShowForAll(text);
}

stock WC_TextDrawHideForAll(Text:text)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawHideForAll(text);
}

stock WC_TextDrawSetString(Text:text, WC_CONST string[])
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawSetString(text, string);
}

stock WC_TextDrawSetPreviewModel(Text:text, modelindex)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawSetPreviewModel(text, modelindex);
}

stock WC_TextDrawSetPreviewRot(Text:text, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fZoom = 1.0)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawSetPreviewRot(text, fRotX, fRotY, fRotZ, fZoom);
}

stock WC_TextDrawSetPreviewVehCol(Text:text, color1, color2)
{
	if (_:text < 0 || text >= Text:MAX_TEXT_DRAWS || s_InternalTextDraw[text]) return 0;
	return TextDrawSetPreviewVehCol(text, color1, color2);
}

stock PlayerText:WC_CreatePlayerTextDraw(playerid, Float:x, Float:y, WC_CONST text[])
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return PlayerText:INVALID_TEXT_DRAW;
	new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, text);

	if (td != PlayerText:INVALID_TEXT_DRAW) {
		s_InternalPlayerTextDraw[playerid][td] = false;
	}

	return td;
}

stock WC_PlayerTextDrawDestroy(playerid, PlayerText:text)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawDestroy(playerid, text);
}

stock WC_PlayerTextDrawLetterSize(playerid, PlayerText:text, Float:x, Float:y)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawLetterSize(playerid, text, x, y);
}

stock WC_PlayerTextDrawTextSize(playerid, PlayerText:text, Float:x, Float:y)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawTextSize(playerid, text, x, y);
}

stock WC_PlayerTextDrawAlignment(playerid, PlayerText:text, alignment)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawAlignment(playerid, text, alignment);
}

stock WC_PlayerTextDrawColor(playerid, PlayerText:text, color)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawColor(playerid, text, color);
}

stock WC_PlayerTextDrawUseBox(playerid, PlayerText:text, use)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawUseBox(playerid, text, use);
}

stock WC_PlayerTextDrawBoxColor(playerid, PlayerText:text, color)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawBoxColor(playerid, text, color);
}

stock WC_PlayerTextDrawSetShadow(playerid, PlayerText:text, size)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawSetShadow(playerid, text, size);
}

stock WC_PlayerTextDrawSetOutline(playerid, PlayerText:text, size)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawSetOutline(playerid, text, size);
}

stock WC_PlayerTextDrawBackgroundColo(playerid, PlayerText:text, color)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawBackgroundColor(playerid, text, color);
}

stock WC_PlayerTextDrawFont(playerid, PlayerText:text, font)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawFont(playerid, text, font);
}

stock WC_PlayerTextDrawSetProportiona(playerid, PlayerText:text, set)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawSetProportional(playerid, text, set);
}

stock WC_PlayerTextDrawSetSelectable(playerid, PlayerText:text, set)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawSetSelectable(playerid, text, set);
}

stock WC_PlayerTextDrawShow(playerid, PlayerText:text)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawShow(playerid, text);
}

stock WC_PlayerTextDrawHide(playerid, PlayerText:text)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawHide(playerid, text);
}

stock WC_PlayerTextDrawSetString(playerid, PlayerText:text, WC_CONST string[])
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawSetString(playerid, text, string);
}

stock WC_PlayerTextDrawSetPreviewMode(playerid, PlayerText:text, modelindex)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawSetPreviewModel(playerid, text, modelindex);
}

stock WC_PlayerTextDrawSetPreviewRot(playerid, PlayerText:text, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fZoom = 1.0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawSetPreviewRot(playerid, text, fRotX, fRotY, fRotZ, fZoom);
}

stock WC_PlayerTextDrawSetPreviewVehC(playerid, PlayerText:text, color1, color2)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
	if (_:text < 0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS || s_InternalPlayerTextDraw[playerid][text]) return 0;
	return PlayerTextDrawSetPreviewVehCol(playerid, text, color1, color2);
}

/*
 * Hooked callbacks
 */
public OnGameModeInit()
{
	state _ALS : _ALS_go;

	ScriptInit();

	return WC_OnGameModeInit();
}

public OnGameModeExit()
{
	ScriptExit();

	return WC_OnGameModeExit();
}

public OnFilterScriptInit()
{
	state _ALS : _ALS_go;

	ScriptInit();

	return WC_OnFilterScriptInit();
}

public OnFilterScriptExit()
{
	ScriptExit();

	return WC_OnFilterScriptExit();
}

public OnPlayerConnect(playerid)
{
	new tick = GetTickCount();

	s_PlayerMaxHealth[playerid] = 100.0;
	s_PlayerHealth[playerid] = 100.0;
	s_PlayerMaxArmour[playerid] = 100.0;
	s_PlayerArmour[playerid] = 0.0;
	s_LastExplosive[playerid] = WEAPON_UNARMED;
	s_LastShotIdx[playerid] = 0;
	s_LastShot[playerid][e_Tick] = 0;
	s_LastHitIdx[playerid] = 0;
	s_RejectedHitsIdx[playerid] = 0;
	s_ShotsFired[playerid] = 0;
	s_HitsIssued[playerid] = 0;
	s_PlayerTeam[playerid] = NO_TEAM;
	s_IsDying[playerid] = false;
	s_BeingResynced[playerid] = false;
	s_SpawnForStreamedIn[playerid] = false;
	s_World[playerid] = 0;
	s_LastAnim[playerid] = -1;
	s_LastZVelo[playerid] = 0.0;
	s_LastZ[playerid] = 0.0;
	s_LastUpdate[playerid] = tick;
	s_DamageFeedTimer[playerid] = -1;
	s_DamageFeedLastUpdate[playerid] = tick;
	s_Spectating[playerid] = INVALID_PLAYER_ID;
	s_HealthBarVisible[playerid] = false;
	s_LastSentHealth[playerid] = 0;
	s_LastSentArmour[playerid] = 0;
	s_LastStop[playerid] = tick;
	s_FirstSpawn[playerid] = true;
	s_LastVehicleEnterTime[playerid] = 0;
	s_TrueDeath[playerid] = true;
	s_InClassSelection[playerid] = false;
	s_ForceClassSelection[playerid] = false;
	s_PlayerClass[playerid] = -2;
	s_SpawnInfoModified[playerid] = false;
	s_PlayerFallbackSpawnInfo[playerid][e_Skin] = -1;
	s_DeathSkip[playerid] = 0;
	s_LastVehicleTick[playerid] = 0;
	s_PreviousHitI[playerid] = 0;
	s_CbugAllowed[playerid] = s_CbugGlobal;
	s_CbugFroze[playerid] = 0;
	s_DeathTimer[playerid] = -1;
	s_DelayedDeathTimer[playerid] = -1;
	s_DamageFeedPlayer[playerid] = -1;
	s_EnableHealthBar[playerid] = true; //enable by default

	if (s_HealthBarForeground[playerid] != PlayerText:INVALID_TEXT_DRAW) {
		s_InternalPlayerTextDraw[playerid][s_HealthBarForeground[playerid]] = false;
		s_HealthBarForeground[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	if (s_DamageFeedGiven[playerid] != PlayerText:INVALID_TEXT_DRAW) {
		s_InternalPlayerTextDraw[playerid][s_DamageFeedGiven[playerid]] = false;
		s_DamageFeedGiven[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	if (s_DamageFeedTaken[playerid] != PlayerText:INVALID_TEXT_DRAW) {
		s_InternalPlayerTextDraw[playerid][s_DamageFeedTaken[playerid]] = false;
		s_DamageFeedTaken[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	for (new i = 0; i < sizeof(s_PreviousHits[]); i++) {
		s_PreviousHits[playerid][i][e_Tick] = 0;
	}

	for (new i = 0; i < sizeof(s_RejectedHits[]); i++) {
		s_RejectedHits[playerid][i][e_Time] = 0;
	}

	for (new i = 0; i < sizeof(s_DamageFeedHitsGiven[]); i++) {
		s_DamageFeedHitsGiven[playerid][i][e_Tick] = 0;
		s_DamageFeedHitsTaken[playerid][i][e_Tick] = 0;
	}

	SetPlayerTeam(playerid, s_PlayerTeam[playerid]);
	FreezeSyncPacket(playerid, .toggle = false);
	SetFakeFacingAngle(playerid, _);
	DamageFeedUpdate(playerid);

	#if WC_CUSTOM_VENDING_MACHINES
		if (!s_AlreadyConnected[playerid]) {
			RemoveDefaultVendingMachines(playerid);
		}
	#endif

	s_AlreadyConnected[playerid] = false;

	return WC_OnPlayerConnect(playerid);
}

public OnPlayerDisconnect(playerid, reason)
{
	WC_OnPlayerDisconnect(playerid, reason);

	#if WC_CUSTOM_VENDING_MACHINES
		if (s_VendingUseTimer[playerid] != -1) {
			KillTimer(s_VendingUseTimer[playerid]);
			s_VendingUseTimer[playerid] = -1;
		}
	#endif

	if (s_DelayedDeathTimer[playerid] != -1) {
		KillTimer(s_DelayedDeathTimer[playerid]);
		s_DelayedDeathTimer[playerid] = -1;
	}

	if (s_DeathTimer[playerid] != -1) {
		KillTimer(s_DeathTimer[playerid]);
		s_DeathTimer[playerid] = -1;
	}

	if (s_KnifeTimeout[playerid] != -1) {
		KillTimer(s_KnifeTimeout[playerid]);
		s_KnifeTimeout[playerid] = -1;
	}

	if (s_HealthBarForeground[playerid] != PlayerText:INVALID_TEXT_DRAW) {
		PlayerTextDrawDestroy(playerid, s_HealthBarForeground[playerid]);
		s_HealthBarForeground[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	if (s_DamageFeedGiven[playerid] != PlayerText:INVALID_TEXT_DRAW) {
		PlayerTextDrawDestroy(playerid, s_DamageFeedGiven[playerid]);
		s_DamageFeedGiven[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	if (s_DamageFeedTaken[playerid] != PlayerText:INVALID_TEXT_DRAW) {
		PlayerTextDrawDestroy(playerid, s_DamageFeedTaken[playerid]);
		s_DamageFeedTaken[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	if (s_DamageFeedTimer[playerid] != -1) {
		KillTimer(s_DamageFeedTimer[playerid]);
		s_DamageFeedTimer[playerid] = -1;
	}

	SetHealthBarVisible(playerid, false);

	s_Spectating[playerid] = INVALID_PLAYER_ID;

	for (new i = 0; i < sizeof(s_LastVehicleShooter); i++) {
		if (s_LastVehicleShooter[i] == playerid) {
			s_LastVehicleShooter[i] = INVALID_PLAYER_ID;
		}
	}

	for (new i = 0; i < sizeof(s_InternalPlayerTextDraw[]); i++) {
		s_InternalPlayerTextDraw[playerid][PlayerText:i] = false;
	}

	new j = 0;

	#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
	foreach (new i : Player) {
	#else
	for (new i = GetPlayerPoolSize(); i != -1; i--) {
	#endif
		for (j = 0; j < sizeof(s_PreviousHits[]); j++) {
			if (s_PreviousHits[i][j][e_Issuer] == playerid) {
				s_PreviousHits[i][j][e_Issuer] = INVALID_PLAYER_ID;
			}
		}
	}

	return 1;
}

public OnPlayerSpawn(playerid)
{
	s_TrueDeath[playerid] = false;
	s_InClassSelection[playerid] = false;

	if (s_ForceClassSelection[playerid]) {
		DebugMessage(playerid, "Being forced into class selection");
		ForceClassSelection(playerid);
		SetPlayerHealth(playerid, 0.0);
		SetPlayerVirtualWorld(playerid, WC_DEATH_WORLD);

		return 1;
	}

	new tick = GetTickCount();
	s_LastUpdate[playerid] = tick;
	s_LastStop[playerid] = tick;

	if (s_BeingResynced[playerid]) {
		s_BeingResynced[playerid] = false;

		UpdateHealthBar(playerid);

		SetPlayerPos(playerid, s_SyncData[playerid][e_PosX], s_SyncData[playerid][e_PosY], s_SyncData[playerid][e_PosZ]);
		SetPlayerFacingAngle(playerid, s_SyncData[playerid][e_PosA]);

		SetPlayerSkin(playerid, s_SyncData[playerid][e_Skin]);
		SetPlayerTeam(playerid, s_SyncData[playerid][e_Team]);

		for (new i = 0; i < 13; i++) {
			if (s_SyncData[playerid][e_WeaponId][i]) {
				GivePlayerWeapon(playerid, s_SyncData[playerid][e_WeaponId][i], s_SyncData[playerid][e_WeaponAmmo][i]);
			}
		}

		SetPlayerArmedWeapon(playerid, s_SyncData[playerid][e_Weapon]);

		return 1;
	}

	if (s_SpawnInfoModified[playerid]) {
		new spawn_info[E_SPAWN_INFO], classid = s_PlayerClass[playerid];

		s_SpawnInfoModified[playerid] = false;

		if (classid == -1) {
			spawn_info = s_PlayerSpawnInfo[playerid];
		} else if (classid == -2) {
			spawn_info = s_PlayerFallbackSpawnInfo[playerid];
		} else {
			if (s_ClassSpawnInfo[classid][e_Skin] == -1 && s_PlayerFallbackSpawnInfo[playerid][e_Skin] != -1) {
				spawn_info = s_PlayerFallbackSpawnInfo[playerid];
			} else {
				spawn_info = s_ClassSpawnInfo[classid];
			}
		}

		if (spawn_info[e_Skin] != -1) {
			SetSpawnInfo(
				playerid,
				spawn_info[e_Team],
				spawn_info[e_Skin],
				spawn_info[e_PosX],
				spawn_info[e_PosY],
				spawn_info[e_PosZ],
				spawn_info[e_Rot],
				spawn_info[e_Weapon1],
				spawn_info[e_Ammo1],
				spawn_info[e_Weapon2],
				spawn_info[e_Ammo2],
				spawn_info[e_Weapon3],
				spawn_info[e_Ammo3]
			);
		}
	} else {
		s_PlayerFallbackSpawnInfo[playerid][e_Team] = s_PlayerTeam[playerid];
		s_PlayerFallbackSpawnInfo[playerid][e_Skin] = GetPlayerSkin(playerid);
		GetPlayerPos(
			playerid,
			s_PlayerFallbackSpawnInfo[playerid][e_PosX],
			s_PlayerFallbackSpawnInfo[playerid][e_PosY],
			s_PlayerFallbackSpawnInfo[playerid][e_PosZ]
		);
		GetPlayerFacingAngle(playerid, s_PlayerFallbackSpawnInfo[playerid][e_Rot]);
	}

	if (s_DeathTimer[playerid] != -1) {
		KillTimer(s_DeathTimer[playerid]);
		s_DeathTimer[playerid] = -1;
	}

	if (s_IsDying[playerid]) {
		s_IsDying[playerid] = false;
	}

	if (s_PlayerHealth[playerid] == 0.0) {
		s_PlayerHealth[playerid] = s_PlayerMaxHealth[playerid];
	}

	UpdatePlayerVirtualWorld(playerid);
	UpdateHealthBar(playerid, true);
	FreezeSyncPacket(playerid, .toggle = false);
	SetFakeFacingAngle(playerid, _);
	DamageFeedUpdate(playerid);

	if (GetPlayerTeam(playerid) != s_PlayerTeam[playerid]) {
		SetPlayerTeam(playerid, s_PlayerTeam[playerid]);
	}

	new animlib[32], animname[32];

	if (s_DeathSkip[playerid] == 2) {
		DebugMessage(playerid, "Death skipped");
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		SetPlayerArmedWeapon(playerid, 0);
		ClearAnimations(playerid);

		animlib = "PED", animname = "IDLE_stance";
		ApplyAnimation(playerid, animlib, animname, 4.1, 1, 0, 0, 0, 1, 1);

		s_DeathSkip[playerid] = 1;
		s_DeathSkipTick[playerid] = tick;

		return 1;
	}

	if (s_FirstSpawn[playerid]) {
		s_FirstSpawn[playerid] = false;

		#if WC_CUSTOM_VENDING_MACHINES
			if (s_CustomVendingMachines) {
				animlib = "VENDING", animname = "null";
				ApplyAnimation(playerid, animlib, animname, 0.0, 0, 0, 0, 0, 0, 0);
			}
		#endif
	}

	return WC_OnPlayerSpawn(playerid);
}

public OnPlayerRequestClass(playerid, classid)
{
	DebugMessage(playerid, "Requested class: %d", classid);

	if (s_DeathSkip[playerid]) {
		DebugMessage(playerid, "Skipping death - class selection skipped");
		SpawnPlayer(playerid);

		return 0;
	}

	if (s_ForceClassSelection[playerid]) {
		s_ForceClassSelection[playerid] = false;
	}

	if (s_BeingResynced[playerid]) {
		s_TrueDeath[playerid] = false;

		SpawnPlayerInPlace(playerid);

		return 0;
	}

	if (s_DeathTimer[playerid] != -1) {
		KillTimer(s_DeathTimer[playerid]);
		s_DeathTimer[playerid] = -1;
	}

	if (s_IsDying[playerid]) {
		OnPlayerDeathFinished(playerid, false);
		s_IsDying[playerid] = false;
	}

	FreezeSyncPacket(playerid, .toggle = false);
	UpdatePlayerVirtualWorld(playerid);

	if (s_TrueDeath[playerid]) {
		if (!s_InClassSelection[playerid]) {
			DebugMessage(playerid, "True death class selection");

			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x, y, z);
			RemoveBuildingForPlayer(playerid, 1484, x, y, z, 350.0),
			RemoveBuildingForPlayer(playerid, 1485, x, y, z, 350.0),
			RemoveBuildingForPlayer(playerid, 1486, x, y, z, 350.0);

			s_InClassSelection[playerid] = true;
		}

		if (WC_OnPlayerRequestClass(playerid, classid)) {
			s_PlayerClass[playerid] = classid;

			return 1;
		} else {
			return 0;
		}
	} else {
		DebugMessage(playerid, "Not true death - being respawned");

		s_ForceClassSelection[playerid] = true;

		SpawnPlayerInPlace(playerid);

		return 0;
	}
}

public OnPlayerDeath(playerid, killerid, reason)
{
	s_TrueDeath[playerid] = true;
	s_InClassSelection[playerid] = false;

	#if WC_CUSTOM_VENDING_MACHINES
		if (s_VendingUseTimer[playerid] != -1) {
			KillTimer(s_VendingUseTimer[playerid]);
			s_VendingUseTimer[playerid] = -1;
		}
	#endif

	if (s_BeingResynced[playerid] || s_ForceClassSelection[playerid]) {
		return 1;
	}

	// Probably fake death
	if (killerid != INVALID_PLAYER_ID && !IsPlayerStreamedIn(killerid, playerid)) {
		killerid = INVALID_PLAYER_ID;
	}

	DebugMessageRedAll("OnPlayerDeath(%d died by %d from %d)", playerid, reason, killerid);

	if (s_DeathTimer[playerid] != -1) {
		KillTimer(s_DeathTimer[playerid]);
		s_DeathTimer[playerid] = -1;
	}

	if (s_IsDying[playerid]) {
		DebugMessageRedAll("death while dying %d", playerid);

		return 1;
	}

	if (reason < WEAPON_UNARMED || reason > WEAPON_UNKNOWN) {
		reason = WEAPON_UNKNOWN;
	}

	new vehicleid = GetPlayerVehicleID(playerid);

	// Let's assume they died from an exploading vehicle
	if (vehicleid != INVALID_VEHICLE_ID && WC_IsValidVehicle(vehicleid)) {
		reason = WEAPON_EXPLOSION;
		killerid = INVALID_PLAYER_ID;

		if (!HasSameTeam(playerid, s_LastVehicleShooter[vehicleid])) {
			killerid = s_LastVehicleShooter[vehicleid];
		}
	}

	new Float:amount = 0.0;
	new bodypart = BODY_PART_UNKNOWN;

	if (reason == WEAPON_PARACHUTE) {
		reason = WEAPON_COLLISION;
	}

	if (OnPlayerDamage(playerid, amount, killerid, reason, bodypart)) {
		if (reason < WEAPON_UNARMED || reason > WEAPON_UNKNOWN) {
			reason = WEAPON_UNKNOWN;
		}

		if (amount == 0.0) {
			amount = s_PlayerHealth[playerid] + s_PlayerArmour[playerid];
		}

		if (reason == WEAPON_COLLISION || reason == WEAPON_DROWN || reason == WEAPON_CARPARK) {
			if (amount <= 0.0) {
				amount = s_PlayerHealth[playerid];
			}

			s_PlayerHealth[playerid] -= amount;
		} else {
			if (amount <= 0.0) {
				amount = s_PlayerHealth[playerid] + s_PlayerArmour[playerid];
			}

			s_PlayerArmour[playerid] -= amount;
		}

		if (s_PlayerArmour[playerid] < 0.0) {
			s_DamageDoneArmour[playerid] = amount + s_PlayerArmour[playerid];
			s_DamageDoneHealth[playerid] = -s_PlayerArmour[playerid];
			s_PlayerHealth[playerid] += s_PlayerArmour[playerid];
			s_PlayerArmour[playerid] = 0.0;
		} else {
			s_DamageDoneArmour[playerid] = amount;
			s_DamageDoneHealth[playerid] = 0.0;
		}

		if (s_PlayerHealth[playerid] <= 0.0) {
			amount += s_PlayerHealth[playerid];
			s_DamageDoneHealth[playerid] += s_PlayerHealth[playerid];
			s_PlayerHealth[playerid] = 0.0;
		}

		OnPlayerDamageDone(playerid, amount, killerid, reason, bodypart);
	}

	if (s_PlayerHealth[playerid] <= 0.0005) {
		s_PlayerHealth[playerid] = 0.0;
		s_IsDying[playerid] = true;

		s_LastDeathTick[playerid] = GetTickCount();

		new animlib[32], animname[32], anim_lock, respawn_time;

		OnPlayerPrepareDeath(playerid, animlib, animname, anim_lock, respawn_time);

		WC_OnPlayerDeath(playerid, killerid, reason);

		OnPlayerDeathFinished(playerid, false);
	} else {
		if (vehicleid || WasPlayerInVehicle(playerid, 10000)) {
			new Float:x, Float:y, Float:z, Float:r;

			GetPlayerPos(playerid, x, y, z);
			SetPlayerPos(playerid, x, y, z);
			SaveSyncData(playerid);

			if (vehicleid) {
				GetVehicleZAngle(vehicleid, r);
			} else {
				GetPlayerFacingAngle(playerid, r);
			}

			s_DeathSkip[playerid] = 2;

			ForceClassSelection(playerid);
			SetSpawnInfo(playerid, s_PlayerTeam[playerid], GetPlayerSkin(playerid), x, y, z, r, 0, 0, 0, 0, 0, 0);
			TogglePlayerSpectating(playerid, true);
			TogglePlayerSpectating(playerid, false);
			SetSpawnInfo(playerid, s_PlayerTeam[playerid], GetPlayerSkin(playerid), x, y, z, r, 0, 0, 0, 0, 0, 0);
			TogglePlayerControllable(playerid, true);
			GivePlayerWeapon(playerid, 1, 1);
		} else {
			SpawnPlayerInPlace(playerid);
		}
	}

	UpdateHealthBar(playerid);

	return 1;
}

static Float:AngleBetweenPoints(Float:x1, Float:y1, Float:x2, Float:y2);

forward WC_CbugPunishment(playerid, weapon);
public WC_CbugPunishment(playerid, weapon) {
	FreezeSyncPacket(playerid, .toggle = false);
	SetPlayerArmedWeapon(playerid, weapon);
	
	if (!IsPlayerDying(playerid)) {
		ClearAnimations(playerid, 1);
	}
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	new animlib[32], animname[32];

	if (!s_CbugAllowed[playerid] && !IsPlayerDying(playerid) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		if (newkeys & KEY_CROUCH) {
			new tick = GetTickCount();
			new diff = tick - s_LastShot[playerid][e_Tick];

			if (s_LastShot[playerid][e_Tick] && diff < 1200 && !s_CbugFroze[playerid]) {
				PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);

				if (s_LastShot[playerid][e_Valid] && floatabs(s_LastShot[playerid][e_HX]) > 1.0 && floatabs(s_LastShot[playerid][e_HY]) > 1.0) {
					SetPlayerFacingAngle(playerid, AngleBetweenPoints(
						s_LastShot[playerid][e_HX],
						s_LastShot[playerid][e_HY],
						s_LastShot[playerid][e_OX],
						s_LastShot[playerid][e_OY]
					));
				}

				new w, a;
				GetPlayerWeaponData(playerid, 0, w, a);

				animlib = "PED", animname = "IDLE_stance";
				ClearAnimations(playerid, 1);
				ApplyAnimation(playerid, animlib, animname, 4.1, 1, 0, 0, 0, 0, 1);
				FreezeSyncPacket(playerid, .toggle = true);
				SetPlayerArmedWeapon(playerid, w);
				SetTimerEx("WC_CbugPunishment", 600, false, "ii", playerid, GetPlayerWeapon(playerid));

				s_CbugFroze[playerid] = tick;

				new j = 0, Float:health, Float:armour;

				#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
				foreach (new i : Player) {
				#else
				for (new i = GetPlayerPoolSize(); i != -1; i--) {
				#endif
					for (j = 0; j < sizeof(s_PreviousHits[]); j++) {
						if (s_PreviousHits[i][j][e_Issuer] == playerid && tick - s_PreviousHits[i][j][e_Tick] <= 1200) {
							s_PreviousHits[i][j][e_Issuer] = INVALID_PLAYER_ID;

							health = WC_GetPlayerHealth(i);
							armour = WC_GetPlayerArmour(i);

							if (s_IsDying[i]) {
								if (s_DelayedDeathTimer[i] == -1) {
									continue;
								}

								KillTimer(s_DelayedDeathTimer[i]);
								s_DelayedDeathTimer[i] = -1;
								ClearAnimations(i, 1);
								SetFakeFacingAngle(i, _);
								FreezeSyncPacket(i, .toggle = false);

								s_IsDying[i] = false;

								if (s_DeathTimer[i] != -1) {
									KillTimer(s_DeathTimer[i]);
									s_DeathTimer[i] = -1;
								}
							}

							health += s_PreviousHits[i][j][e_Health];
							armour += s_PreviousHits[i][j][e_Armour];

							WC_SetPlayerHealth(i, health, armour);
						}
					}
				}
			}
		}
	}

	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		#if WC_CUSTOM_VENDING_MACHINES
			if (s_CustomVendingMachines
			 && newkeys == KEY_SECONDARY_ATTACK
			 && !oldkeys
			 && s_VendingUseTimer[playerid] == -1
			 && GetPlayerAnimationIndex(playerid) != 1660) {
				new bool:failed = false;

				if (GetPlayerMoney(playerid) <= 0 || s_PlayerHealth[playerid] >= s_PlayerMaxHealth[playerid]) {
					failed = true;
				}

				new Float:z;

				GetPlayerPos(playerid, z, z, z);

				for (new i = 0, Float:health_given; i < sizeof(sc_VendingMachines); i++) {
					if (floatabs(z - sc_VendingMachines[i][e_PosZ]) > 1.5) {
						continue;
					}

					if (IsPlayerInRangeOfPoint(playerid, 0.5, sc_VendingMachines[i][e_FrontX], sc_VendingMachines[i][e_FrontY], z)) {
						if (failed) {
							PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
							break;
						}

						health_given = 35.0;

						if (OnPlayerUseVendingMachine(playerid, health_given)) {
							s_VendingUseTimer[playerid] = SetTimerEx("WC_VendingMachineUsed", 2500, false, "if", playerid, health_given);

							animlib = "VENDING", animname = "VEND_USE";
							SetPlayerFacingAngle(playerid, sc_VendingMachines[i][e_RotZ]);
							SetPlayerPos(playerid, sc_VendingMachines[i][e_FrontX], sc_VendingMachines[i][e_FrontY], z);
							ApplyAnimation(playerid, animlib, animname, 4.1, 0, 0, 1, 0, 0, 1);

							PlayerPlaySound(playerid, 42600, 0.0, 0.0, 0.0);
						} else {
							PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
						}

						break;
					}
				}
			}
		#endif

		if (newkeys & KEY_FIRE) {
			new weap = GetPlayerWeapon(playerid);

			switch (weap) {
				case WEAPON_BOMB, WEAPON_SATCHEL: {
					s_LastExplosive[playerid] = WEAPON_SATCHEL;
				}

				case WEAPON_ROCKETLAUNCHER, WEAPON_HEATSEEKER, WEAPON_GRENADE: {
					s_LastExplosive[playerid] = weap;
				}
			}
		}
	}

	return WC_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	// Send ped floor_hit_f
	if (s_IsDying[playerid]) {
		SendLastSyncPacket(playerid, forplayerid, .animation = 0x2e040000 + 1150);
	}

	return WC_OnPlayerStreamIn(playerid, forplayerid);
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	#if WC_CUSTOM_VENDING_MACHINES
		if (s_VendingUseTimer[playerid] != -1) {
			KillTimer(s_VendingUseTimer[playerid]);
			s_VendingUseTimer[playerid] = -1;
		}
	#endif

	s_LastVehicleEnterTime[playerid] = gettime();
	s_LastVehicleTick[playerid] = GetTickCount();

	if (s_IsDying[playerid]) {
		TogglePlayerControllable(playerid, false);
		ApplyAnimation(playerid, "PED", "KO_skid_back", 4.1, 0, 0, 0, 1, 0, 1);
	}

	return WC_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	s_LastVehicleTick[playerid] = GetTickCount();

	return WC_OnPlayerExitVehicle(playerid, vehicleid);
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (s_Spectating[playerid] != INVALID_PLAYER_ID && newstate != PLAYER_STATE_SPECTATING) {
		s_Spectating[playerid] = INVALID_PLAYER_ID;
	}

	#if WC_CUSTOM_VENDING_MACHINES
		if (s_VendingUseTimer[playerid] != -1) {
			KillTimer(s_VendingUseTimer[playerid]);
			s_VendingUseTimer[playerid] = -1;
		}
	#endif

	if (s_IsDying[playerid] && (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)) {
		TogglePlayerControllable(playerid, false);
	}

	if (oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER) {
		s_LastVehicleTick[playerid] = GetTickCount();

		if (newstate == PLAYER_STATE_ONFOOT) {
			new Float:vx, Float:vy, Float:vz;
			GetPlayerVelocity(playerid, vx, vy, vz);

			if (vx * vx + vy * vy + vz * vz <= 0.05) {
				#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
				foreach (new i : Player) {
				#else
				for (new i = GetPlayerPoolSize(); i != -1; i--) {
				#endif
					if (i != playerid && IsPlayerStreamedIn(playerid, i)) {
						SendLastSyncPacket(playerid, i);
						ClearAnimationsForPlayer(playerid, i);
					}
				}
			}
		}
	}

	switch (newstate) {
		case PLAYER_STATE_ONFOOT,
		     PLAYER_STATE_DRIVER,
		     PLAYER_STATE_PASSENGER: {
			SetHealthBarVisible(playerid, true);
		}

		default: {
			SetHealthBarVisible(playerid, false);
		}
	}

	return WC_OnPlayerStateChange(playerid, newstate, oldstate);
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	if (!WC_IsPlayerSpawned(playerid)) {
		return 0;
	}

	return WC_OnPlayerPickUpPickup(playerid, pickupid);
}

public OnPlayerUpdate(playerid)
{
	if (s_IsDying[playerid]) {
		return 1;
	}

	new tick = GetTickCount();

	if (s_DeathSkip[playerid] == 1) {
		if (s_DeathSkipTick[playerid]) {
			if (tick - s_DeathSkipTick[playerid] > 1000) {
				new Float:x, Float:y, Float:z, Float:r;

				GetPlayerPos(playerid, x, y, z);
				GetPlayerFacingAngle(playerid, r);

				SetSpawnInfo(playerid, s_PlayerTeam[playerid], GetPlayerSkin(playerid), x, y, z, r, 0, 0, 0, 0, 0, 0);

				s_DeathSkipTick[playerid] = 0;

				new animlib[] = "PED", animname[] = "IDLE_stance";
				ApplyAnimation(playerid, animlib, animname, 4.1, 1, 0, 0, 0, 1, 1);
			}
		} else {
			if (GetPlayerAnimationIndex(playerid) != 1189) {
				s_DeathSkip[playerid] = 0;

				WC_DeathSkipEnd(playerid);

				DebugMessage(playerid, "Death skip end");
			}
		}
	}

	if (s_SpawnForStreamedIn[playerid]) {
		WC_SpawnForStreamedIn(playerid);

		s_SpawnForStreamedIn[playerid] = false;
	}

	s_LastUpdate[playerid] = tick;

	// Detect fall damage based on velocity and animation
	if (s_CustomFallDamage) {
		new Float:vz, Float:z;

		GetPlayerVelocity(playerid, vz, vz, vz);
		GetPlayerPos(playerid, z, z, z);

		new surfing = GetPlayerSurfingVehicleID(playerid);

		if (surfing == INVALID_VEHICLE_ID) {
			surfing = GetPlayerSurfingObjectID(playerid) != INVALID_OBJECT_ID;
		} else {
			surfing = 1;
		}

		if (surfing || tick - s_LastStop[playerid] < 2000) {
			vz = 0.0;
			s_LastZVelo[playerid] = 0.0;
		} else {
			if (vz != 0.0) {
				s_LastZVelo[playerid] = vz;
			}

			if (z - s_LastZ[playerid] > 1.0) {
				s_LastZVelo[playerid] = 0.1;
				vz = 0.1;
			}
		}

		s_LastZ[playerid] = z;

		new anim = GetPlayerAnimationIndex(playerid);

		if (anim != s_LastAnim[playerid]) {
			new prev = s_LastAnim[playerid];
			s_LastAnim[playerid] = anim;

			if (((prev == 1130 && vz == 0.0) || 1128 <= anim <= 1134 || anim == 1208)) {
				new Float:amount = -1.0;
				DebugMessage(playerid, "vz: %f anim: %d prev: %d", vz, anim, prev);

				vz = s_LastZVelo[playerid];

				if (vz <= s_FallDeathVelocity) {
					amount = 0.0;
				} else if (vz <= -0.2) {
					if (vz == -0.2) {
						amount = s_WeaponDamage[WEAPON_COLLISION] * 0.2;
					} else {
						amount = (vz + 0.2) / (s_FallDeathVelocity + 0.2);
						amount *= s_WeaponDamage[WEAPON_COLLISION];
					}
				}

				if (GetPlayerWeapon(playerid) == WEAPON_PARACHUTE && anim == 1134) {
					amount = -1.0;
				}

				if (amount >= 0.0) {
					DebugMessage(playerid, "fall dmg: %.5f (vz: %f, anim: %d, prev: %d)", amount, vz, anim, prev);

					InflictDamage(playerid, amount, INVALID_PLAYER_ID, WEAPON_COLLISION, 3);
				}
			}
		}
	}

	return WC_OnPlayerUpdate(playerid);
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	if (!IsHighRateWeapon(weaponid)) {
		DebugMessage(playerid, "OnPlayerGiveDamage(%d gave %f to %d using %d on bodypart %d)", playerid, amount, damagedid, weaponid, bodypart);
	}

	// Nobody got damaged
	if (!IsPlayerConnected(damagedid)) {
		OnInvalidWeaponDamage(playerid, damagedid, amount, weaponid, bodypart, WC_NO_DAMAGED, true);

		AddRejectedHit(playerid, damagedid, HIT_NO_DAMAGEDID, weaponid);

		return 0;
	}

	if (s_IsDying[damagedid]) {
		AddRejectedHit(playerid, damagedid, HIT_DYING_PLAYER, weaponid);
		return 0;
	}

	if (!s_LagCompMode) {
		new npc = IsPlayerNPC(damagedid);

		if (weaponid == WEAPON_KNIFE && _:amount == _:0.0) {
			if (s_KnifeTimeout[damagedid] != -1) {
				KillTimer(s_KnifeTimeout[damagedid]);
			}

			s_KnifeTimeout[damagedid] = SetTimerEx("WC_SetSpawnForStreamedIn", 2500, false, "i", damagedid);
		}

		if (!npc) {
			return 0;
		}
	}

	// Ignore unreliable and invalid damage
	if (weaponid < WEAPON_UNARMED || weaponid >= sizeof(s_ValidDamageGiven) || !s_ValidDamageGiven[weaponid]) {
		// Fire is synced as taken damage (because it's not reliable as given), so no need to show a rejected hit.
		// Vehicle damage is also synced as taken, so no need to show that either.
		if (weaponid != WEAPON_FLAMETHROWER && weaponid != WEAPON_VEHICLE) {
			AddRejectedHit(playerid, damagedid, HIT_INVALID_WEAPON, weaponid);
		}

		return 0;
	}

	new tick = GetTickCount();
	if (tick == 0) tick = 1;

	if (!WC_IsPlayerSpawned(playerid) && tick - s_LastDeathTick[playerid] > 80) {
		// Make sure the rejected hit wasn't added in OnPlayerWeaponShot
		if (!IsBulletWeapon(weaponid) || s_LastShot[playerid][e_Valid]) {
			AddRejectedHit(playerid, damagedid, HIT_NOT_SPAWNED, weaponid);
		}

		return 0;
	}

	new npc = IsPlayerNPC(damagedid);

	// From stealth knife, can be any weapon
	if (_:amount == _:1833.33154296875) {
		return 0;
	}

	if (weaponid == WEAPON_KNIFE) {
		if (_:amount == _:0.0) {
			// Resync without bothering the player being knifed
			if (npc || HasSameTeam(playerid, damagedid)) {
				if (s_KnifeTimeout[damagedid] != -1) {
					KillTimer(s_KnifeTimeout[damagedid]);
				}

				s_KnifeTimeout[damagedid] = SetTimerEx("WC_SpawnForStreamedIn", 150, false, "i", damagedid);
				ClearAnimations(playerid, 1);
				SetPlayerArmedWeapon(playerid, 0);

				return 0;
			} else {
				new Float:x, Float:y, Float:z;
				GetPlayerPos(playerid, x, y, z);

				if (GetPlayerDistanceFromPoint(damagedid, x, y, z) > s_WeaponRange[weaponid] + 2.0) {
					if (s_KnifeTimeout[damagedid] != -1) {
						KillTimer(s_KnifeTimeout[damagedid]);
					}

					s_KnifeTimeout[damagedid] = SetTimerEx("WC_SpawnForStreamedIn", 150, false, "i", damagedid);
					ClearAnimations(playerid, 1);
					SetPlayerArmedWeapon(playerid, 0);

					return 0;
				}
			}

			if (!OnPlayerDamage(damagedid, amount, playerid, weaponid, bodypart)) {
				if (s_KnifeTimeout[damagedid] != -1) {
					KillTimer(s_KnifeTimeout[damagedid]);
				}

				s_KnifeTimeout[damagedid] = SetTimerEx("WC_SpawnForStreamedIn", 150, false, "i", damagedid);
				ClearAnimations(playerid, 1);
				SetPlayerArmedWeapon(playerid, 0);

				return 0;
			}

			s_DamageDoneHealth[playerid] = s_PlayerHealth[playerid];
			s_DamageDoneArmour[playerid] = s_PlayerArmour[playerid];

			OnPlayerDamageDone(damagedid, s_PlayerHealth[damagedid] + s_PlayerArmour[damagedid], playerid, weaponid, bodypart);

			ClearAnimations(damagedid, 1);

			new animlib[32] = "KNIFE", animname[32] = "KILL_Knife_Ped_Damage";
			PlayerDeath(damagedid, animlib, animname, _, 5200);

			SetTimerEx("WC_SecondKnifeAnim", 2200, false, "i", damagedid);

			WC_OnPlayerDeath(damagedid, playerid, weaponid);

			DebugMessage(damagedid, "being knifed by %d", playerid);
			DebugMessage(playerid, "knifing %d", damagedid);

			new Float:x, Float:y, Float:z, Float:a;

			GetPlayerFacingAngle(damagedid, a);
			SetPlayerFacingAngle(playerid, a);
			PosInFront(damagedid, -1.0, x, y, z);

			SetPlayerVelocity(damagedid, 0.0, 0.0, 0.0);
			SetPlayerVelocity(playerid, 0.0, 0.0, 0.0);

			new forcesync = 2;

			if (747 < GetPlayerAnimationIndex(playerid) > 748) {
				DebugMessageRed(playerid, "applying knife anim for you too (current: %d)", GetPlayerAnimationIndex(playerid));

				forcesync = 1;
			}

			animname = "KILL_Knife_Player";
			ApplyAnimation(playerid, animlib, animname, 4.1, 0, 1, 1, 0, 1800, forcesync);

			return 0;
		}
	}

	if (HasSameTeam(playerid, damagedid)) {
		AddRejectedHit(playerid, damagedid, HIT_SAME_TEAM, weaponid);
		return 0;
	}

	// Both players should see eachother
	if ((!IsPlayerStreamedIn(playerid, damagedid) && !WC_IsPlayerPaused(damagedid)) || !IsPlayerStreamedIn(damagedid, playerid)) {
		AddRejectedHit(playerid, damagedid, HIT_UNSTREAMED, weaponid, damagedid);
		return 0;
	}

	new Float:bullets, err;

	if ((err = ProcessDamage(damagedid, playerid, amount, weaponid, bodypart, bullets))) {
		if (err == WC_INVALID_DAMAGE) {
			AddRejectedHit(playerid, damagedid, HIT_INVALID_DAMAGE, weaponid, _:amount);
		}

		if (err != WC_INVALID_DISTANCE) {
			OnInvalidWeaponDamage(playerid, damagedid, amount, weaponid, bodypart, err, true);
		}

		return 0;
	}

	new idx = (s_LastHitIdx[playerid] + 1) % sizeof(s_LastHitTicks[]);

	// JIT plugin fix
	if (idx < 0) {
		idx += sizeof(s_LastHitTicks[]);
	}

	s_LastHitIdx[playerid] = idx;
	s_LastHitTicks[playerid][idx] = tick;
	s_LastHitWeapons[playerid][idx] = weaponid;
	s_HitsIssued[playerid] += 1;

	#if WC_DEBUG
		if (s_HitsIssued[playerid] > 1) {
			new prev_tick_idx = (idx - 1) % sizeof(s_LastHitTicks[]);

			// JIT plugin fix
			if (prev_tick_idx < 0) {
				prev_tick_idx += sizeof(s_LastHitTicks[]);
			}

			new prev_tick = s_LastHitTicks[playerid][prev_tick_idx];

			DebugMessage(playerid, "(hit) last: %d last 3: %d", tick - prev_tick, AverageHitRate(playerid, 3));
		}
	#endif

	new multiple_weapons;
	new avg_rate = AverageHitRate(playerid, s_MaxHitRateSamples, multiple_weapons);

	// Hit issue flood?
	// Could be either a cheat or just lag
	if (avg_rate != -1) {
		if (multiple_weapons) {
			if (avg_rate < 100) {
				AddRejectedHit(playerid, damagedid, HIT_RATE_TOO_FAST_MULTIPLE, weaponid, avg_rate, s_MaxHitRateSamples);
				return 0;
			}
		} else if (s_MaxWeaponShootRate[weaponid] - avg_rate > 20) {
			AddRejectedHit(playerid, damagedid, HIT_RATE_TOO_FAST, weaponid, avg_rate, s_MaxHitRateSamples, s_MaxWeaponShootRate[weaponid]);
			return 0;
		}
	}

	if (IsBulletWeapon(weaponid) && _:amount != _:2.6400001049041748046875 && GetPlayerState(playerid) != PLAYER_STATE_DRIVER) {
		new valid = true;

		if (!s_LastShot[playerid][e_Valid]) {
			valid = false;
			//AddRejectedHit(playerid, damagedid, HIT_LAST_SHOT_INVALID, weaponid);
			DebugMessageRed(playerid, "last shot not valid");
		} else if (WEAPON_SHOTGUN <= weaponid <= WEAPON_SHOTGSPA) {
			// Let's assume someone won't hit 2 players with 1 shotgun shot, and that one OnPlayerWeaponShot can be out of sync
			if (s_LastShot[playerid][e_Hits] >= 2) {
				valid = false;
				AddRejectedHit(playerid, damagedid, HIT_MULTIPLE_PLAYERS_SHOTGUN, weaponid, s_LastShot[playerid][e_Hits] + 1);
			}
		} else if (s_LastShot[playerid][e_Hits] > 0) {
			// Sniper doesn't always send OnPlayerWeaponShot
			if (s_LastShot[playerid][e_Hits] >= 3 && weaponid != WEAPON_SNIPER) {
				valid = false;
				AddRejectedHit(playerid, damagedid, HIT_MULTIPLE_PLAYERS, weaponid, s_LastShot[playerid][e_Hits] + 1);
			} else {
				DebugMessageRed(playerid, "hit %d players with 1 shot", s_LastShot[playerid][e_Hits] + 1);
			}
		}

		if (valid) {
			new Float:dist = GetPlayerDistanceFromPoint(damagedid, s_LastShot[playerid][e_HX], s_LastShot[playerid][e_HY], s_LastShot[playerid][e_HZ]);

			if (dist > 20.0) {
				new in_veh = IsPlayerInAnyVehicle(damagedid) || GetPlayerSurfingVehicleID(damagedid) != INVALID_VEHICLE_ID;

				if ((!in_veh && GetPlayerSurfingObjectID(damagedid) == INVALID_OBJECT_ID) || dist > 50.0) {
					valid = false;
					AddRejectedHit(playerid, damagedid, HIT_TOO_FAR_FROM_SHOT, weaponid, _:dist);
				}
			}
		}

		s_LastShot[playerid][e_Hits] += 1;

		if (!valid) {
			return 0;
		}
	}

	if (npc) {
		OnPlayerDamageDone(damagedid, amount, playerid, weaponid, bodypart);
	} else {
		InflictDamage(damagedid, amount, playerid, weaponid, bodypart);
	}

	// Don't send OnPlayerGiveDamage to the rest of the script, since it should not be used
	return 0;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (IsPlayerNPC(playerid)) {
		return 0;
	}

	UpdateHealthBar(playerid, true);

	if (s_IsDying[playerid]) {
		return 0;
	}

	if (s_BeingResynced[playerid]) {
		return 0;
	}

	if (s_InClassSelection[playerid] || s_ForceClassSelection[playerid]) {
		return 0;
	}

	if (!IsHighRateWeapon(weaponid)) {
		DebugMessage(playerid, "OnPlayerTakeDamage(%d took %f from %d by %d on bodypart %d)", playerid, amount, issuerid, weaponid, bodypart);
	}

	// Ignore unreliable and invalid damage
	if (weaponid < WEAPON_UNARMED || weaponid >= sizeof(s_ValidDamageTaken) || !s_ValidDamageTaken[weaponid]) {
		return 0;
	}

	// Carjack damage
	if (weaponid == WEAPON_COLLISION && _:amount == _:0.0) {
		return 0;
	}

	// From stealth knife, can be any weaponid
	if (_:amount == _:1833.33154296875) {
		return 0;
	}

	// Climb bug
	if (weaponid == WEAPON_COLLISION) {
		if (s_CustomFallDamage) {
			return 0;
		}

		new anim = GetPlayerAnimationIndex(playerid);

		if (1061 <= anim <= 1067) {
			DebugMessage(playerid, "climb bug prevented");
			return 0;
		}
	} else if (weaponid == WEAPON_KNIFE) {
		// Being knifed client-side

		// With the plugin, this part is never actually used (it can't happen)
		if (_:amount == _:0.0) {
			if (s_KnifeTimeout[playerid] != -1) {
				KillTimer(s_KnifeTimeout[playerid]);

				s_KnifeTimeout[playerid] = -1;
			}

			if (issuerid == INVALID_PLAYER_ID || HasSameTeam(playerid, issuerid)) {
				ResyncPlayer(playerid);

				return 0;
			} else {
				new Float:x, Float:y, Float:z;
				GetPlayerPos(issuerid, x, y, z);

				if (GetPlayerDistanceFromPoint(playerid, x, y, z) > s_WeaponRange[weaponid] + 2.0) {
					ResyncPlayer(playerid);

					return 0;
				}
			}

			if (!OnPlayerDamage(playerid, amount, issuerid, weaponid, bodypart)) {
				ResyncPlayer(playerid);

				return 0;
			}

			// Make sure the values were not modified
			weaponid = WEAPON_KNIFE;
			amount = 0.0;

			s_DamageDoneHealth[playerid] = s_PlayerHealth[playerid];
			s_DamageDoneArmour[playerid] = s_PlayerArmour[playerid];

			OnPlayerDamageDone(playerid, s_PlayerHealth[playerid] + s_PlayerArmour[playerid], issuerid, weaponid, bodypart);

			new animlib[32] = "KNIFE", animname[32] = "KILL_Knife_Ped_Die";
			PlayerDeath(playerid, animlib, animname, _, 4000 - GetPlayerPing(playerid));

			WC_OnPlayerDeath(playerid, issuerid, weaponid);

			SetPlayerHealth(playerid, Float:0x7f7fffff);

			DebugMessage(playerid, "being knifed by %d", issuerid);
			DebugMessage(issuerid, "knifing %d", playerid);

			new Float:x, Float:y, Float:z, Float:a;

			GetPlayerFacingAngle(playerid, a);
			SetPlayerFacingAngle(issuerid, a);
			PosInFront(playerid, -1.0, x, y, z);

			SetPlayerVelocity(playerid, 0.0, 0.0, 0.0);
			SetPlayerVelocity(issuerid, 0.0, 0.0, 0.0);

			new forcesync = 2;

			if (GetPlayerAnimationIndex(issuerid) != 747) {
				DebugMessageRed(issuerid, "applying knife anim for you too (current: %d)", GetPlayerAnimationIndex(issuerid));

				forcesync = 1;
			}

			animname = "KILL_Knife_Player";
			ApplyAnimation(issuerid, animlib, animname, 4.1, 0, 1, 1, 0, 1800, forcesync);

			return 0;
		}
	}

	// If it's lagcomp, only allow damage that's valid for both modes
	if (s_LagCompMode && s_ValidDamageTaken[weaponid] != 2) {
		if (issuerid != INVALID_PLAYER_ID && GetPlayerState(issuerid) == PLAYER_STATE_DRIVER && (weaponid == WEAPON_M4 || weaponid == WEAPON_MINIGUN)) {
			weaponid = weaponid == WEAPON_M4 ? WEAPON_VEHICLE_M4 : WEAPON_VEHICLE_MINIGUN;
		} else {
			return 0;
		}
	}

	// Should still be damaged by grenades or fire after someone has died
	if (issuerid != INVALID_PLAYER_ID && IsPlayerConnected(issuerid)) {
		if (HasSameTeam(playerid, issuerid)) {
			return 0;
		}

		if (s_IsDying[issuerid] && (IsBulletWeapon(weaponid) || IsMeleeWeapon(weaponid)) && GetTickCount() - s_LastDeathTick[issuerid] > 80) {
			DebugMessageRed(playerid, "shot/punched by dead player (%d)", issuerid);
			return 0;
		}

		if (s_BeingResynced[issuerid]) {
			return 0;
		}

		// https://github.com/oscar-broman/samp-weapon-config/issues/104
		if (weaponid == WEAPON_COLLISION
		|| weaponid == WEAPON_DROWN) {
			return 0;
		}

		// https://github.com/oscar-broman/samp-weapon-config/issues/104
		if (weaponid == WEAPON_VEHICLE
		|| weaponid == WEAPON_HELIBLADES) {
			if (GetPlayerState(issuerid) != PLAYER_STATE_DRIVER) {
				return 0;
			}
		}

		// Will be applied on fire, explosion, vehicle and heliblades (carpark) damage
		// Both players should see eachother, if playerid claims to keep issuerid valid
		if ((!IsPlayerStreamedIn(playerid, issuerid) && !WC_IsPlayerPaused(issuerid)) || !IsPlayerStreamedIn(issuerid, playerid)) {
			// Probably fake or belated damage, so let's just reset issuerid
			issuerid = INVALID_PLAYER_ID;
		}
	}

	new Float:bullets = 0.0, err;

	if ((err = ProcessDamage(playerid, issuerid, amount, weaponid, bodypart, bullets))) {
		if (err == WC_INVALID_DAMAGE) {
			AddRejectedHit(issuerid, playerid, HIT_INVALID_DAMAGE, weaponid, _:amount);
		}

		if (err != WC_INVALID_DISTANCE) {
			OnInvalidWeaponDamage(issuerid, playerid, amount, weaponid, bodypart, err, false);
		}

		return 0;
	}

	if (IsBulletWeapon(weaponid)) {
		new Float:x, Float:y, Float:z, Float:dist;
		GetPlayerPos(issuerid, x, y, z);
		dist = GetPlayerDistanceFromPoint(playerid, x, y, z);

		if (dist > s_WeaponRange[weaponid] + 2.0) {
			AddRejectedHit(issuerid, playerid, HIT_OUT_OF_RANGE, weaponid, _:dist, _:s_WeaponRange[weaponid]);
			return 0;
		}
	}

	InflictDamage(playerid, amount, issuerid, weaponid, bodypart);

	return 0;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	#if WC_CUSTOM_VENDING_MACHINES
		if (s_VendingUseTimer[playerid] != -1) {
			KillTimer(s_VendingUseTimer[playerid]);
			s_VendingUseTimer[playerid] = -1;
		}
	#endif

	s_LastShot[playerid][e_Valid] = false;

	new tick = GetTickCount();
	if (tick == 0) tick = 1;

	if (s_CbugFroze[playerid] && tick - s_CbugFroze[playerid] < 900) {
		return 0;
	}

	s_CbugFroze[playerid] = 0;

	new damagedid = INVALID_PLAYER_ID;

	if (hittype == BULLET_HIT_TYPE_PLAYER && hitid != INVALID_PLAYER_ID) {
		if (!IsPlayerConnected(hitid)) {
			AddRejectedHit(playerid, hitid, HIT_DISCONNECTED, weaponid, hitid);

			return 0;
		}

		damagedid = hitid;
	}

	if (hittype < BULLET_HIT_TYPE_NONE || hittype > BULLET_HIT_TYPE_PLAYER_OBJECT) {
		AddRejectedHit(playerid, damagedid, HIT_INVALID_HITTYPE, weaponid, hittype);

		return 0;
	}

	#if WC_DEBUG
		if (hittype == BULLET_HIT_TYPE_PLAYER) {
			DebugMessage(playerid, "OnPlayerWeaponShot(%d shot %d with %d at %f, %f, %f)", playerid, hitid, weaponid, fX, fY, fZ);
		} else if (hittype) {
			DebugMessage(playerid, "OnPlayerWeaponShot(%d shot %d %d with %d at %f, %f, %f)", playerid, hittype, hitid, weaponid, fX, fY, fZ);
		} else {
			DebugMessage(playerid, "OnPlayerWeaponShot(%d shot with %d at %f, %f, %f)", playerid, weaponid, fX, fY, fZ);
		}
	#endif

	if (s_BeingResynced[playerid]) {
		AddRejectedHit(playerid, damagedid, HIT_BEING_RESYNCED, weaponid);

		return 0;
	}

	if (!WC_IsPlayerSpawned(playerid) && tick - s_LastDeathTick[playerid] > 80) {
		AddRejectedHit(playerid, damagedid, HIT_NOT_SPAWNED, weaponid);

		return 0;
	}

	if (!IsBulletWeapon(weaponid)) {
		AddRejectedHit(playerid, damagedid, HIT_INVALID_WEAPON, weaponid);

		return 0;
	}

	new Float:fOriginX, Float:fOriginY, Float:fOriginZ, Float:fHitPosX, Float:fHitPosY, Float:fHitPosZ;
	new Float:x, Float:y, Float:z;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerLastShotVectors(playerid, fOriginX, fOriginY, fOriginZ, fHitPosX, fHitPosY, fHitPosZ);

	new Float:length = VectorSize(fOriginX - fHitPosX, fOriginY - fHitPosY, fOriginZ - fHitPosZ);
	new Float:origin_dist = VectorSize(fOriginX - x, fOriginY - y, fOriginZ - z);

	if (origin_dist > 15.0) {
		new in_veh = IsPlayerInAnyVehicle(playerid) || GetPlayerSurfingVehicleID(playerid) != INVALID_VEHICLE_ID;

		if ((!in_veh && GetPlayerSurfingObjectID(playerid) == INVALID_OBJECT_ID) || origin_dist > 50.0) {
			AddRejectedHit(playerid, damagedid, HIT_TOO_FAR_FROM_ORIGIN, weaponid, _:origin_dist);

			return 0;
		}
	}

	// Shot exceeding the max range?
	if (hittype != BULLET_HIT_TYPE_NONE) {
		if (length > s_WeaponRange[weaponid]) {
			if (hittype == BULLET_HIT_TYPE_PLAYER) {
				AddRejectedHit(playerid, damagedid, HIT_OUT_OF_RANGE, weaponid, _:length, _:s_WeaponRange[weaponid]);
			}

			return 0;
		}

		if (hittype == BULLET_HIT_TYPE_PLAYER) {
			if (IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleID(playerid) == GetPlayerVehicleID(hitid)) {
				AddRejectedHit(playerid, damagedid, HIT_SAME_VEHICLE, weaponid);
				return 0;
			}

			new Float:dist = GetPlayerDistanceFromPoint(hitid, fHitPosX, fHitPosY, fHitPosZ);
			new in_veh = IsPlayerInAnyVehicle(hitid) || GetPlayerSurfingVehicleID(hitid) != INVALID_VEHICLE_ID;

			if (dist > 20.0) {
				if ((!in_veh && GetPlayerSurfingObjectID(hitid) == INVALID_OBJECT_ID) || dist > 50.0) {
					AddRejectedHit(playerid, damagedid, HIT_TOO_FAR_FROM_SHOT, weaponid, _:dist);

					return 0;
				}
			}
		}
	}

	new idx = (s_LastShotIdx[playerid] + 1) % sizeof(s_LastShotTicks[]);

	// JIT plugin fix
	if (idx < 0) {
		idx += sizeof(s_LastShotTicks[]);
	}

	s_LastShotIdx[playerid] = idx;
	s_LastShotTicks[playerid][idx] = tick;
	s_LastShotWeapons[playerid][idx] = weaponid;
	s_ShotsFired[playerid] += 1;

	#if WC_DEBUG
		if (s_ShotsFired[playerid] > 1) {
			new prev_tick_idx = (idx - 1) % sizeof(s_LastShotTicks[]);

			// JIT plugin fix
			if (prev_tick_idx < 0) {
				prev_tick_idx += sizeof(s_LastShotTicks[]);
			}

			new prev_tick = s_LastShotTicks[playerid][prev_tick_idx];

			DebugMessage(playerid, "(shot) last: %d last 3: %d", tick - prev_tick, AverageShootRate(playerid, 3));
		}
	#endif

	s_LastShot[playerid][e_Tick] = tick;
	s_LastShot[playerid][e_Weapon] = weaponid;
	s_LastShot[playerid][e_HitType] = hittype;
	s_LastShot[playerid][e_HitId] = hitid;
	s_LastShot[playerid][e_X] = fX;
	s_LastShot[playerid][e_Y] = fY;
	s_LastShot[playerid][e_Z] = fZ;
	s_LastShot[playerid][e_OX] = fOriginX;
	s_LastShot[playerid][e_OY] = fOriginY;
	s_LastShot[playerid][e_OZ] = fOriginZ;
	s_LastShot[playerid][e_HX] = fHitPosX;
	s_LastShot[playerid][e_HY] = fHitPosY;
	s_LastShot[playerid][e_HZ] = fHitPosZ;
	s_LastShot[playerid][e_Length] = length;
	s_LastShot[playerid][e_Hits] = 0;

	new multiple_weapons;
	new avg_rate = AverageShootRate(playerid, s_MaxShootRateSamples, multiple_weapons);

	// Bullet flood?
	// Could be either a cheat or just lag
	if (avg_rate != -1) {
		if (multiple_weapons) {
			if (avg_rate < 100) {
				AddRejectedHit(playerid, damagedid, SHOOTING_RATE_TOO_FAST_MULTIPLE, weaponid, avg_rate, s_MaxShootRateSamples);
				return 0;
			}
		} else if (s_MaxWeaponShootRate[weaponid] - avg_rate > 20) {
			AddRejectedHit(playerid, damagedid, SHOOTING_RATE_TOO_FAST, weaponid, avg_rate, s_MaxShootRateSamples, s_MaxWeaponShootRate[weaponid]);
			return 0;
		}
	}

	// Destroy vehicles with passengers in them
	if (hittype == BULLET_HIT_TYPE_VEHICLE) {
		if (hitid < 0 || hitid > MAX_VEHICLES || !WC_IsValidVehicle(hitid)) {
			AddRejectedHit(playerid, damagedid, HIT_INVALID_VEHICLE, weaponid, hitid);
			return 0;
		}

		new vehicleid = GetPlayerVehicleID(playerid);

		// Shouldn't be possible to damage the vehicle you're in
		if (hitid == vehicleid) {
			AddRejectedHit(playerid, damagedid, HIT_OWN_VEHICLE, weaponid);
			return 0;
		}

		if (s_VehiclePassengerDamage) {
			new has_driver = false;
			new has_passenger = false;
			new seat;

			#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
			foreach (new otherid : Player) {
			#else
			for (new otherid = GetPlayerPoolSize(); otherid != -1; otherid--) {
			#endif
				if (otherid == playerid) {
					continue;
				}

				if (GetPlayerVehicleID(otherid) != hitid) {
					continue;
				}

				seat = GetPlayerVehicleSeat(otherid);

				if (seat == 0) {
					has_driver = true;
				} else {
					has_passenger = true;
				}
			}

			if (!has_driver && has_passenger) {
				new Float:health;

				GetVehicleHealth(hitid, health);

				if (WEAPON_SHOTGUN <= weaponid <= WEAPON_SHOTGSPA) {
					health -= 120.0;
				} else {
					health -= s_WeaponDamage[weaponid] * 3.0;
				}

				if (health <= 0.0) {
					health = 0.0;
				}

				SetVehicleHealth(hitid, health);
			}
		}

		if (s_VehicleUnoccupiedDamage) {
			new has_occupent = false;

			#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
			foreach (new otherid : Player) {
			#else
			for (new otherid = GetPlayerPoolSize(); otherid != -1; otherid--) {
			#endif
				if (otherid == playerid) {
					continue;
				}

				if (GetPlayerVehicleID(otherid) != hitid) {
					continue;
				}

				has_occupent = true;
			}

			if (!has_occupent) {
				new Float:health;

				GetVehicleHealth(hitid, health);
				if (health >= 250.0) { //vehicles start on fire below 250 hp
					if (WEAPON_SHOTGUN <= weaponid <= WEAPON_SHOTGSPA) {
						health -= 120.0;
					} else {
						health -= s_WeaponDamage[weaponid] * 3.0;
					}

					if (health < 250.0) {
						if (s_VehicleRespawnTimer[hitid] == -1) {
							health = 249.0;
							s_VehicleRespawnTimer[hitid] = SetTimerEx("WC_KillVehicle", 6000, false, "ii", hitid, playerid);
						}
					}

					SetVehicleHealth(hitid, health);
				}
			}
		}
	}

	new retval = WC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);

	s_LastShot[playerid][e_Valid] = !!retval;

	// Valid shot?
	if (retval) {
		if (hittype == BULLET_HIT_TYPE_VEHICLE) {
			s_LastVehicleShooter[hitid] = playerid;
		}
	}

	return retval;
}

forward WC_KillVehicle(vehicleid, killerid);
public WC_KillVehicle(vehicleid, killerid)
{
	OnVehicleDeath(vehicleid, killerid);
	s_VehicleRespawnTimer[vehicleid] = SetTimerEx("WC_OnDeadVehicleSpawn", 10000, false, "i", vehicleid);
	return 1;
}

forward WC_OnDeadVehicleSpawn(vehicleid);
public WC_OnDeadVehicleSpawn(vehicleid)
{
	s_VehicleRespawnTimer[vehicleid] = -1;
	return SetVehicleToRespawn(vehicleid);
}

public OnVehicleSpawn(vehicleid)
{
	if (s_VehicleRespawnTimer[vehicleid] != -1) {
		KillTimer(s_VehicleRespawnTimer[vehicleid]);
		s_VehicleRespawnTimer[vehicleid] = -1;
	}

	s_VehicleAlive[vehicleid] = true;
	s_LastVehicleShooter[vehicleid] = INVALID_PLAYER_ID;

	return WC_OnVehicleSpawn(vehicleid);
}

public OnVehicleDeath(vehicleid, killerid)
{
	if (s_VehicleRespawnTimer[vehicleid] != -1) {
		KillTimer(s_VehicleRespawnTimer[vehicleid]);
		s_VehicleRespawnTimer[vehicleid] = -1;
	}
	if (s_VehicleAlive[vehicleid]) {
		s_VehicleAlive[vehicleid] = false;

		return WC_OnVehicleDeath(vehicleid, killerid);
	}
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	if (!WC_IsPlayerSpawned(playerid)) {
		return 1;
	}

	return WC_OnPlayerEnterCheckpoint(playerid);
}

public OnPlayerLeaveCheckpoint(playerid)
{
	// If they're dying, it will be called in PlayerDeath (when the death anim begins)
	if (s_IsDying[playerid]) {
		return 1;
	}

	return WC_OnPlayerLeaveCheckpoint(playerid);
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	if (!WC_IsPlayerSpawned(playerid)) {
		return 1;
	}

	return WC_OnPlayerEnterRaceCheckpoint(playerid);
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	// If they're dying, it will be called in PlayerDeath (when the death anim begins)
	if (s_IsDying[playerid]) {
		return 1;
	}

	return WC_OnPlayerLeaveRaceCheckpoint(playerid);
}

/*
 * Internal functions
 */

static ScriptInit()
{
	s_LagCompMode = GetServerVarAsInt("lagcompmode");

	if (s_LagCompMode) {
		SetKnifeSync(false);
	} else {
		SetKnifeSync(true);
	}

	for (new i = 0; i < sizeof(s_ClassSpawnInfo); i++) {
		s_ClassSpawnInfo[i][e_Skin] = -1;
	}

	s_HealthBarBorder = TextDrawCreate(546.2, 66.4, "LD_SPAC:white");

	if (s_HealthBarBorder == Text:INVALID_TEXT_DRAW) {
		printf("(wc) WARN: Unable to create healthbar border textdraw");
	} else {
		s_InternalTextDraw[s_HealthBarBorder] = true;

		TextDrawTextSize		(s_HealthBarBorder, 61.5, 8.8);
		TextDrawAlignment		(s_HealthBarBorder, 1);
		TextDrawColor			(s_HealthBarBorder, 255);
		TextDrawSetShadow		(s_HealthBarBorder, 0);
		TextDrawBackgroundColor	(s_HealthBarBorder, 255);
		TextDrawFont			(s_HealthBarBorder, 4);
		TextDrawSetProportional	(s_HealthBarBorder, 0);
	}

	s_HealthBarBackground[0] = TextDrawCreate(548.2, 68.6, "LD_SPAC:white");

	if (s_HealthBarBackground[0] == Text:INVALID_TEXT_DRAW) {
		printf("(wc) WARN: Unable to create healthbar background textdraw");
	} else {
		s_InternalTextDraw[s_HealthBarBackground[0]] = true;

		TextDrawTextSize		(s_HealthBarBackground[0], 57.6, 5.0);
		TextDrawAlignment		(s_HealthBarBackground[0], 1);
		TextDrawColor			(s_HealthBarBackground[0], WC_HEALTH_BAR_BG_COLOR);
		TextDrawSetShadow		(s_HealthBarBackground[0], 0);
		TextDrawBackgroundColor	(s_HealthBarBackground[0], 255);
		TextDrawFont			(s_HealthBarBackground[0], 4);
		TextDrawSetProportional	(s_HealthBarBackground[0], 0);
	}

	s_HealthBarBackground[1] = TextDrawCreate(517.20, 65.000, "_");

	if (s_HealthBarBackground[1] == Text:INVALID_TEXT_DRAW) {
		printf("(wc) WARN: Unable to create healthbar background textdraw");
	} else {
		s_InternalTextDraw[s_HealthBarBackground[1]] = true;

		TextDrawFont(s_HealthBarBackground[1], 1);
		TextDrawLetterSize(s_HealthBarBackground[1], 0.733332, 1.1200000);
		TextDrawTextSize(s_HealthBarBackground[1], 561.500, 28.000);
		TextDrawSetOutline(s_HealthBarBackground[1], 1);
		TextDrawSetShadow(s_HealthBarBackground[1], 0);
		TextDrawAlignment(s_HealthBarBackground[1], 1);
		TextDrawColor(s_HealthBarBackground[1], WC_HEALTH_BAR_BG_COLOR);
		TextDrawBackgroundColor(s_HealthBarBackground[1], 255);
		TextDrawBoxColor(s_HealthBarBackground[1], 255);
		TextDrawUseBox(s_HealthBarBackground[1], 1);
		TextDrawSetProportional(s_HealthBarBackground[1], 1);
		TextDrawSetSelectable(s_HealthBarBackground[1], 0);
	}

	#if WC_CUSTOM_VENDING_MACHINES
		if (s_CustomVendingMachines) {
			CreateVendingMachines();
		}
	#endif

	new worldid, tick = GetTickCount();

	#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
	foreach (new playerid : Player) {
	#else
	for (new playerid = GetPlayerPoolSize(); playerid != -1; playerid--) {
	#endif
		s_PlayerTeam[playerid] = GetPlayerTeam(playerid);

		SetPlayerTeam(playerid, s_PlayerTeam[playerid]);
		DamageFeedUpdate(playerid);

		worldid = GetPlayerVirtualWorld(playerid);

		if (worldid == WC_DEATH_WORLD) {
			worldid = 0;

			SetPlayerVirtualWorld(playerid, worldid);
		}

		s_World[playerid] = worldid;
		s_LastUpdate[playerid] = tick;
		s_DamageFeedLastUpdate[playerid] = tick;
		s_LastStop[playerid] = tick;
		s_LastVehicleEnterTime[playerid] = 0;
		s_TrueDeath[playerid] = true;
		s_InClassSelection[playerid] = true;
		s_PlayerFallbackSpawnInfo[playerid][e_Skin] = -1;
		s_AlreadyConnected[playerid] = true;

		#if WC_CUSTOM_VENDING_MACHINES
			RemoveDefaultVendingMachines(playerid);
		#endif

		if (PLAYER_STATE_ONFOOT <= GetPlayerState(playerid) <= PLAYER_STATE_PASSENGER) {
			GetPlayerHealth(playerid, s_PlayerHealth[playerid]);
			GetPlayerArmour(playerid, s_PlayerArmour[playerid]);

			if (s_PlayerHealth[playerid] == 0.0) {
				s_PlayerHealth[playerid] = s_PlayerMaxHealth[playerid];
			}

			UpdateHealthBar(playerid);
		}

		switch (GetPlayerState(playerid)) {
			case PLAYER_STATE_ONFOOT,
			     PLAYER_STATE_DRIVER,
			     PLAYER_STATE_PASSENGER,
			     PLAYER_STATE_SPAWNED: {
				SetHealthBarVisible(playerid, true);
			}

			default: {
				SetHealthBarVisible(playerid, false);
			}
		}
	}
}

static ScriptExit()
{
	SetKnifeSync(true);

	#if WC_CUSTOM_VENDING_MACHINES
		DestroyVendingMachines();
	#endif

	new Float:health;

	#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
	foreach (new playerid : Player) {
	#else
	for (new playerid = GetPlayerPoolSize(); playerid != -1; playerid--) {
	#endif
		#if WC_CUSTOM_VENDING_MACHINES
			if (s_VendingUseTimer[playerid] != -1) {
				KillTimer(s_VendingUseTimer[playerid]);
				s_VendingUseTimer[playerid] = -1;
			}
		#endif

		// Put things back the way they were
		SetPlayerTeam(playerid, s_PlayerTeam[playerid]);

		if (PLAYER_STATE_ONFOOT <= GetPlayerState(playerid) <= PLAYER_STATE_PASSENGER) {
			health = s_PlayerHealth[playerid];

			if (health == 0.0) {
				health = s_PlayerMaxHealth[playerid];
			}

			SetPlayerHealth(playerid, health);
			SetPlayerArmour(playerid, s_PlayerArmour[playerid]);
		}

		SetFakeHealth(playerid, 255);
		SetFakeArmour(playerid, 255);
		FreezeSyncPacket(playerid, .toggle = false);
		SetFakeFacingAngle(playerid, _);
		SetHealthBarVisible(playerid, false);

		if (s_HealthBarForeground[playerid] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, s_HealthBarForeground[playerid]);
			s_InternalPlayerTextDraw[playerid][s_HealthBarForeground[playerid]] = false;
			s_HealthBarForeground[playerid] = PlayerText:INVALID_TEXT_DRAW;
		}

		if (s_DamageFeedGiven[playerid] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, s_DamageFeedGiven[playerid]);
			s_InternalPlayerTextDraw[playerid][s_DamageFeedGiven[playerid]] = false;
			s_DamageFeedGiven[playerid] = PlayerText:INVALID_TEXT_DRAW;
		}

		if (s_DamageFeedTaken[playerid] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, s_DamageFeedTaken[playerid]);
			s_InternalPlayerTextDraw[playerid][s_DamageFeedTaken[playerid]] = false;
			s_DamageFeedTaken[playerid] = PlayerText:INVALID_TEXT_DRAW;
		}
	}

	if (s_HealthBarBorder != Text:INVALID_TEXT_DRAW) {
		TextDrawDestroy(s_HealthBarBorder);
		s_InternalTextDraw[s_HealthBarBorder] = false;
	}

	for(new i = 0; i < 2; i++) {
		if (s_HealthBarBackground[i] != Text:INVALID_TEXT_DRAW) {
			TextDrawDestroy(s_HealthBarBackground[i]);
			s_InternalTextDraw[s_HealthBarBackground[i]] = false;
		}
	}
}

static UpdatePlayerVirtualWorld(playerid)
{
	new worldid = GetPlayerVirtualWorld(playerid);

	if (worldid == WC_DEATH_WORLD) {
		worldid = s_World[playerid];
	} else if (worldid != s_World[playerid]) {
		s_World[playerid] = worldid;
	}

	SetPlayerVirtualWorld(playerid, worldid);
}

static HasSameTeam(playerid, otherid)
{
	if (otherid < 0 || otherid >= MAX_PLAYERS || playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (s_PlayerTeam[playerid] == NO_TEAM || s_PlayerTeam[otherid] == NO_TEAM) {
		return 0;
	}

	return (s_PlayerTeam[playerid] == s_PlayerTeam[otherid]);
}

stock WC_IsPlayerPaused(playerid)
{
	return (GetTickCount() - s_LastUpdate[playerid] > 2000);
}

static Float:WC_Bar_Calculate(Float:width, Float:max, Float:value)
{
	return ((width / max) * value);
}

static UpdateHealthBar(playerid, bool:force = false)
{
	if (s_BeingResynced[playerid] || s_ForceClassSelection[playerid]) {
		return;
	}

	new health = floatround(s_PlayerHealth[playerid] / s_PlayerMaxHealth[playerid] * 100.0, floatround_ceil);
	new armour = floatround(s_PlayerArmour[playerid] / s_PlayerMaxArmour[playerid] * 100.0, floatround_ceil);

	// Make the values reflect what the client should see
	if (s_IsDying[playerid]) {
		health = 0;
		armour = 0;
	} else {
		if (health > 100) {
			health = 100;
		}

		if (armour > 100) {
			armour = 100;
		}
	}

	if (force) {
		s_LastSentHealth[playerid] = -1;
		s_LastSentArmour[playerid] = -1;
	} else if (s_HealthBarVisible[playerid] && s_HealthBarForeground[playerid] == PlayerText:INVALID_TEXT_DRAW && !s_IsDying[playerid]) {
		s_LastSentHealth[playerid] = -1;
	} else if (health == s_LastSentHealth[playerid] && armour == s_LastSentArmour[playerid]) {
		return;
	}

	SetFakeHealth(playerid, health);
	SetFakeArmour(playerid, armour);

	UpdateSyncData(playerid);

	if (health != s_LastSentHealth[playerid]) {
		s_LastSentHealth[playerid] = health;

		SetPlayerHealth(playerid, 8000000.0 + float(health));

		if (s_HealthBarVisible[playerid] && !s_IsDying[playerid]) {
			new bool:isAndroid = GetPVarInt(playerid, "NotAndroid") == 0;

			if (s_HealthBarForeground[playerid] == PlayerText:INVALID_TEXT_DRAW) {
				if(isAndroid) {
					s_HealthBarForeground[playerid] = CreatePlayerTextDraw(
						playerid,
						517.20,
						65.00,
						"_"
					);

					PlayerTextDrawTextSize(playerid,
						s_HealthBarForeground[playerid],
						514.5000 + 47.0 * (health / 100.0),
						28.000
					);
				}
				else {
					s_HealthBarForeground[playerid] = CreatePlayerTextDraw(
						playerid,
						548.2,
						68.6,
						"LD_SPAC:white"
					);
					PlayerTextDrawTextSize(playerid,
						s_HealthBarForeground[playerid],
						WC_Bar_Calculate(
						57.6,
						100.0,
						float(health)),
						5.0
					);
				}

				if (s_HealthBarForeground[playerid] == PlayerText:INVALID_TEXT_DRAW) {
					printf("(wc) WARN: Unable to create player healthbar foreground");
				} else {
					s_InternalPlayerTextDraw[playerid][s_HealthBarForeground[playerid]] = true;

					if(isAndroid) {
						PlayerTextDrawFont(playerid, s_HealthBarForeground[playerid], 1);
						PlayerTextDrawLetterSize(playerid, s_HealthBarForeground[playerid], 0.733332, 1.1200000);
						PlayerTextDrawSetOutline(playerid, s_HealthBarForeground[playerid], 1);
						PlayerTextDrawSetShadow(playerid, s_HealthBarForeground[playerid], 0);
						PlayerTextDrawAlignment(playerid, s_HealthBarForeground[playerid], 1);
						PlayerTextDrawColor(playerid, s_HealthBarForeground[playerid], -1);
						PlayerTextDrawBackgroundColor(playerid, s_HealthBarForeground[playerid], 255);
						PlayerTextDrawBoxColor(playerid, s_HealthBarForeground[playerid], WC_HEALTH_BAR_FG_COLOR);
						PlayerTextDrawUseBox(playerid, s_HealthBarForeground[playerid], 1);
						PlayerTextDrawSetProportional(playerid, s_HealthBarForeground[playerid], 1);
						PlayerTextDrawSetSelectable(playerid, s_HealthBarForeground[playerid], 0);
						PlayerTextDrawShow(playerid, 			s_HealthBarForeground[playerid]);
					}
					else {
						PlayerTextDrawAlignment(playerid, 		s_HealthBarForeground[playerid], 1);
						PlayerTextDrawColor(playerid, 			s_HealthBarForeground[playerid], WC_HEALTH_BAR_FG_COLOR);
						PlayerTextDrawSetShadow(playerid, 		s_HealthBarForeground[playerid], 0);
						PlayerTextDrawBackgroundColor(playerid, s_HealthBarForeground[playerid], 255);
						PlayerTextDrawFont(playerid, 			s_HealthBarForeground[playerid], 4);
						PlayerTextDrawSetProportional(playerid, s_HealthBarForeground[playerid], 0);
						PlayerTextDrawShow(playerid, 			s_HealthBarForeground[playerid]);
					}
				}
			} else if (s_InternalPlayerTextDraw[playerid][s_HealthBarForeground[playerid]]) {
				if(isAndroid) {
					PlayerTextDrawTextSize(playerid,
						s_HealthBarForeground[playerid],
						514.5000 + 47.0 * (health / 100.00),
						28.000
					);
				}
				else {
					PlayerTextDrawTextSize(playerid,
						s_HealthBarForeground[playerid],
						WC_Bar_Calculate(
						57.6,
						100.0,
						float(health)),
						5.0
					);
				}
				PlayerTextDrawShow(playerid, s_HealthBarForeground[playerid]);
			}

		} else if (s_HealthBarForeground[playerid] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, s_HealthBarForeground[playerid]);
			s_InternalPlayerTextDraw[playerid][s_HealthBarForeground[playerid]] = false;
			s_HealthBarForeground[playerid] = PlayerText:INVALID_TEXT_DRAW;
		}
	}

	if (armour != s_LastSentArmour[playerid]) {
		s_LastSentArmour[playerid] = armour;

		SetPlayerArmour(playerid, float(armour));
	}
}

static SetHealthBarVisible(playerid, bool:toggle)
{
	if (!s_EnableHealthBar[playerid]) {
		toggle = false;
	}
	if (s_HealthBarVisible[playerid] == toggle) {
		return;
	}

	s_HealthBarVisible[playerid] = toggle;

	new healthBarBackgroundIndex = GetPVarInt(playerid, "NotAndroid") == 0 ? 1 : 0;
	if (toggle) {
		if (s_HealthBarBorder != Text:INVALID_TEXT_DRAW && GetPVarInt(playerid, "NotAndroid") == 1) {
			TextDrawShowForPlayer(playerid, s_HealthBarBorder);
		}

		if (s_HealthBarBackground[healthBarBackgroundIndex] != Text:INVALID_TEXT_DRAW) {
			TextDrawShowForPlayer(playerid, s_HealthBarBackground[healthBarBackgroundIndex]);
		}

		UpdateHealthBar(playerid, true);
	} else {
		if (s_HealthBarForeground[playerid] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawHide(playerid, s_HealthBarForeground[playerid]);
		}

		if (s_HealthBarBorder != Text:INVALID_TEXT_DRAW && GetPVarInt(playerid, "NotAndroid") == 1) {
			TextDrawHideForPlayer(playerid, s_HealthBarBorder);
		}

		if (s_HealthBarBackground[healthBarBackgroundIndex] != Text:INVALID_TEXT_DRAW) {
			TextDrawHideForPlayer(playerid, s_HealthBarBackground[healthBarBackgroundIndex]);
		}
	}
}

static SpawnPlayerInPlace(playerid) {
	new Float:x, Float:y, Float:z, Float:r;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, r);

	SetSpawnInfo(playerid, s_PlayerTeam[playerid], GetPlayerSkin(playerid), x, y, z, r, 0, 0, 0, 0, 0, 0);

	s_SpawnInfoModified[playerid] = true;

	SpawnPlayer(playerid);
}

static PosInFront(playerid, Float:distance, &Float:x, &Float:y, &Float:z)
{
	new Float:a;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	x += (distance * floatsin(-a, degrees));
	y += (distance * floatcos(-a, degrees));
}

static Float:AngleBetweenPoints(Float:x1, Float:y1, Float:x2, Float:y2)
{
	return -(90.0 - atan2(y1 - y2, x1 - x2));
}

static UpdateSyncData(playerid)
{
	// Currently re-sending onfoot data is only supported
	if (!IsPlayerConnected(playerid) || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) {
		return;
	}

	#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
	foreach (new i : Player) {
	#else
	for (new i = GetPlayerPoolSize(); i != -1; i--) {
	#endif
		if (i != playerid && IsPlayerStreamedIn(playerid, i)) {
			SendLastSyncPacket(playerid, i);
		}
	}
}

static WasPlayerInVehicle(playerid, time) {
	if (!s_LastVehicleTick[playerid]) {
		return 0;
	}

	if (GetTickCount() - time < s_LastVehicleTick[playerid]) {
		return 1;
	}

	return 0;
}

#if WC_CUSTOM_VENDING_MACHINES
	static RemoveDefaultVendingMachines(playerid)
	{
		RemoveBuildingForPlayer(playerid, 955, 0.0, 0.0, 0.0, 20000.0); // CJ_EXT_SPRUNK
		RemoveBuildingForPlayer(playerid, 956, 0.0, 0.0, 0.0, 20000.0); // CJ_EXT_CANDY
		RemoveBuildingForPlayer(playerid, 1209, 0.0, 0.0, 0.0, 20000.0); // vendmach
		RemoveBuildingForPlayer(playerid, 1302, 0.0, 0.0, 0.0, 20000.0); // vendmachfd
		RemoveBuildingForPlayer(playerid, 1775, 0.0, 0.0, 0.0, 20000.0); // CJ_SPRUNK1
		RemoveBuildingForPlayer(playerid, 1776, 0.0, 0.0, 0.0, 20000.0); // CJ_CANDYVENDOR
		RemoveBuildingForPlayer(playerid, 1977, 0.0, 0.0, 0.0, 20000.0); // vendin3

		// Make sure they're all gone..
		for (new i = 0; i < sizeof(sc_VendingMachines); i++) {
			RemoveBuildingForPlayer(
				playerid,
				sc_VendingMachines[i][e_Model],
				sc_VendingMachines[i][e_PosX],
				sc_VendingMachines[i][e_PosY],
				sc_VendingMachines[i][e_PosZ],
				1.0
			);
		}
	}

	static CreateVendingMachines()
	{
		DestroyVendingMachines();

		for (new i = 0; i < sizeof(sc_VendingMachines); i++) {
			#if WC_USE_STREAMER
				s_VendingMachineObject[i] = CreateDynamicObject(
					sc_VendingMachines[i][e_Model],
					sc_VendingMachines[i][e_PosX],
					sc_VendingMachines[i][e_PosY],
					sc_VendingMachines[i][e_PosZ],
					sc_VendingMachines[i][e_RotX],
					sc_VendingMachines[i][e_RotY],
					sc_VendingMachines[i][e_RotZ],
					.interiorid = sc_VendingMachines[i][e_Interior]
				);
			#else
				s_VendingMachineObject[i] = CreateObject(
					sc_VendingMachines[i][e_Model],
					sc_VendingMachines[i][e_PosX],
					sc_VendingMachines[i][e_PosY],
					sc_VendingMachines[i][e_PosZ],
					sc_VendingMachines[i][e_RotX],
					sc_VendingMachines[i][e_RotY],
					sc_VendingMachines[i][e_RotZ]
				);
			#endif
		}
	}

	static DestroyVendingMachines()
	{
		for (new i = 0; i < sizeof(s_VendingMachineObject); i++) {
			#if WC_USE_STREAMER
				if (s_VendingMachineObject[i] != INVALID_STREAMER_ID) {
					DestroyDynamicObject(s_VendingMachineObject[i]);
					s_VendingMachineObject[i] = INVALID_STREAMER_ID;
				}
			#else
				if (s_VendingMachineObject[i] != -1) {
					DestroyObject(s_VendingMachineObject[i]);
					s_VendingMachineObject[i] = -1;
				}
			#endif
		}
	}
#endif

forward WC_DeathSkipEnd(playerid);
public WC_DeathSkipEnd(playerid)
{
	TogglePlayerControllable(playerid, true);

	ResetPlayerWeapons(playerid);

	for (new i = 0; i < 13; i++) {
		if (s_SyncData[playerid][e_WeaponId][i]) {
			GivePlayerWeapon(playerid, s_SyncData[playerid][e_WeaponId][i], s_SyncData[playerid][e_WeaponAmmo][i]);
		}
	}

	SetPlayerArmedWeapon(playerid, s_SyncData[playerid][e_Weapon]);
	SetPlayerSpecialAction(playerid, 0);
}

forward WC_SpawnForStreamedIn(playerid);
public WC_SpawnForStreamedIn(playerid)
{
	if (!IsPlayerConnected(playerid)) {
		return;
	}

	SpawnPlayerForWorld(playerid);

	#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
	foreach (new i : Player) {
	#else
	for (new i = GetPlayerPoolSize(); i != -1; i--) {
	#endif
		if (i != playerid && IsPlayerStreamedIn(playerid, i)) {
			SendLastSyncPacket(playerid, i);
			ClearAnimationsForPlayer(playerid, i);
		}
	}
}

forward WC_SetSpawnForStreamedIn(playerid);
public WC_SetSpawnForStreamedIn(playerid)
{
	s_SpawnForStreamedIn[playerid] = true;
}

static ProcessDamage(&playerid, &issuerid, &Float:amount, &weaponid, &bodypart, &Float:bullets)
{
	if (amount < 0.0) {
		return WC_INVALID_DAMAGE;
	}

	// Adjust invalid amounts caused by an animation bug
	switch (amount) {
		case 3.63000011444091796875,
		     5.940000057220458984375,
		     5.610000133514404296875: {
			amount = 2.6400001049041748046875;
		}

		case 3.30000019073486328125: {
			if (weaponid != WEAPON_SHOTGUN && weaponid != WEAPON_SAWEDOFF) {
				amount = 2.6400001049041748046875;
			}
		}

		case 4.950000286102294921875: {
			if (IsMeleeWeapon(weaponid)) {
				amount = 2.6400001049041748046875;
			}
		}

		case 6.270000457763671875,
		     6.93000030517578125,
		     7.2600002288818359375,
		     7.9200000762939453125,
		     8.5799999237060546875,
		     9.24000072479248046875,
		     11.88000011444091796875,
		     11.22000026702880859375: {
			amount = 2.6400001049041748046875;
		}

		case 9.90000057220458984375: {
			switch (weaponid) {
				case WEAPON_VEHICLE, WEAPON_VEHICLE_M4, WEAPON_AK47,
				     WEAPON_M4, WEAPON_SHOTGUN, WEAPON_SAWEDOFF, WEAPON_SHOTGSPA: {}

				default: {
					amount = 6.6000003814697265625;
				}
			}
		}
	}

	// Car parking
	if (weaponid == WEAPON_HELIBLADES && _:amount != _:330.0) {
		weaponid = WEAPON_CARPARK;
	}

	// Finish processing drown/fire/carpark quickly, since they are sent at very high rates
	if (IsHighRateWeapon(weaponid)) {
		// Apply reasonable bounds
		if (weaponid == WEAPON_DROWN) {
			if (amount > 10.0) amount = 10.0;
		} else if (amount > 1.0) {
			amount = 1.0;
		}

		// Adjust the damage if the multiplier is not 1.0
		if (_:s_WeaponDamage[weaponid] != _:1.0) {
			amount *= s_WeaponDamage[weaponid];
		}

		// Make sure the distance and issuer is valid; carpark can be self-inflicted so it doesn't require an issuer
		if (weaponid == WEAPON_SPRAYCAN || weaponid == WEAPON_FIREEXTINGUISHER || (weaponid == WEAPON_CARPARK && issuerid != INVALID_PLAYER_ID)) {
			if (issuerid == INVALID_PLAYER_ID) {
				return WC_NO_ISSUER;
			}

			new Float:x, Float:y, Float:z, Float:dist;
			GetPlayerPos(issuerid, x, y, z);
			dist = GetPlayerDistanceFromPoint(playerid, x, y, z);

			if (weaponid == WEAPON_CARPARK) {
				if (dist > 15.0) {
					AddRejectedHit(issuerid, playerid, HIT_TOO_FAR_FROM_ORIGIN, weaponid, _:dist);
					return WC_INVALID_DISTANCE;
				}
			} else {
				if (dist > s_WeaponRange[weaponid] + 2.0) {
					AddRejectedHit(issuerid, playerid, HIT_TOO_FAR_FROM_ORIGIN, weaponid, _:dist, _:s_WeaponRange[weaponid]);
					return WC_INVALID_DISTANCE;
				}
			}
		}

		return WC_NO_ERROR;
	}

	// Bullet or melee damage must have an issuerid, otherwise something has gone wrong (e.g. sniper bug)
	if (issuerid == INVALID_PLAYER_ID && (IsBulletWeapon(weaponid) || IsMeleeWeapon(weaponid))) {
		return WC_NO_ISSUER;
	}

	// Punching with a parachute
	if (weaponid == WEAPON_PARACHUTE) {
		weaponid = WEAPON_UNARMED;
	} else if (weaponid == WEAPON_COLLISION) {
		// Collision damage should never be above 165

		if (amount > 165.0) {
			amount = 1.0;
		} else {
			amount /= 165.0;
		}
	} else if (weaponid == WEAPON_EXPLOSION) {
		// Explosions do at most 82.5 damage. This will later be multipled by the damage value
		amount /= 82.5;

		// Figure out what caused the explosion
		if (issuerid != INVALID_PLAYER_ID) {
			if (GetPlayerState(issuerid) == PLAYER_STATE_DRIVER) {
				weaponid = WEAPON_VEHICLE_ROCKETLAUNCHER;
			} else if (s_LastExplosive[issuerid]) {
				weaponid = s_LastExplosive[issuerid];
			}
		} else if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
			weaponid = WEAPON_VEHICLE_ROCKETLAUNCHER;
		}
	}

	// Check for pistol whip
	switch (weaponid) {
		case WEAPON_COLT45 .. WEAPON_SNIPER,
		     WEAPON_MINIGUN, WEAPON_SPRAYCAN, WEAPON_FIREEXTINGUISHER: {
			// A pistol whip inflicts 2.64 damage
			if (_:amount == _:2.6400001049041748046875) {
				// Save the weapon in the bodypart argument (it's always BODY_PART_TORSO)
				bodypart = weaponid;
				weaponid = WEAPON_PISTOLWHIP;
			}
		}
	}

	new melee = IsMeleeWeapon(weaponid);

	// Can't punch from a vehicle
	if (melee && IsPlayerInAnyVehicle(issuerid)) {
		return WC_INVALID_DAMAGE;
	}

	if (weaponid != WEAPON_PISTOLWHIP) {
		switch (amount) {
			case 1.32000005245208740234375,
			     1.650000095367431640625,
			     1.980000019073486328125,
			     2.3100001811981201171875,
			     2.6400001049041748046875,
			     2.9700000286102294921875,
			     3.96000003814697265625,
			     4.28999996185302734375,
			     4.62000036239624023437,
			     5.280000209808349609375: {
				// Damage is most likely from punching and switching weapon quickly
				if (!melee) {
					DebugMessage(issuerid, "weapon changed from %d to melee (punch & swap)", weaponid);
					weaponid = WEAPON_UNARMED;
					melee = true;
				}
			}

			case 6.6000003814697265625: {
				if (!melee) {
					switch (weaponid) {
						case WEAPON_UZI, WEAPON_TEC9,
						     WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {}

						default: {
							DebugMessage(issuerid, "weapon changed from %d to melee (punch & swap)", weaponid);
							weaponid = WEAPON_UNARMED;
							melee = true;
						}
					}
				}
			}

			case 54.12000274658203125: {
				if (!melee) {
					DebugMessage(issuerid, "weapon changed from %d to melee (punch & swap)", weaponid);
					melee = true;
					weaponid = WEAPON_UNARMED;
					amount = 1.32000005245208740234375;
				}

				// Be extra sure about this one
				if (GetPlayerFightingStyle(issuerid) != FIGHT_STYLE_KNEEHEAD) {
					return WC_INVALID_DAMAGE;
				}
			}

			// Melee damage has been tampered with
			default: {
				if (melee) {
					return WC_INVALID_DAMAGE;
				}
			}
		}
	}

	if (melee) {
		new Float:x, Float:y, Float:z, Float:dist;
		GetPlayerPos(issuerid, x, y, z);
		dist = GetPlayerDistanceFromPoint(playerid, x, y, z);

		if (WEAPON_UNARMED <= weaponid < sizeof(s_WeaponRange) && dist > s_WeaponRange[weaponid] + 2.0) {
			AddRejectedHit(issuerid, playerid, HIT_TOO_FAR_FROM_ORIGIN, weaponid, _:dist, _:s_WeaponRange[weaponid]);
			return WC_INVALID_DISTANCE;
		}
	}

	switch (weaponid) {
		// The spas shotguns shoot 8 bullets, each inflicting 4.95 damage
		case WEAPON_SHOTGSPA: {
			bullets = amount / 4.950000286102294921875;

			if (8.0 - bullets < -0.05) {
				return WC_INVALID_DAMAGE;
			}
		}

		// Shotguns and sawed-off shotguns shoot 15 bullets, each inflicting 3.3 damage
		case WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {
			bullets = amount / 3.30000019073486328125;

			if (15.0 - bullets < -0.05) {
				return WC_INVALID_DAMAGE;
			}
		}
	}

	if (_:bullets) {
		new Float:f = floatfract(bullets);

		// The damage for each bullet has been tampered with
		if (f > 0.01 && f < 0.99) {
			return WC_INVALID_DAMAGE;
		}

		// Divide the damage amount by the number of bullets
		amount /= bullets;
	}

	// Check chainsaw damage
	if (weaponid == WEAPON_CHAINSAW) {
		switch (amount) {
			case 6.6000003814697265625,
			     13.5300006866455078125,
			     16.1700000762939453125,
			     26.40000152587890625,
			     27.060001373291015625: {}

			default: {
				return WC_INVALID_DAMAGE;
			}
		}
	} else if (weaponid == WEAPON_DEAGLE) {
		// Check deagle damage

		switch (amount) {
			case 46.200000762939453125,
			     23.1000003814697265625: {}

			default: {
				return WC_INVALID_DAMAGE;
			}
		}
	}

	// Check gun damage
	new Float:def_amount = 0.0;

	switch (weaponid) {
		case WEAPON_COLT45,
		     WEAPON_MP5: def_amount = 8.25;
		case WEAPON_SILENCED: def_amount = 13.200000762939453125;
		case WEAPON_UZI,
		     WEAPON_TEC9: def_amount = 6.6000003814697265625;
		case WEAPON_AK47,
		     WEAPON_M4,
		     WEAPON_VEHICLE_M4: def_amount = 9.90000057220458984375;
		case WEAPON_RIFLE: def_amount = 24.7500019073486328125;
		case WEAPON_SNIPER: def_amount = 41.25;
		case WEAPON_MINIGUN,
		     WEAPON_VEHICLE_MINIGUN: def_amount = 46.200000762939453125;
		case WEAPON_VEHICLE: def_amount = 9.90000057220458984375;
	}

	if (_:def_amount && _:def_amount != _:amount) {
		return WC_INVALID_DAMAGE;
	}

	// Adjust the damage
	switch (s_DamageType[weaponid]) {
		case DAMAGE_TYPE_MULTIPLIER: {
			if (_:s_WeaponDamage[weaponid] != _:1.0) {
				amount *= s_WeaponDamage[weaponid];
			}
		}

		case DAMAGE_TYPE_STATIC: {
			if (_:bullets) {
				amount = s_WeaponDamage[weaponid] * bullets;
			} else {
				amount = s_WeaponDamage[weaponid];
			}
		}

		case DAMAGE_TYPE_RANGE,
		     DAMAGE_TYPE_RANGE_MULTIPLIER: {
			new Float:length = 0.0;
			if (s_LagCompMode) {
				length = s_LastShot[issuerid][e_Length];
			} else {
				new Float:X, Float:Y, Float:Z;
				GetPlayerPos(issuerid, X, Y, Z);
				length = GetPlayerDistanceFromPoint(playerid, X, Y, Z);
			}

			for (new i = s_DamageRangeSteps[weaponid] - 1; i >= 0; i--) {
				if (i == 0 || length >= s_DamageRangeRanges[weaponid][i]) {
					if (s_DamageType[weaponid] == DAMAGE_TYPE_RANGE_MULTIPLIER) {
						if (_:s_DamageRangeValues[weaponid][i] != _:1.0) {
							amount *= s_DamageRangeValues[weaponid][i];
						}
					} else {
						if (_:bullets) {
							amount = s_DamageRangeValues[weaponid][i] * bullets;
						} else {
							amount = s_DamageRangeValues[weaponid][i];
						}
					}

					break;
				}
			}
		}
	}

	return WC_NO_ERROR;
}

static InflictDamage(playerid, Float:amount, issuerid = INVALID_PLAYER_ID, weaponid = WEAPON_UNKNOWN, bodypart = BODY_PART_UNKNOWN, bool:ignore_armour = false)
{
	if (!WC_IsPlayerSpawned(playerid) || amount < 0.0) {
		return;
	}

	if (!OnPlayerDamage(playerid, amount, issuerid, weaponid, bodypart)) {
		UpdateHealthBar(playerid);

		if (weaponid < WEAPON_UNARMED || weaponid > WEAPON_UNKNOWN) {
			weaponid = WEAPON_UNKNOWN;
		}

		#if WC_DEBUG
			new Float:length = 0.0;

			if (issuerid != INVALID_PLAYER_ID) {
				if (IsBulletWeapon(weaponid)) {
					length = s_LastShot[issuerid][e_Length];
				}
			}

			if (!IsHighRateWeapon(weaponid)) {
				DebugMessageAll("!InflictDamage(%d, %.4f, %d, %d, %d) length = %f", playerid, amount, issuerid, weaponid, bodypart, length);
			}
		#endif

		return;
	}

	if (weaponid < WEAPON_UNARMED || weaponid > WEAPON_UNKNOWN) {
		weaponid = WEAPON_UNKNOWN;
	}

	#if WC_DEBUG
		new Float:length = 0.0;

		if (issuerid != INVALID_PLAYER_ID) {
			if (IsBulletWeapon(weaponid)) {
				length = s_LastShot[issuerid][e_Length];
			}
		}

		if (!IsHighRateWeapon(weaponid)) {
			DebugMessageAll("InflictDamage(%d, %.4f, %d, %d, %d) length = %f", playerid, amount, issuerid, weaponid, bodypart, length);
		}
	#endif

	if (!ignore_armour && weaponid != WEAPON_COLLISION && weaponid != WEAPON_DROWN && weaponid != WEAPON_CARPARK && weaponid != WEAPON_UNKNOWN
	&& (!s_DamageArmourToggle[0] || (s_DamageArmour[weaponid][0] && (!s_DamageArmourToggle[1] || ((s_DamageArmour[weaponid][1] && bodypart == 3) || (!s_DamageArmour[weaponid][1])))))) {
		if (amount <= 0.0) {
			amount = s_PlayerHealth[playerid] + s_PlayerArmour[playerid];
		}

		s_PlayerArmour[playerid] -= amount;
	} else {
		if (amount <= 0.0) {
			amount = s_PlayerHealth[playerid];
		}

		s_PlayerHealth[playerid] -= amount;
	}

	if (s_PlayerArmour[playerid] < 0.0) {
		s_DamageDoneArmour[playerid] = amount + s_PlayerArmour[playerid];
		s_DamageDoneHealth[playerid] = -s_PlayerArmour[playerid];
		s_PlayerHealth[playerid] += s_PlayerArmour[playerid];
		s_PlayerArmour[playerid] = 0.0;
	} else {
		s_DamageDoneArmour[playerid] = amount;
		s_DamageDoneHealth[playerid] = 0.0;
	}

	if (s_PlayerHealth[playerid] <= 0.0) {
		amount += s_PlayerHealth[playerid];
		s_DamageDoneHealth[playerid] += s_PlayerHealth[playerid];
		s_PlayerHealth[playerid] = 0.0;
	}

	OnPlayerDamageDone(playerid, amount, issuerid, weaponid, bodypart);
	new animlib[32] = "PED", animname[32];

	if (s_PlayerHealth[playerid] <= 0.0005) {
		new vehicleid = GetPlayerVehicleID(playerid);

		if (vehicleid) {
			new modelid = GetVehicleModel(vehicleid);
			new seat = GetPlayerVehicleSeat(playerid);

			TogglePlayerControllable(playerid, false);

			switch (modelid) {
				case 509, 481, 510, 462, 448, 581, 522,
				     461, 521, 523, 463, 586, 468, 471: {
					new Float:vx, Float:vy, Float:vz;
					GetVehicleVelocity(vehicleid, vx, vy, vz);

					if (vx * vx + vy * vy + vz * vz >= 0.4) {
						animname = "BIKE_fallR";
						PlayerDeath(playerid, animlib, animname, 0);
					} else {
						animname = "BIKE_fall_off";
						PlayerDeath(playerid, animlib, animname, 0);
					}
				}

				default: {
					if (seat & 1) {
						animname = "CAR_dead_LHS";
						PlayerDeath(playerid, animlib, animname);
					} else {
						animname = "CAR_dead_RHS";
						PlayerDeath(playerid, animlib, animname);
					}
				}
			}
		} else if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK) {
			animname = "KO_skid_back";
			PlayerDeath(playerid, animlib, animname, .freeze_sync = false);
		} else {
			if (gettime() - s_LastVehicleEnterTime[playerid] < 10) {
				TogglePlayerControllable(playerid, false);
			}

			new anim = GetPlayerAnimationIndex(playerid);

			if (anim == 1250 || (1538 <= anim <= 1544) || weaponid == WEAPON_DROWN) {
				// In water
				animname = "Drown";
				PlayerDeath(playerid, animlib, animname);
			} else if (1195 <= anim <= 1198) {
				// Jumping animation
				animname = "KO_skid_back";
				PlayerDeath(playerid, animlib, animname);
			} else if (WEAPON_SHOTGUN <= weaponid <= WEAPON_SHOTGSPA) {
				if (IsPlayerBehindPlayer(issuerid, playerid)) {
					animname = "KO_shot_front";
					MakePlayerFacePlayer(playerid, issuerid, true);
					PlayerDeath(playerid, animlib, animname);
				} else {
					animname = "BIKE_fall_off";
					MakePlayerFacePlayer(playerid, issuerid);
					PlayerDeath(playerid, animlib, animname);
				}
			} else if (WEAPON_RIFLE <= weaponid <= WEAPON_SNIPER) {
				if (bodypart == 9) {
					animname = "KO_shot_face";
					PlayerDeath(playerid, animlib, animname);
				} else if (IsPlayerBehindPlayer(issuerid, playerid)) {
					animname = "KO_shot_front";
					PlayerDeath(playerid, animlib, animname);
				} else {
					animname = "KO_shot_stom";
					PlayerDeath(playerid, animlib, animname);
				}
			} else if (IsBulletWeapon(weaponid)) {
				if (bodypart == 9) {
					animname = "KO_shot_face";
					PlayerDeath(playerid, animlib, animname);
				} else {
					animname = "KO_shot_front";
					PlayerDeath(playerid, animlib, animname);
				}
			} else if (weaponid == WEAPON_PISTOLWHIP) {
				animname = "KO_spin_R";
				PlayerDeath(playerid, animlib, animname);
			} else if (weaponid == WEAPON_CARPARK || IsMeleeWeapon(weaponid) && weaponid != WEAPON_CHAINSAW) {
				animname = "KO_skid_front";
				PlayerDeath(playerid, animlib, animname);
			} else if (weaponid == WEAPON_SPRAYCAN || weaponid == WEAPON_FIREEXTINGUISHER) {
				animlib = "KNIFE", animname = "KILL_Knife_Ped_Die";
				PlayerDeath(playerid, animlib, animname);
			} else {
				animname = "KO_skid_back";
				PlayerDeath(playerid, animlib, animname);
			}
		}

		if (s_CbugAllowed[playerid]) {
			WC_OnPlayerDeath(playerid, issuerid, weaponid);
		} else {
			s_DelayedDeathTimer[playerid] = SetTimerEx(#WC_DelayedDeath, 1200, false, "iii", playerid, issuerid, weaponid);
		}
	}

	UpdateHealthBar(playerid);
}

forward WC_DelayedDeath(playerid, issuerid, reason);
public WC_DelayedDeath(playerid, issuerid, reason) {
	s_DelayedDeathTimer[playerid] = -1;

	WC_OnPlayerDeath(playerid, issuerid, reason);
}

static PlayerDeath(playerid, animlib[32], animname[32], anim_lock = 0, respawn_time = -1, bool:freeze_sync = true, anim_freeze = 1)
{
	s_PlayerHealth[playerid] = 0.0;
	s_PlayerArmour[playerid] = 0.0;
	s_IsDying[playerid] = true;

	s_LastDeathTick[playerid] = GetTickCount();

	new action = GetPlayerSpecialAction(playerid);

	if (action && action != SPECIAL_ACTION_DUCK) {
		if (action == SPECIAL_ACTION_USEJETPACK) {
			ClearAnimations(playerid);
		}

		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

		if (action == SPECIAL_ACTION_USEJETPACK) {
			new Float:vx, Float:vy, Float:vz;
			GetPlayerVelocity(playerid, vx, vy, vz);
			SetPlayerVelocity(playerid, vx, vy, vz);
		}
	}

	OnPlayerPrepareDeath(playerid, animlib, animname, anim_lock, respawn_time);

	UpdateHealthBar(playerid);
	FreezeSyncPacket(playerid, .toggle = freeze_sync);

	if (respawn_time == -1) {
		respawn_time = s_RespawnTime;
	}

	if (animlib[0] && animname[0]) {
		ApplyAnimation(playerid, animlib, animname, 4.1, 0, anim_lock, anim_lock, anim_freeze, 0, 1);
	}

	if (s_DeathTimer[playerid] != -1) {
		KillTimer(s_DeathTimer[playerid]);
	}

	s_DeathTimer[playerid] = SetTimerEx("WC_PlayerDeathRespawn", respawn_time, false, "i", playerid);

	if (s_HealthBarForeground[playerid] != PlayerText:INVALID_TEXT_DRAW) {
		PlayerTextDrawHide(playerid, s_HealthBarForeground[playerid]);
	}

	if (IsPlayerInCheckpoint(playerid)) {
		WC_OnPlayerLeaveCheckpoint(playerid);
	}

	if (IsPlayerInRaceCheckpoint(playerid)) {
		WC_OnPlayerLeaveRaceCheckpoint(playerid);
	}
}

public OnPlayerPrepareDeath(playerid, animlib[32], animname[32], &anim_lock, &respawn_time)
{
	return WC_OnPlayerPrepareDeath(playerid, animlib, animname, anim_lock, respawn_time);
}

public OnRejectedHit(playerid, hit[E_REJECTED_HIT])
{
	#if WC_DEBUG
		new output[256];
		new reason = hit[e_Reason];
		new i1 = hit[e_Info1];
		new i2 = hit[e_Info2];
		new i3 = hit[e_Info3];
		new weapon = hit[e_Weapon];

		new weapon_name[32];

		WC_GetWeaponName(weapon, weapon_name);

		format(output, sizeof(output), "(%s -> %s) %s", weapon_name, hit[e_Name], g_HitRejectReasons[reason]);

		format(output, sizeof(output), output, i1, i2, i3);

		DebugMessageRed(playerid, "Rejected hit: %s", output);
	#endif

	WC_OnRejectedHit(playerid, hit);
}

public OnPlayerDeathFinished(playerid, bool:cancelable)
{
	if (s_PlayerHealth[playerid] == 0.0) {
		s_PlayerHealth[playerid] = s_PlayerMaxHealth[playerid];
	}

	if (s_DeathTimer[playerid] != -1) {
		KillTimer(s_DeathTimer[playerid]);
		s_DeathTimer[playerid] = -1;
	}

	new retval = WC_OnPlayerDeathFinished(playerid, cancelable);

	if (!retval && cancelable) {
		return 0;
	}

	ResetPlayerWeapons(playerid);

	return 1;
}

#if WC_CUSTOM_VENDING_MACHINES
	public OnPlayerUseVendingMachine(playerid, &Float:health_given) {
		return WC_OnPlayerUseVendingMachine(playerid, health_given);
	}

	forward WC_VendingMachineUsed(playerid, Float:health_given);
	public WC_VendingMachineUsed(playerid, Float:health_given)
	{
		s_VendingUseTimer[playerid] = -1;

		if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && !s_IsDying[playerid]) {
			new Float:health = s_PlayerHealth[playerid];

			health += health_given;

			if (health > s_PlayerMaxHealth[playerid]) {
				health = s_PlayerMaxHealth[playerid];
			}

			WC_SetPlayerHealth(playerid, health);
		}
	}
#endif

forward WC_DamageFeedUpdate(playerid);
public WC_DamageFeedUpdate(playerid)
{
	s_DamageFeedTimer[playerid] = -1;

	if (IsPlayerConnected(playerid) && IsDamageFeedActive(playerid)) {
		DamageFeedUpdate(playerid, true);
	}
}

static DamageFeedUpdate(playerid, bool:modified = false)
{
	if (!IsDamageFeedActive(playerid)) {
		if (s_DamageFeedGiven[playerid] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, s_DamageFeedGiven[playerid]);
			s_InternalPlayerTextDraw[playerid][s_DamageFeedGiven[playerid]] = false;
			s_DamageFeedGiven[playerid] = PlayerText:INVALID_TEXT_DRAW;
		}

		if (s_DamageFeedTaken[playerid] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, s_DamageFeedTaken[playerid]);
			s_InternalPlayerTextDraw[playerid][s_DamageFeedTaken[playerid]] = false;
			s_DamageFeedTaken[playerid] = PlayerText:INVALID_TEXT_DRAW;
		}

		return;
	}

	if (s_DamageFeedGiven[playerid] == PlayerText:INVALID_TEXT_DRAW) {
		new PlayerText:td = CreatePlayerTextDraw(playerid, 200.0, 365.0, "_");

		if (td == PlayerText:INVALID_TEXT_DRAW) {
			printf("(wc) WARN: Unable to create damage feed textdraw");
		} else {
			s_InternalPlayerTextDraw[playerid][td] = true;

			PlayerTextDrawLetterSize(playerid, td, 0.2, 0.9);
			PlayerTextDrawColor(playerid, td, 0x30FF50FF);
			PlayerTextDrawFont(playerid, td, 1);
			PlayerTextDrawSetShadow(playerid, td, 0);
			PlayerTextDrawAlignment(playerid, td, 2);
			PlayerTextDrawSetOutline(playerid, td, 1);
			PlayerTextDrawBackgroundColor(playerid, td, 0x0000001A);

			s_DamageFeedGiven[playerid] = td;
		}
	}

	if (s_DamageFeedTaken[playerid] == PlayerText:INVALID_TEXT_DRAW) {
		new PlayerText:td = CreatePlayerTextDraw(playerid, 440.0, 365.0, "_");

		if (td == PlayerText:INVALID_TEXT_DRAW) {
			printf("(wc) WARN: Unable to create damage feed textdraw");
		} else {
			s_InternalPlayerTextDraw[playerid][td] = true;

			PlayerTextDrawLetterSize(playerid, td, 0.2, 0.9);
			PlayerTextDrawColor(playerid, td, 0x33CCFFFF);
			PlayerTextDrawFont(playerid, td, 1);
			PlayerTextDrawSetShadow(playerid, td, 0);
			PlayerTextDrawAlignment(playerid, td, 2);
			PlayerTextDrawSetOutline(playerid, td, 1);
			PlayerTextDrawBackgroundColor(playerid, td, 0x0000001A);

			s_DamageFeedTaken[playerid] = td;
		}
	}

	new tick = GetTickCount();
	if (tick == 0) tick = 1;
	new lowest_tick = tick + 1;

	for (new i = 0, j = 0; i < sizeof(s_DamageFeedHitsGiven[]) - 1; i++) {
		if (!s_DamageFeedHitsGiven[playerid][i][e_Tick]) {
			break;
		}

		if (tick - s_DamageFeedHitsGiven[playerid][i][e_Tick] >= s_DamageFeedHideDelay) {
			modified = true;

			for (j = i; j < sizeof(s_DamageFeedHitsGiven[]) - 1; j++) {
				s_DamageFeedHitsGiven[playerid][j][e_Tick] = 0;
			}

			break;
		}

		if (s_DamageFeedHitsGiven[playerid][i][e_Tick] < lowest_tick) {
			lowest_tick = s_DamageFeedHitsGiven[playerid][i][e_Tick];
		}
	}

	for (new i = 0, j = 0; i < sizeof(s_DamageFeedHitsTaken[]) - 1; i++) {
		if (!s_DamageFeedHitsTaken[playerid][i][e_Tick]) {
			break;
		}

		if (tick - s_DamageFeedHitsTaken[playerid][i][e_Tick] >= s_DamageFeedHideDelay) {
			modified = true;

			for (j = i; j < sizeof(s_DamageFeedHitsTaken[]) - 1; j++) {
				s_DamageFeedHitsTaken[playerid][j][e_Tick] = 0;
			}

			break;
		}

		if (s_DamageFeedHitsTaken[playerid][i][e_Tick] < lowest_tick) {
			lowest_tick = s_DamageFeedHitsTaken[playerid][i][e_Tick];
		}
	}

	if (s_DamageFeedTimer[playerid] != -1) {
		KillTimer(s_DamageFeedTimer[playerid]);
	}

	if (tick - s_DamageFeedLastUpdate[playerid] < s_DamageFeedMaxUpdateRate && modified) {
		s_DamageFeedTimer[playerid] = SetTimerEx("WC_DamageFeedUpdate", s_DamageFeedMaxUpdateRate - (tick - s_DamageFeedLastUpdate[playerid]), false, "i", playerid);
	} else {
		if (lowest_tick == tick + 1) {
			s_DamageFeedTimer[playerid] = -1;
			modified = true;
		} else {
			s_DamageFeedTimer[playerid] = SetTimerEx("WC_DamageFeedUpdate", s_DamageFeedHideDelay - (tick - lowest_tick) + 10, false, "i", playerid);
		}

		if (modified) {
			DamageFeedUpdateText(playerid);

			s_DamageFeedLastUpdate[playerid] = tick;
		}
	}
}

static DamageFeedUpdateText(playerid)
{
	new buf[64 * WC_FEED_HEIGHT] = "";

	for (new i = 0, weapon[32]; i < sizeof(s_DamageFeedHitsGiven[]) - 1; i++) {
		if (!s_DamageFeedHitsGiven[playerid][i][e_Tick]) {
			break;
		}

		if (s_DamageFeedHitsGiven[playerid][i][e_Weapon] == -1) {
			weapon = "Multiple";
		} else {
			WC_GetWeaponName(s_DamageFeedHitsGiven[playerid][i][e_Weapon], weapon);
		}

		if (s_DamageFeedHitsGiven[playerid][i][e_Issuer] == INVALID_PLAYER_ID) {
			format(
				buf,
				sizeof(buf),
				"%s%s +%.2f~n~",
				buf,
				weapon,
				s_DamageFeedHitsGiven[playerid][i][e_Amount] + 0.009
			);
		} else {
			format(
				buf,
				sizeof(buf),
				"%s%s - %s +%.2f (%.2f)~n~",
				buf,
				s_DamageFeedHitsGiven[playerid][i][e_Name],
				weapon,
				s_DamageFeedHitsGiven[playerid][i][e_Amount] + 0.009,
				s_PlayerHealth[s_DamageFeedHitsGiven[playerid][i][e_Issuer]]
			);
		}
	}

	if (s_DamageFeedGiven[playerid] == PlayerText:INVALID_TEXT_DRAW) {
		printf("(wc) WARN: Doesn't have feed textdraw when needed");
	} else if (s_InternalPlayerTextDraw[playerid][s_DamageFeedGiven[playerid]]) {
		if (buf[0]) {
			PlayerTextDrawSetString(playerid, s_DamageFeedGiven[playerid], buf);
			PlayerTextDrawShow(playerid, s_DamageFeedGiven[playerid]);
		} else {
			PlayerTextDrawHide(playerid, s_DamageFeedGiven[playerid]);
		}
	}

	buf = "";

	for (new i = 0, weapon[32]; i < sizeof(s_DamageFeedHitsTaken[]) - 1; i++) {
		if (!s_DamageFeedHitsTaken[playerid][i][e_Tick]) {
			break;
		}

		if (s_DamageFeedHitsTaken[playerid][i][e_Weapon] == -1) {
			weapon = "Multiple";
		} else {
			WC_GetWeaponName(s_DamageFeedHitsTaken[playerid][i][e_Weapon], weapon);
		}

		if (s_DamageFeedHitsTaken[playerid][i][e_Issuer] == INVALID_PLAYER_ID) {
			format(
				buf,
				sizeof(buf),
				"%s%s -%.2f (%.2f)~n~",
				buf,
				weapon,
				s_DamageFeedHitsTaken[playerid][i][e_Amount] + 0.009,
				s_PlayerHealth[playerid]
			);
		} else {
			format(
				buf,
				sizeof(buf),
				"%s%s - %s -%.2f (%.2f)~n~",
				buf,
				s_DamageFeedHitsTaken[playerid][i][e_Name],
				weapon,
				s_DamageFeedHitsTaken[playerid][i][e_Amount] + 0.009,
				s_PlayerHealth[s_DamageFeedHitsGiven[playerid][i][e_Issuer]]
			);
		}
	}

	if (s_DamageFeedTaken[playerid] == PlayerText:INVALID_TEXT_DRAW) {
		printf("(wc) WARN: Doesn't have feed textdraw when needed");
	} else if (s_InternalPlayerTextDraw[playerid][s_DamageFeedTaken[playerid]]) {
		if (buf[0]) {
			PlayerTextDrawSetString(playerid, s_DamageFeedTaken[playerid], buf);
			PlayerTextDrawShow(playerid, s_DamageFeedTaken[playerid]);
		} else {
			PlayerTextDrawHide(playerid, s_DamageFeedTaken[playerid]);
		}
	}
}

static DamageFeedAddHitGiven(playerid, issuerid, Float:amount, weapon)
{
	#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
	foreach (new i : Player) {
	#else
	for (new i = GetPlayerPoolSize(); i != -1; i--) {
	#endif
		if (s_Spectating[i] == playerid && i != playerid) {
			DamageFeedAddHit(s_DamageFeedHitsGiven[i], i, issuerid, amount, weapon);
		}
	}

	DamageFeedAddHit(s_DamageFeedHitsGiven[playerid], playerid, issuerid, amount, weapon);
}

static DamageFeedAddHitTaken(playerid, issuerid, Float:amount, weapon)
{
	#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
	foreach (new i : Player) {
	#else
	for (new i = GetPlayerPoolSize(); i != -1; i--) {
	#endif
		if (s_Spectating[i] == playerid && i != playerid) {
			DamageFeedAddHit(s_DamageFeedHitsTaken[i], i, issuerid, amount, weapon);
		}
	}

	DamageFeedAddHit(s_DamageFeedHitsTaken[playerid], playerid, issuerid, amount, weapon);
}

static DamageFeedAddHit(arr[WC_FEED_HEIGHT][E_DAMAGE_FEED_HIT], playerid, issuerid, Float:amount, weapon)
{
	if (!IsDamageFeedActive(playerid)) {
		return;
	}

	new tick = GetTickCount();
	if (tick == 0) tick = 1;
	new idx = -1;

	for (new i = 0; i < sizeof(arr) - 1; i++) {
		if (!arr[i][e_Tick]) {
			break;
		}

		if (tick - arr[i][e_Tick] >= s_DamageFeedHideDelay) {
			DamageFeedRemoveHit(arr, i);
			break;
		}

		// Remove it and add it on top below
		if (arr[i][e_Issuer] == issuerid) {
			// Multiple weapons
			if (arr[i][e_Weapon] != weapon) {
				//weapon = -1;
			}

			amount += arr[i][e_Amount];
			idx = i;
			break;
		}
	}

	if (idx == -1) {
		idx = 0;

		// Insert it at the top
		for (new i = sizeof(arr) - 1; i >= 1; i--) {
			arr[i] = arr[i - 1];
		}
	}

	arr[idx][e_Tick] = tick;
	arr[idx][e_Amount] = amount;
	arr[idx][e_Issuer] = issuerid;
	arr[idx][e_Weapon] = weapon;

	GetPlayerName(issuerid, arr[idx][e_Name], MAX_PLAYER_NAME);

	DamageFeedUpdate(playerid, true);
}

static DamageFeedRemoveHit(arr[WC_FEED_HEIGHT][E_DAMAGE_FEED_HIT], idx)
{
	for (new i = 0; i < WC_FEED_HEIGHT; i++) {
		if (i >= idx) {
			arr[i][e_Tick] = 0;
		}
	}
}

static SaveSyncData(playerid)
{
	GetPlayerHealth(playerid, s_SyncData[playerid][e_Health]);
	GetPlayerArmour(playerid, s_SyncData[playerid][e_Armour]);

	GetPlayerPos(playerid, s_SyncData[playerid][e_PosX], s_SyncData[playerid][e_PosY], s_SyncData[playerid][e_PosZ]);
	GetPlayerFacingAngle(playerid, s_SyncData[playerid][e_PosA]);

	s_SyncData[playerid][e_Skin] = GetPlayerSkin(playerid);
	s_SyncData[playerid][e_Team] = GetPlayerTeam(playerid);

	s_SyncData[playerid][e_Weapon] = GetPlayerWeapon(playerid);

	for (new i = 0; i < 13; i++) {
		GetPlayerWeaponData(playerid, i, s_SyncData[playerid][e_WeaponId][i], s_SyncData[playerid][e_WeaponAmmo][i]);
	}
}

static MakePlayerFacePlayer(playerid, targetid, opposite = false, forcesync = true)
{
	new Float:x1, Float:y1, Float:z1;
	new Float:x2, Float:y2, Float:z2;

	GetPlayerPos(playerid, x1, y1, z1);
	GetPlayerPos(targetid, x2, y2, z2);
	new Float:angle = AngleBetweenPoints(x2, y2, x1, y1);

	if (opposite) {
		angle += 180.0;
		if (angle > 360.0) angle -= 360.0;
	}

	if (angle < 0.0) angle += 360.0;
	if (angle > 360.0) angle -= 360.0;

	SetPlayerFacingAngle(playerid, angle);

	if (forcesync) {
		SetFakeFacingAngle(playerid, angle);
		UpdateSyncData(playerid);
	}
}

static IsPlayerBehindPlayer(playerid, targetid, Float:diff = 90.0)
{
	new Float:x1, Float:y1, Float:z1;
	new Float:x2, Float:y2, Float:z2;
	new Float:ang, Float:angdiff;

	GetPlayerPos(playerid, x1, y1, z1);
	GetPlayerPos(targetid, x2, y2, z2);
	GetPlayerFacingAngle(targetid, ang);

	angdiff = AngleBetweenPoints(x1, y1, x2, y2);

	if (angdiff < 0.0) angdiff += 360.0;
	if (angdiff > 360.0) angdiff -= 360.0;

	ang = ang - angdiff;

	if (ang > 180.0) ang -= 360.0;
	if (ang < -180.0) ang += 360.0;

	return floatabs(ang) > diff;
}

static AddRejectedHit(playerid, damagedid, reason, weapon, i1 = 0, i2 = 0, i3 = 0)
{
	if (0 <= playerid < MAX_PLAYERS) {
		new idx = s_RejectedHitsIdx[playerid];

		if (s_RejectedHits[playerid][idx][e_Time]) {
			idx += 1;

			if (idx >= sizeof(s_RejectedHits[])) {
				idx = 0;
			}

			s_RejectedHitsIdx[playerid] = idx;
		}

		new time, hour, minute, second;

		time = gettime(hour, minute, second);

		s_RejectedHits[playerid][idx][e_Reason] = reason;
		s_RejectedHits[playerid][idx][e_Time] = time;
		s_RejectedHits[playerid][idx][e_Weapon] = weapon;
		s_RejectedHits[playerid][idx][e_Hour] = hour;
		s_RejectedHits[playerid][idx][e_Minute] = minute;
		s_RejectedHits[playerid][idx][e_Second] = second;
		s_RejectedHits[playerid][idx][e_Info1] = _:i1;
		s_RejectedHits[playerid][idx][e_Info2] = _:i2;
		s_RejectedHits[playerid][idx][e_Info3] = _:i3;

		if (0 <= damagedid < MAX_PLAYERS) {
			GetPlayerName(damagedid, s_RejectedHits[playerid][idx][e_Name], MAX_PLAYER_NAME);
		} else {
			s_RejectedHits[playerid][idx][e_Name][0] = '#';
			s_RejectedHits[playerid][idx][e_Name][1] = '\0';
		}

		OnRejectedHit(playerid, s_RejectedHits[playerid][idx]);
	}
}

forward WC_SecondKnifeAnim(playerid);
public WC_SecondKnifeAnim(playerid)
{
	new animlib[] = "KNIFE", animname[] = "KILL_Knife_Ped_Die";
	ApplyAnimation(playerid, animlib, animname, 4.1, 0, 1, 1, 1, 3000, 1);
}

forward WC_PlayerDeathRespawn(playerid);
public WC_PlayerDeathRespawn(playerid)
{
	if (!s_IsDying[playerid]) {
		return;
	}

	s_IsDying[playerid] = false;

	if (!OnPlayerDeathFinished(playerid, true)) {
		UpdateHealthBar(playerid);
		SetFakeFacingAngle(playerid, _);
		FreezeSyncPacket(playerid, .toggle = false);

		return;
	}

	s_IsDying[playerid] = true;
	s_TrueDeath[playerid] = false;

	if (IsPlayerInAnyVehicle(playerid)) {
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		SetPlayerPos(playerid, x, y, z);
	}

	SetPlayerVirtualWorld(playerid, WC_DEATH_WORLD);
	SetFakeFacingAngle(playerid, _);
	TogglePlayerSpectating(playerid, true);
	TogglePlayerSpectating(playerid, false);
}

public OnInvalidWeaponDamage(playerid, damagedid, Float:amount, weaponid, bodypart, error, bool:given)
{
	DebugMessageRedAll("OnInvalidWeaponDamage(%d, %d, %f, %d, %d, %d, %d)", playerid, damagedid, amount, weaponid, bodypart, error, given);

	WC_OnInvalidWeaponDamage(playerid, damagedid, Float:amount, weaponid, bodypart, error, bool:given);
}

public OnPlayerDamageDone(playerid, Float:amount, issuerid, weapon, bodypart)
{
	new idx = s_PreviousHitI[playerid];

	s_PreviousHitI[playerid] = (s_PreviousHitI[playerid] - 1) % sizeof(s_PreviousHits[]);

	// JIT plugin fix
	if (s_PreviousHitI[playerid] < 0) {
		s_PreviousHitI[playerid] += sizeof(s_PreviousHits[]);
	}

	s_PreviousHits[playerid][idx][e_Tick] = GetTickCount();
	s_PreviousHits[playerid][idx][e_Issuer] = issuerid;
	s_PreviousHits[playerid][idx][e_Weapon] = weapon;
	s_PreviousHits[playerid][idx][e_Amount] = amount;
	s_PreviousHits[playerid][idx][e_Bodypart] = bodypart;
	s_PreviousHits[playerid][idx][e_Health] = GetLastDamageHealth(playerid);
	s_PreviousHits[playerid][idx][e_Armour] = GetLastDamageArmour(playerid);

	if (!IsHighRateWeapon(weapon)) {
		DebugMessageAll("OnPlayerDamageDone(%d did %f to %d with %d on bodypart %d)", issuerid, amount, playerid, weapon, bodypart);

		if (s_DamageTakenSound[playerid]) {
			PlayerPlaySound(playerid, s_DamageTakenSound[playerid], 0.0, 0.0, 0.0);

			#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
			foreach (new i : Player) {
			#else
			for (new i = GetPlayerPoolSize(); i != -1; i--) {
			#endif
				if (s_Spectating[i] == playerid && i != playerid) {
					PlayerPlaySound(i, s_DamageTakenSound[playerid], 0.0, 0.0, 0.0);
				}
			}
		}

		if (issuerid != INVALID_PLAYER_ID && s_DamageGivenSound[issuerid]) {
			PlayerPlaySound(issuerid, s_DamageGivenSound[issuerid], 0.0, 0.0, 0.0);

			#if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
			foreach (new i : Player) {
			#else
			for (new i = GetPlayerPoolSize(); i != -1; i--) {
			#endif
				if (s_Spectating[i] == issuerid && i != issuerid) {
					PlayerPlaySound(i, s_DamageGivenSound[issuerid], 0.0, 0.0, 0.0);
				}
			}
		}
	}

	if (issuerid != INVALID_PLAYER_ID) {
		DamageFeedAddHitGiven(issuerid, playerid, amount, weapon);
	}

	DamageFeedAddHitTaken(playerid, issuerid, amount, weapon);

	WC_OnPlayerDamageDone(playerid, amount, issuerid, weapon, bodypart);
}

public OnPlayerDamage(&playerid, &Float:amount, &issuerid, &weapon, &bodypart)
{
	return WC_OnPlayerDamage(playerid, amount, issuerid, weapon, bodypart);
}

/*
 * ALS callbacks
 */
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit(%0) CHAIN_PUBLIC:WC_OnGameModeInit(%0)
CHAIN_FORWARD:WC_OnGameModeInit() = 1;


#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit(%0) CHAIN_PUBLIC:WC_OnGameModeExit(%0)
CHAIN_FORWARD:WC_OnGameModeExit() = 1;


#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit(%0) CHAIN_PUBLIC:WC_OnFilterScriptInit(%0)
CHAIN_FORWARD:WC_OnFilterScriptInit() = 1;


#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit(%0) CHAIN_PUBLIC:WC_OnFilterScriptExit(%0)
CHAIN_FORWARD:WC_OnFilterScriptExit() = 1;


#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect(%0) CHAIN_PUBLIC:WC_OnPlayerConnect(%0)
CHAIN_FORWARD:WC_OnPlayerConnect(playerid) = 1;


#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect(%0) CHAIN_PUBLIC:WC_OnPlayerDisconnect(%0)
CHAIN_FORWARD:WC_OnPlayerDisconnect(playerid, reason) = 1;


#if defined _ALS_OnPlayerStreamIn
	#undef OnPlayerStreamIn
#else
	#define _ALS_OnPlayerStreamIn
#endif
#define OnPlayerStreamIn(%0) CHAIN_PUBLIC:WC_OnPlayerStreamIn(%0)
CHAIN_FORWARD:WC_OnPlayerStreamIn(playerid, forplayerid) = 1;


#if defined _ALS_OnVehicleDeath
	#undef OnVehicleDeath
#else
	#define _ALS_OnVehicleDeath
#endif
#define OnVehicleDeath(%0) CHAIN_PUBLIC:WC_OnVehicleDeath(%0)
CHAIN_FORWARD:WC_OnVehicleDeath(vehicleid, killerid) = 1;


#if defined _ALS_OnVehicleSpawn
	#undef OnVehicleSpawn
#else
	#define _ALS_OnVehicleSpawn
#endif
#define OnVehicleSpawn(%0) CHAIN_PUBLIC:WC_OnVehicleSpawn(%0)
CHAIN_FORWARD:WC_OnVehicleSpawn(vehicleid) = 1;


#if defined _ALS_OnPlayerEnterVehicle
	#undef OnPlayerEnterVehicle
#else
	#define _ALS_OnPlayerEnterVehicle
#endif
#define OnPlayerEnterVehicle(%0) CHAIN_PUBLIC:WC_OnPlayerEnterVehicle(%0)
CHAIN_FORWARD:WC_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) = 1;


#if defined _ALS_OnPlayerExitVehicle
	#undef OnPlayerExitVehicle
#else
	#define _ALS_OnPlayerExitVehicle
#endif
#define OnPlayerExitVehicle(%0) CHAIN_PUBLIC:WC_OnPlayerExitVehicle(%0)
CHAIN_FORWARD:WC_OnPlayerExitVehicle(playerid, vehicleid) = 1;


#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange(%0) CHAIN_PUBLIC:WC_OnPlayerStateChange(%0)
CHAIN_FORWARD:WC_OnPlayerStateChange(playerid, newstate, oldstate) = 1;


#if defined _ALS_OnPlayerPickUpPickup
	#undef OnPlayerPickUpPickup
#else
	#define _ALS_OnPlayerPickUpPickup
#endif
#define OnPlayerPickUpPickup(%0) CHAIN_PUBLIC:WC_OnPlayerPickUpPickup(%0)
CHAIN_FORWARD:WC_OnPlayerPickUpPickup(playerid, pickupid) = 1;


#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate(%0) CHAIN_PUBLIC:WC_OnPlayerUpdate(%0)
CHAIN_FORWARD:WC_OnPlayerUpdate(playerid) = 1;


#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn(%0) CHAIN_PUBLIC:WC_OnPlayerSpawn(%0)
CHAIN_FORWARD:WC_OnPlayerSpawn(playerid) = 1;


#if defined _ALS_OnPlayerRequestClass
	#undef OnPlayerRequestClass
#else
	#define _ALS_OnPlayerRequestClass
#endif
#define OnPlayerRequestClass(%0) CHAIN_PUBLIC:WC_OnPlayerRequestClass(%0)
CHAIN_FORWARD:WC_OnPlayerRequestClass(playerid, classid) = 1;


#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath(%0) CHAIN_PUBLIC:WC_OnPlayerDeath(%0)
CHAIN_FORWARD:WC_OnPlayerDeath(playerid, killerid, reason) = 1;


#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange(%0) CHAIN_PUBLIC:WC_OnPlayerKeyStateChange(%0)
CHAIN_FORWARD:WC_OnPlayerKeyStateChange(playerid, newkeys, oldkeys) = 1;


#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot(%0) CHAIN_PUBLIC:WC_OnPlayerWeaponShot(%0)
CHAIN_FORWARD:WC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) = 1;


#if defined _ALS_OnPlayerEnterCheckpoint
	#undef OnPlayerEnterCheckpoint
#else
	#define _ALS_OnPlayerEnterCheckpoint
#endif
#define OnPlayerEnterCheckpoint(%0) CHAIN_PUBLIC:WC_OnPlayerEnterCheckpoint(%0)
CHAIN_FORWARD:WC_OnPlayerEnterCheckpoint(playerid) = 1;


#if defined _ALS_OnPlayerLeaveCheckpoint
	#undef OnPlayerLeaveCheckpoint
#else
	#define _ALS_OnPlayerLeaveCheckpoint
#endif
#define OnPlayerLeaveCheckpoint(%0) CHAIN_PUBLIC:WC_OnPlayerLeaveCheckpoint(%0)
CHAIN_FORWARD:WC_OnPlayerLeaveCheckpoint(playerid) = 1;


#if defined _ALS_OnPlayerEnterRaceCP
	#undef OnPlayerEnterRaceCheckpoint
#else
	#define _ALS_OnPlayerEnterRaceCP
#endif
#define OnPlayerEnterRaceCheckpoint(%0) CHAIN_PUBLIC:WC_OnPlayerEnterRaceCheckpoint(%0)
CHAIN_FORWARD:WC_OnPlayerEnterRaceCheckpoint(playerid) = 1;


#if defined _ALS_OnPlayerLeaveRaceCP
	#undef OnPlayerLeaveRaceCheckpoint
#else
	#define _ALS_OnPlayerLeaveRaceCP
#endif
#define OnPlayerLeaveRaceCheckpoint(%0) CHAIN_PUBLIC:WC_OnPlayerLeaveRaceCheckpoint(%0)
CHAIN_FORWARD:WC_OnPlayerLeaveRaceCheckpoint(playerid) = 1;


#if defined _ALS_OnInvalidWeaponDamage
	#undef OnInvalidWeaponDamage
#else
	#define _ALS_OnInvalidWeaponDamage
#endif
#define OnInvalidWeaponDamage(%0) CHAIN_PUBLIC:WC_OnInvalidWeaponDamage(%0)
CHAIN_FORWARD:WC_OnInvalidWeaponDamage(playerid, damagedid, Float:amount, weaponid, bodypart, error, bool:given) = 1;


#if defined _ALS_OnPlayerDamageDone
	#undef OnPlayerDamageDone
#else
	#define _ALS_OnPlayerDamageDone
#endif
#define OnPlayerDamageDone(%0) CHAIN_PUBLIC:WC_OnPlayerDamageDone(%0)
CHAIN_FORWARD:WC_OnPlayerDamageDone(playerid, Float:amount, issuerid, weapon, bodypart) = 1;


#if defined _ALS_OnPlayerDamage
	#undef OnPlayerDamage
#else
	#define _ALS_OnPlayerDamage
#endif
#define OnPlayerDamage(%0) CHAIN_PUBLIC:WC_OnPlayerDamage(%0)
CHAIN_FORWARD:WC_OnPlayerDamage(&playerid, &Float:amount, &issuerid, &weapon, &bodypart) = 1;


#if defined _ALS_OnPlayerPrepareDeath
	#undef OnPlayerPrepareDeath
#else
	#define _ALS_OnPlayerPrepareDeath
#endif
#define OnPlayerPrepareDeath(%0) CHAIN_PUBLIC:WC_OnPlayerPrepareDeath(%0)
CHAIN_FORWARD:WC_OnPlayerPrepareDeath(playerid, animlib[32], animname[32], &anim_lock, &respawn_time) = 1;


#if defined _ALS_OnRejectedHit
	#undef OnRejectedHit
#else
	#define _ALS_OnRejectedHit
#endif
#define OnRejectedHit(%0) CHAIN_PUBLIC:WC_OnRejectedHit(%0)
CHAIN_FORWARD:WC_OnRejectedHit(playerid, hit[E_REJECTED_HIT]) = 1;


#if WC_CUSTOM_VENDING_MACHINES
	#if defined _ALS_OnPlayerUseVendingMachine
		#undef OnPlayerUseVendingMachine
	#else
		#define _ALS_OnPlayerUseVendingMachine
	#endif
	#define OnPlayerUseVendingMachine(%0) CHAIN_PUBLIC:WC_OnPlayerUseVendingMachine(%0)
	CHAIN_FORWARD:WC_OnPlayerUseVendingMachine(playerid, &Float:health_given) = 1;
#endif


#if defined _ALS_OnPlayerDeathFinished
	#undef OnPlayerDeathFinished
#else
	#define _ALS_OnPlayerDeathFinished
#endif
#define OnPlayerDeathFinished(%0) CHAIN_PUBLIC:WC_OnPlayerDeathFinished(%0)
CHAIN_FORWARD:WC_OnPlayerDeathFinished(playerid, bool:cancelable) = 1;

/*
 * ALS functions
 */
#if defined _ALS_SpawnPlayer
	#undef SpawnPlayer
#else
	#define _ALS_SpawnPlayer
#endif
#define SpawnPlayer WC_SpawnPlayer


#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
	#define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth WC_SetPlayerHealth


#if defined _ALS_GetPlayerState
	#undef GetPlayerState
#else
	#define _ALS_GetPlayerState
#endif
#define GetPlayerState WC_GetPlayerState


#if defined _ALS_GetPlayerHealth
	#undef GetPlayerHealth
#else
	#define _ALS_GetPlayerHealth
#endif
#define GetPlayerHealth WC_GetPlayerHealth


#if defined _ALS_SetPlayerArmour
	#undef SetPlayerArmour
#else
	#define _ALS_SetPlayerArmour
#endif
#define SetPlayerArmour WC_SetPlayerArmour


#if defined _ALS_GetPlayerArmour
	#undef GetPlayerArmour
#else
	#define _ALS_GetPlayerArmour
#endif
#define GetPlayerArmour WC_GetPlayerArmour


#if defined _ALS_GetPlayerTeam
	#undef GetPlayerTeam
#else
	#define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam WC_GetPlayerTeam


#if defined _ALS_SetPlayerTeam
	#undef SetPlayerTeam
#else
	#define _ALS_SetPlayerTeam
#endif
#define SetPlayerTeam WC_SetPlayerTeam


#if defined _ALS_SendDeathMessage
	#undef SendDeathMessage
#else
	#define _ALS_SendDeathMessage
#endif
#define SendDeathMessage WC_SendDeathMessage


#if defined _ALS_GetWeaponName
	#undef GetWeaponName
#else
	#define _ALS_GetWeaponName
#endif
#define GetWeaponName WC_GetWeaponName


#if defined _ALS_ApplyAnimation
	#undef ApplyAnimation
#else
	#define _ALS_ApplyAnimation
#endif
#define ApplyAnimation WC_ApplyAnimation


#if defined _ALS_ClearAnimations
	#undef ClearAnimations
#else
	#define _ALS_ClearAnimations
#endif
#define ClearAnimations WC_ClearAnimations


#if defined _ALS_AddPlayerClass
	#undef AddPlayerClass
#else
	#define _ALS_AddPlayerClass
#endif
#define AddPlayerClass WC_AddPlayerClass


#if defined _ALS_AddPlayerClassEx
	#undef AddPlayerClassEx
#else
	#define _ALS_AddPlayerClassEx
#endif
#define AddPlayerClassEx WC_AddPlayerClassEx


#if defined _ALS_SetSpawnInfo
	#undef SetSpawnInfo
#else
	#define _ALS_SetSpawnInfo
#endif
#define SetSpawnInfo WC_SetSpawnInfo


#if defined _ALS_TogglePlayerSpectating
	#undef TogglePlayerSpectating
#else
	#define _ALS_TogglePlayerSpectating
#endif
#define TogglePlayerSpectating WC_TogglePlayerSpectating


#if defined _ALS_TogglePlayerControllable
	#undef TogglePlayerControllable
#else
	#define _ALS_TogglePlayerControllable
#endif
#define TogglePlayerControllable WC_TogglePlayerControllable


#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else
	#define _ALS_SetPlayerPos
#endif
#define SetPlayerPos WC_SetPlayerPos


#if defined _ALS_SetPlayerPosFindZ
	#undef SetPlayerPosFindZ
#else
	#define _ALS_SetPlayerPosFindZ
#endif
#define SetPlayerPosFindZ WC_SetPlayerPosFindZ


#if defined _ALS_SetPlayerVelocity
	#undef SetPlayerVelocity
#else
	#define _ALS_SetPlayerVelocity
#endif
#define SetPlayerVelocity WC_SetPlayerVelocity


#if defined _ALS_SetPlayerVirtualWorld
	#undef SetPlayerVirtualWorld
#else
	#define _ALS_SetPlayerVirtualWorld
#endif
#define SetPlayerVirtualWorld WC_SetPlayerVirtualWorld


#if defined _ALS_GetPlayerVirtualWorld
	#undef GetPlayerVirtualWorld
#else
	#define _ALS_GetPlayerVirtualWorld
#endif
#define GetPlayerVirtualWorld WC_GetPlayerVirtualWorld


#if defined _ALS_PlayerSpectatePlayer
	#undef PlayerSpectatePlayer
#else
	#define _ALS_PlayerSpectatePlayer
#endif
#define PlayerSpectatePlayer WC_PlayerSpectatePlayer


#if defined _ALS_DestroyVehicle
	#undef DestroyVehicle
#else
	#define _ALS_DestroyVehicle
#endif
#define DestroyVehicle WC_DestroyVehicle


#if defined _ALS_CreateVehicle
	#undef CreateVehicle
#else
	#define _ALS_CreateVehicle
#endif
#define CreateVehicle WC_CreateVehicle


#if defined _ALS_AddStaticVehicle
	#undef AddStaticVehicle
#else
	#define _ALS_AddStaticVehicle
#endif
#define AddStaticVehicle WC_AddStaticVehicle


#if defined _ALS_AddStaticVehicleEx
	#undef AddStaticVehicleEx
#else
	#define _ALS_AddStaticVehicleEx
#endif
#define AddStaticVehicleEx WC_AddStaticVehicleEx


#if defined _ALS_IsPlayerInCheckpoint
	#undef IsPlayerInCheckpoint
#else
	#define _ALS_IsPlayerInCheckpoint
#endif
#define IsPlayerInCheckpoint WC_IsPlayerInCheckpoint


#if defined _ALS_IsPlayerInRaceCheckpoint
	#undef IsPlayerInRaceCheckpoint
#else
	#define _ALS_IsPlayerInRaceCheckpoint
#endif
#define IsPlayerInRaceCheckpoint WC_IsPlayerInRaceCheckpoint


#if defined _ALS_SetPlayerSpecialAction
	#undef SetPlayerSpecialAction
#else
	#define _ALS_SetPlayerSpecialAction
#endif
#define SetPlayerSpecialAction WC_SetPlayerSpecialAction


#if defined _ALS_TextDrawCreate
	#undef TextDrawCreate
#else
	#define _ALS_TextDrawCreate
#endif
#define TextDrawCreate WC_TextDrawCreate


#if defined _ALS_TextDrawDestroy
	#undef TextDrawDestroy
#else
	#define _ALS_TextDrawDestroy
#endif
#define TextDrawDestroy WC_TextDrawDestroy


#if defined _ALS_TextDrawLetterSize
	#undef TextDrawLetterSize
#else
	#define _ALS_TextDrawLetterSize
#endif
#define TextDrawLetterSize WC_TextDrawLetterSize


#if defined _ALS_TextDrawTextSize
	#undef TextDrawTextSize
#else
	#define _ALS_TextDrawTextSize
#endif
#define TextDrawTextSize WC_TextDrawTextSize


#if defined _ALS_TextDrawAlignment
	#undef TextDrawAlignment
#else
	#define _ALS_TextDrawAlignment
#endif
#define TextDrawAlignment WC_TextDrawAlignment


#if defined _ALS_TextDrawColor
	#undef TextDrawColor
#else
	#define _ALS_TextDrawColor
#endif
#define TextDrawColor WC_TextDrawColor


#if defined _ALS_TextDrawUseBox
	#undef TextDrawUseBox
#else
	#define _ALS_TextDrawUseBox
#endif
#define TextDrawUseBox WC_TextDrawUseBox


#if defined _ALS_TextDrawBoxColor
	#undef TextDrawBoxColor
#else
	#define _ALS_TextDrawBoxColor
#endif
#define TextDrawBoxColor WC_TextDrawBoxColor


#if defined _ALS_TextDrawSetShadow
	#undef TextDrawSetShadow
#else
	#define _ALS_TextDrawSetShadow
#endif
#define TextDrawSetShadow WC_TextDrawSetShadow


#if defined _ALS_TextDrawSetOutline
	#undef TextDrawSetOutline
#else
	#define _ALS_TextDrawSetOutline
#endif
#define TextDrawSetOutline WC_TextDrawSetOutline


#if defined _ALS_TextDrawBackgroundColor
	#undef TextDrawBackgroundColor
#else
	#define _ALS_TextDrawBackgroundColor
#endif
#define TextDrawBackgroundColor WC_TextDrawBackgroundColor


#if defined _ALS_TextDrawFont
	#undef TextDrawFont
#else
	#define _ALS_TextDrawFont
#endif
#define TextDrawFont WC_TextDrawFont


#if defined _ALS_TextDrawSetProportional
	#undef TextDrawSetProportional
#else
	#define _ALS_TextDrawSetProportional
#endif
#define TextDrawSetProportional WC_TextDrawSetProportional


#if defined _ALS_TextDrawSetSelectable
	#undef TextDrawSetSelectable
#else
	#define _ALS_TextDrawSetSelectable
#endif
#define TextDrawSetSelectable WC_TextDrawSetSelectable


#if defined _ALS_TextDrawShowForPlayer
	#undef TextDrawShowForPlayer
#else
	#define _ALS_TextDrawShowForPlayer
#endif
#define TextDrawShowForPlayer WC_TextDrawShowForPlayer


#if defined _ALS_TextDrawHideForPlayer
	#undef TextDrawHideForPlayer
#else
	#define _ALS_TextDrawHideForPlayer
#endif
#define TextDrawHideForPlayer WC_TextDrawHideForPlayer


#if defined _ALS_TextDrawShowForAll
	#undef TextDrawShowForAll
#else
	#define _ALS_TextDrawShowForAll
#endif
#define TextDrawShowForAll WC_TextDrawShowForAll


#if defined _ALS_TextDrawHideForAll
	#undef TextDrawHideForAll
#else
	#define _ALS_TextDrawHideForAll
#endif
#define TextDrawHideForAll WC_TextDrawHideForAll


#if defined _ALS_TextDrawSetString
	#undef TextDrawSetString
#else
	#define _ALS_TextDrawSetString
#endif
#define TextDrawSetString WC_TextDrawSetString


#if defined _ALS_TextDrawSetPreviewModel
	#undef TextDrawSetPreviewModel
#else
	#define _ALS_TextDrawSetPreviewModel
#endif
#define TextDrawSetPreviewModel WC_TextDrawSetPreviewModel


#if defined _ALS_TextDrawSetPreviewRot
	#undef TextDrawSetPreviewRot
#else
	#define _ALS_TextDrawSetPreviewRot
#endif
#define TextDrawSetPreviewRot WC_TextDrawSetPreviewRot


#if defined _ALS_TextDrawSetPreviewVehCol
	#undef TextDrawSetPreviewVehCol
#else
	#define _ALS_TextDrawSetPreviewVehCol
#endif
#define TextDrawSetPreviewVehCol WC_TextDrawSetPreviewVehCol


#if defined _ALS_CreatePlayerTextDraw
	#undef CreatePlayerTextDraw
#else
	#define _ALS_CreatePlayerTextDraw
#endif
#define CreatePlayerTextDraw WC_CreatePlayerTextDraw


#if defined _ALS_PlayerTextDrawDestroy
	#undef PlayerTextDrawDestroy
#else
	#define _ALS_PlayerTextDrawDestroy
#endif
#define PlayerTextDrawDestroy WC_PlayerTextDrawDestroy


#if defined _ALS_PlayerTextDrawLetterSize
	#undef PlayerTextDrawLetterSize
#else
	#define _ALS_PlayerTextDrawLetterSize
#endif
#define PlayerTextDrawLetterSize WC_PlayerTextDrawLetterSize


#if defined _ALS_PlayerTextDrawTextSize
	#undef PlayerTextDrawTextSize
#else
	#define _ALS_PlayerTextDrawTextSize
#endif
#define PlayerTextDrawTextSize WC_PlayerTextDrawTextSize


#if defined _ALS_PlayerTextDrawAlignment
	#undef PlayerTextDrawAlignment
#else
	#define _ALS_PlayerTextDrawAlignment
#endif
#define PlayerTextDrawAlignment WC_PlayerTextDrawAlignment


#if defined _ALS_PlayerTextDrawColor
	#undef PlayerTextDrawColor
#else
	#define _ALS_PlayerTextDrawColor
#endif
#define PlayerTextDrawColor WC_PlayerTextDrawColor


#if defined _ALS_PlayerTextDrawUseBox
	#undef PlayerTextDrawUseBox
#else
	#define _ALS_PlayerTextDrawUseBox
#endif
#define PlayerTextDrawUseBox WC_PlayerTextDrawUseBox


#if defined _ALS_PlayerTextDrawBoxColor
	#undef PlayerTextDrawBoxColor
#else
	#define _ALS_PlayerTextDrawBoxColor
#endif
#define PlayerTextDrawBoxColor WC_PlayerTextDrawBoxColor


#if defined _ALS_PlayerTextDrawSetShadow
	#undef PlayerTextDrawSetShadow
#else
	#define _ALS_PlayerTextDrawSetShadow
#endif
#define PlayerTextDrawSetShadow WC_PlayerTextDrawSetShadow


#if defined _ALS_PlayerTextDrawSetOutline
	#undef PlayerTextDrawSetOutline
#else
	#define _ALS_PlayerTextDrawSetOutline
#endif
#define PlayerTextDrawSetOutline WC_PlayerTextDrawSetOutline


#if defined _ALS_PlayerTextDrawBackgroundCo
	#undef PlayerTextDrawBackgroundColor
#else
	#define _ALS_PlayerTextDrawBackgroundCo
#endif
#define PlayerTextDrawBackgroundColor WC_PlayerTextDrawBackgroundColo


#if defined _ALS_PlayerTextDrawFont
	#undef PlayerTextDrawFont
#else
	#define _ALS_PlayerTextDrawFont
#endif
#define PlayerTextDrawFont WC_PlayerTextDrawFont


#if defined _ALS_PlayerTextDrawSetProportio
	#undef PlayerTextDrawSetProportional
#else
	#define _ALS_PlayerTextDrawSetProportio
#endif
#define PlayerTextDrawSetProportional WC_PlayerTextDrawSetProportiona


#if defined _ALS_PlayerTextDrawSetSelectabl
	#undef PlayerTextDrawSetSelectable
#else
	#define _ALS_PlayerTextDrawSetSelectabl
#endif
#define PlayerTextDrawSetSelectable WC_PlayerTextDrawSetSelectable


#if defined _ALS_PlayerTextDrawShow
	#undef PlayerTextDrawShow
#else
	#define _ALS_PlayerTextDrawShow
#endif
#define PlayerTextDrawShow WC_PlayerTextDrawShow


#if defined _ALS_PlayerTextDrawHide
	#undef PlayerTextDrawHide
#else
	#define _ALS_PlayerTextDrawHide
#endif
#define PlayerTextDrawHide WC_PlayerTextDrawHide


#if defined _ALS_PlayerTextDrawSetString
	#undef PlayerTextDrawSetString
#else
	#define _ALS_PlayerTextDrawSetString
#endif
#define PlayerTextDrawSetString WC_PlayerTextDrawSetString


#if defined _ALS_PlayerTextDrawSetPreviewMo
	#undef PlayerTextDrawSetPreviewModel
#else
	#define _ALS_PlayerTextDrawSetPreviewMo
#endif
#define PlayerTextDrawSetPreviewModel WC_PlayerTextDrawSetPreviewMode


#if defined _ALS_PlayerTextDrawSetPreviewRo
	#undef PlayerTextDrawSetPreviewRot
#else
	#define _ALS_PlayerTextDrawSetPreviewRo
#endif
#define PlayerTextDrawSetPreviewRot WC_PlayerTextDrawSetPreviewRot


#if defined _ALS_PlayerTextDrawSetPreviewVe
	#undef PlayerTextDrawSetPreviewVehCol
#else
	#define _ALS_PlayerTextDrawSetPreviewVe
#endif
#define PlayerTextDrawSetPreviewVehCol WC_PlayerTextDrawSetPreviewVehC
