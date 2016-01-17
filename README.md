# `weapon-config.inc`

This is an include that provides a more consistent and responsive damage system with many new features.

It's pretty much plug-and-play if you don't have any filterscripts that interfere with the health or death events.

# How to use

1. `#include <weapon-config>` before any other script
2. Replace `OnPlayerGiveDamage` and `OnPlayerTakeDamage` with just one callback:
    
    ```pawn
    public OnPlayerDamage(&playerid, &Float:amount, &issuerid, &weapon, &bodypart)
    ```
3. Add config functions in `OnGameModeInit` (or any other places, they can be called at any time).
    **Recommended**:
    
    ```pawn
    public OnGameModeInit() {
        SetVehiclePassengerDamage(true);
        SetDisableSyncBugs(true);
    }
    ```

## Requirements

This include file requires the [SKY](https://github.com/oscar-broman/SKY/) plugin.

# Features

- **Server-side health**
    - Impossible to use health hacks on lagcomp mode
    - Any type of damage can be modified/prevented, even falling 1000m from the sky
    - Vending machines are controlled server-side (buildings removed and objects created)
    - Paused players can be killed (with death animations) and their HP bars always show the correct values
- **Destroy vehicles with a passenger but no driver**
- **Custom falling damage (optional)**
    - Adjust the damage and at which speed a player will die
- **Sounds and on-screen TextDraw indicators of damage given/taken**
    - Also shows another player's damage feed when spectating
- **New weapon types detected**:
    - `WEAPON_PISTOLWHIP` - When you punch someone with a gun
    - `WEAPON_VEHICLE_M4` - Vehicles with M4 guns (e.g. Rustler)
    - `WEAPON_VEHICLE_MINIGUN` - Vehicles with miniguns (e.g. Hunter)
    - `WEAPON_HELIBLADES` - Helikill
    - `WEAPON_CARPARK` - When you park your car on someone
- **Extensive sanity checking on shots:**
    - Modified weapon.dat is automatically detected
    - Shot vector, player distance, and much more is examined
    - A callback is invoked for each so-called "rejected hit" so that the player is informed. A few of these are:
        - Inflicting damage when already dead (due to lag)
        - Hit a player too far from the shot hit position (due to severe lag or cheating)
        - Hitting/shooting too fast (due to severe lag or cheating)
- **Modify every weapon's damage amount**
    - To a single value
    - To multiple values depending on the shot distance
    - With custom logic in a callback, for example:
        - Increase damage for headshots
        - Increase/lower damage for combos
        - Lower damage for c-bug rapid fire
- **Knife sync fixed in both lagcomp and no-lagcomp**
- **New death animations and respawn logic**
    - Customize respawn time globally and for each death
    - Fully customizable animations, with a nice set of defaults
    - Different animation depending on weapon/bodypart, for example:
        - Headshots make you fall back with both hands in your face
        - Shotgun kills make you fly backward like in GTA:VC (unless killed from behind)

## Implementation details

All players are given infinite health and set to the same team. Damage is counted by the script whenever `OnPlayer(Give/Take)Damage` is called. The real `GetPlayerHealth` is **never** read by the include file.

The players healthbars are modified by editing SA-MP packets, so they are very responsive.

The death animations are applied as "forcesync" and even the facing angle is force synced (with SKY). This allows perfect animations even for laggy/paused players.

The *real* `OnPlayerDeath` is never called from the SA-MP server (only in some rare cases). A player never actually dies in their game - they just see a death animation applied and get respawned.

Many SA-MP functions are hooked to provide new values (such as `GetPlayerState`, `(Get/Set)Player(Health/Armour)`, `GetPlayerTeam`).

## Caveats

At the moment, filterscripts are not fully supported. If you need to modify the health, go into spec mode, change the virtual world, and more then you should do it through the gamemode.

A simple workaround is to just do something like this:

```pawn
// In the gamemode
forward SetHealth(playerid, Float:health);
public SetHealth(playerid, Float:health) {
    SetPlayerHealth(playerid, health);
}
// In the filterscript
CallRemoteFunction("SetHealth", "i", playerid, health);
```

# API

## Callbacks

### Existing callbacks

`OnPlayerGiveDamage` and `OnPlayerTakeDamage` are removed. Use `OnPlayerDamage` (see below).

`OnPlayerDeath` and `OnPlayerWeaponShot` are called as usual.

### New callbacks

```pawn
public OnPlayerDamage(playerid, &Float:amount, &issuerid, &weapon, &bodypart);
```
Called when damage is about to be inflicted on a player
Most arguments can be modified (e.g. the damage could be adjusted)
* `playerid` - The player who is about to get damaged
* `amount` - The amount of damage about to get inflicted (0.0 means all HP)
    This will not always be the same in OnPlayerDamageDone, for example:
    if amount is 50.0 and you have 10.0, only 10.0 will be reported in OnPlayerDamageDone
* `weapon` - The weapon used to inflict the damage
* `bodypart` - The bodypart

Return 0 to prevent the damage from being inflicted

```pawn
public OnPlayerDamageDone(playerid, Float:amount, issuerid, weapon, bodypart);
```
Called after damage has been inflicted

Same parameters as above, but they can not be modified

Return value ignored

```pawn
public OnPlayerPrepareDeath(playerid, animlib[32], animname[32], &anim_lock, &respawn_time);
```
Before the death animation is applied

* `playerid` - The player that is about to die
* `animlib` - The anim lib to play (change to empty string for no animation)
* `animname` - The anim name to play
* `anim_lock` - The "lock" parameter in ApplyAnimation
* `respawn_time` - The time (in milliseconds) until the player respawns

Return value ignored

```pawn
public OnPlayerDeathFinished(playerid);
```
When the death animation has finished and the player has been sent to respawn

* `playerid` - The player

Return value ignored

```pawn
public OnRejectedHit(playerid, hit[E_REJECTED_HIT]);
```
When a shot or damage given is rejected
See E_REJECTED_HIT and GetRejectedHit for more

* `playerid` - The player whose hit was rejected
* `hit` - An enum containing information about the rejected hit

Return value ignored

```pawn
public OnInvalidWeaponDamage(playerid, damagedid, Float:amount, weaponid, bodypart, error, bool:given);
```
When a player takes or gives invalid damage (WC_* errors above)
* `playerid` - The player that inflicted the damage
* `damagedid` - The player that took or was given a hit
* `amount` - The damage amount inflicted
* `weaponid` - The weapon used
* `bodypart` - The bodypart
* `error` - Which error (see the enum containing WC_NO_ERROR)
* `given` - `true` means that `playerid` reported the damage in `OnPlayerGiveDamage`,
    `false` means that `damagedid` reported the damage in `OnPlayerTakeDamage`

Return value ignored

## Functions

```pawn
AverageShootRate(playerid, shots, &multiple_weapons = 0);
```
The average time (in milliseconds) between shots.
* `playerid` - The player
* `hits` - Number of hits to average on (max 10)
* `multiple_weapons` - Will be set to 1 if different weapons were used in the last shots

Returns -1 if there is not enough data to calculate the rate, otherwise the average time is returned.

```pawn
AverageHitRate(playerid, hits, &multiple_weapons = 0);
```
Same as above, but for hits inflicted with `OnPlayerGiveDamage`

```pawn
DamagePlayer(playerid, Float:amount, issuerid = INVALID_PLAYER_ID, weaponid = WEAPON_UNKNOWN, bodypart = BODY_PART_UNKNOWN, bool:ignore_armour = false);
```
Inflict a hit on a player. All callbacks except `OnPlayerWeaponShot` will be called.
* `ignore_armour` - When `true` will do damage straight to health, and not armour.

```pawn
Float:GetPlayerHealth(playerid, &Float:health = 0.0);
```
Hooked version of the original, which also returns the value

```pawn
Float:GetPlayerArmour(playerid, &Float:armour = 0.0);
```
Hooked version of the original, which also returns the value

```pawn
GetRejectedHit(playerid, idx, output[], maxlength = sizeof(output));
```
Get a string explaining why the hit was rejected at idx (max. `WC_MAX_REJECTED_HITS - 1`)

```pawn
SetRespawnTime(ms);
```
Set the respawn time in milliseconds

```pawn
GetRespawnTime();
```
Get the respawn time

```pawn
IsBulletWeapon(weaponid);
```
Returns true if the weapon shoots bullets

```pawn
IsHighRateWeapon(weaponid);
```
Returns true if the weapon's damage can be reported in high rates to the server (such as fire)

```pawn
IsMeleeWeapon(weaponid);
```
Returns true if it's a melee weapon (including `WEAPON_PISTOLWHIP`)

```pawn
IsPlayerDying(playerid);
```
Returns true if the player is between the dying animation and spawning

```pawn
IsPlayerSpawned(playerid);
```
Returns true if the player is spawned and not in a dying animation

```pawn
GetWeaponName(weaponid, weapon[], len = sizeof(weapon));
```
Hooked version of the native, fixed and containing custom weapons (such as pistol whip)

```pawn
ReturnWeaponName(weaponid);
```
Return the weapon name (uses the fixed GetWeaponName)

```pawn
SetCustomFallDamage(bool:toggle, Float:damage_multiplier = 25.0, Float:death_velocity = -0.6);
```
Toggle custom falling damage

```pawn
SetCustomFallDamageMachines(bool:toggle);
```
Toggle vending machines (they are removed and disabled by default)

```pawn
SetDamageFeed(bool:toggle);
```
Toggle damage feed

```pawn
SetDamageSounds(taken, given);
```
Set sounds when damage is given and taken (0 to disable)

```pawn
SetVehiclePassengerDamage(bool:toggle);
```
Allow vehicles to be damaged when they have a passenger and no driver

```pawn
SetVehicleUnoccupiedDamage(bool:toggle);
```
Allow vehicles to be damaged when they don't have any players inside them

```pawn
SetWeaponDamage(weaponid, damage_type, Float:amount, Float:...);
```
Modify a weapon's damage
* `weaponid` - The weapon to modify
* `damage_type` - One of the following:
    * DAMAGE_TYPE_MULTIPLIER
        Multiply the original damage inflicted by amount
        This is default for melee, grenades, and other weapons that
        inflict different amounts of damage (shotguns excluded)
    * DAMAGE_TYPE_STATIC
        Inflict a specific amount of damage for each hit
        For shotguns, this modifies the value for each bullet that hit the player
        Combat shotgun shoots 8 bullets, other shotguns shoot 15
    * DAMAGE_TYPE_RANGE_MULTIPLIER
        Same as DAMAGE_TYPE_MULTIPLIER, but the damage depends on the distance
    * DAMAGE_TYPE_RANGE
        Same as DAMAGE_TYPE_STATIC, but the damage depends on the distance
* `amount` - The amount of damage
* `...` - If `damage_type` contains `RANGE`, the arguments should be a list of ranges and damage
    For example:
    ```pawn
    SetWeaponDamage(WEAPON_SNIPER, WEAPON_TYPE_RANGE, 40.0, 20.0, 30.0, 60.0, 20.0)
    ```
    This will make sniper damage:
    * `40` if distance is less than `20`
    * `30` if distance is less than `60`
    * `20` for any other distance

```pawn
SetWeaponMaxRange(weaponid, Float:range);
```
Set the max range of a weapon. The default value is those from weapon.dat
Because of a SA-MP bug, weapons can (and will) exceed this range.
This script, however, will block those out-of-range shots and give a rejected hit.

```pawn
SetWeaponShootRate(weaponid, max_rate);
```
Set the max allowed shoot rate of a weapon.
Could be used to prevent C-bug damage or allow infinite shooting if a script uses GivePlayerWeapon to do so.

```pawn
SetCustomArmourRules(bool:armour_rules, bool:torso_rules);
```
Toggle the custom armour rules on and off. Both are disabled by default.
* `armour_rules` - Toggle all of the rules. When off, nothing is affected. Armour is affected as it normally would. When on, weapons can be set to either damage armour before health or just take health and never damage armour.
* `torso_rules` - Toggle all torso-only rules. When off, all weapons will have effects no matter which bodypart is 'hit'. When on, weapons with the `torso_only` rule (of `SetWeaponArmourRule`) on will only damage armour when the torso is 'hit' (and when it's off, armour is damaged no matter which body part is 'hit').

```pawn
SetWeaponArmourRule(weaponid, bool:affects_armour, bool:torso_only);
```
Set custom rules for a weapon. The defaults aren't going to comfort EVERYONE, so everyone needs the ability to modify the weapons themselves.
* `weaponid` - The ID of the weapon to modify the rules of.
* `affects_armour` - Whether this weapon will distribute damage over armour and health or just damage health directly.
* `torso_only` - Whether this weapon will only damage armour when the 'hit' bodypart is the torso or all bodyparts. Only works when `torso_rules` are enabled using `SetCustomArmourRules`.
