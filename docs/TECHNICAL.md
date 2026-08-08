# Rush Hour — Technical Notes

This document complements the main README with a closer look at the implementation currently present in `game.cpp`.

## Architecture

The project keeps most game-specific code in `game.cpp`, with drawing helpers in `util.cpp` and `util.h`.

At a high level, responsibilities are split between:

- `itemsonboard` — abstract positional base class for drawable board entities
- `Player` — abstract player state shared by Taxi and Delivery roles
- `Board` — owns and maintains board entities
- `Game` — controls menus, timer, HUD, themes, refueling, leaderboard state, and game-over handling
- GLUT callbacks — forward keyboard/timer/display events into the active game state

## Class Hierarchy

`itemsonboard` defines shared `x`/`y` coordinates, a pure virtual `draw()` function, and an overridable `update()` function.

Derived entity types include:

- `Obstacle`
  - `BoxObstacle`
  - `TreeObstacle`
- `OtherCar`
- `Passenger`
- `Package`
- `FuelStation`
- `Player`
  - `TaxiDriver`
  - `DeliveryDriver`

This allows the project to use inheritance and runtime polymorphism for entities that share a common position/drawing model but have different visual or gameplay behavior.

## Board Representation

The source defines:

```text
BOARD_ROWS = 26
BOARD_COLS = 26
TILE_SIZE  = 35
```

`BoardGrid` stores the logical map layout and `occupied` tracks positions already reserved by placed objects.

The initial map generation:

1. reserves regularly spaced rows/columns as roads
2. randomly assigns roughly 35% of the total grid to buildings
3. converts remaining unassigned cells to roads
4. places obstacles, fuel stations, packages, passengers, and traffic cars

## Reachability

Some generated task positions are checked with `isReachable()` before they are accepted.

`isReachable()` clears a `visited` matrix and calls the recursive `dfsVisit()` function. DFS explores the four cardinal directions and only traverses road cells that are not marked occupied.

For a board with `R × C` cells, the worst-case DFS traversal is:

```text
O(R × C)
```

The fixed 26 × 26 board keeps this search small in practice.

## Player Roles

Both roles derive from the abstract `Player` class, which stores:

- fuel
- score
- wallet balance
- completed-job count
- current passenger/package target

### Taxi Driver

A completed passenger trip adds 10 score points and the passenger's generated fare to the wallet.

Collision penalties:

- obstacle: -2
- traffic car: -3

### Delivery Driver

A completed package delivery adds 20 score points and the package's generated fare to the wallet.

Collision penalties:

- obstacle: -4
- traffic car: -5

The role can be switched during gameplay when the player is at `(0, 0)`. The replacement player object inherits the current position, fuel, wallet, score, and completed-job count.

## Dynamic Difficulty

After every two completed jobs, the game can add another `OtherCar` until `MAX_OTHER_CARS` is reached.

Existing traffic direction speeds are also increased, so progression affects both traffic density and movement rate.

`OtherCar::update()` moves cars along the road network and chooses a new direction when the next position leaves the board or is not a road tile.

## Fuel Economy

The player begins with 500 fuel units. Successful movement consumes one unit.

Fuel stations open a numeric refueling prompt when the player presses `R` nearby. The requested amount is constrained by:

- available wallet funds
- remaining fuel-tank capacity

The HUD displays score, wallet, fuel, remaining time, carried task state, and a graphical fuel bar.

## Timing and Game Over

`GAME_DURATION` is 180 seconds.

The update loop checks for game over when:

- elapsed time reaches 180 seconds
- fuel reaches zero
- score falls below zero

The game-over screen treats a score of at least `TARGET_SCORE` (100) as a win.

## Leaderboard Persistence

`Leaderboard` stores up to 10 `LeaderboardEntry` records. Entries contain a score and player name.

`highscores.txt` is opened in binary mode:

- `load()` uses `fread()`
- `save()` uses `fwrite()`

When a new score qualifies, `tryAddScore()` finds its descending insertion position, shifts lower entries, and saves the updated list.

With leaderboard size `H` capped at 10, insertion is `O(H)` and effectively constant in this project.

## Rendering and Event Loop

The game uses FreeGLUT callbacks for its event loop:

- `GameDisplay()` — rendering
- `NonPrintableKeys()` — arrow-key movement
- `PrintableKeys()` — menus and gameplay actions
- `Timer()` — periodic updates

The window is created at 1300 × 980 pixels.

SDL2_mixer is used separately for background music and WAV sound effects.

## Runtime Assets

The executable expects the supplied audio files to remain available in the working directory:

- `music.mp3`
- `pickup.mp3`
- `dropoff.mp3`
- `collision.mp3`
- `gameover.mp3`

The leaderboard file `highscores.txt` is runtime data rather than a required source asset.
