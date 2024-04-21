# Prestigious
This module allows you to prestige, resetting your level back to 1 and resetting quest progress.

> [!CAUTION]
> This module is a work in progress and a bit quirky. Use at your own risk.

## Setup
- `Clone` this module into your `./azerothcore-wotlk/modules` directory.
- `Re-run` CMake.
- `Compile` your core.
- `Enable` the module in the module config.
- `Start` the WorldServer to generate database tables.
- `Populate` the `prestige_rewards` template with items for each prestige level.
- `Reload` config with `.reload config`

## Notes
### prestige_rewards table definitions
- `entry:int` - This field is any entry from the `item_template` table.
- `count:int` - This is the amount of item to reward. _This is affected by multipliers in config if scalable is enabled_
- `scalable:bool` - Determines if the item count should scale with the config multipliers. 0 = no, 1 = yes.
- `prestige:int` - Determines what level prestige to reward the item. Set to 0 to reward for all levels.
