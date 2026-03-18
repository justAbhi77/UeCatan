# UE Catan Prototype

This project now contains a small **multiplayer, turn-based Unreal Engine 5 board game prototype** inspired by Catan.

## What was added

- A replicated game mode, game state, player state, and player controller.
- A procedural mini-board made from Unreal basic shapes.
- A top-down camera pawn so every player can see the whole board.
- A lightweight HUD that explains the available commands and shows the latest match log.

## How the prototype works

This is intentionally simpler than full Catan so the networking and turn flow stay easy to understand:

1. Players join the session and receive a turn order.
2. The server creates a small hex-style board with resource tiles.
3. Each player can place a **free first settlement** on an empty non-desert tile.
4. On later turns, players:
   - roll dice with `RollDice`
   - collect resources from owned tiles that match the dice result
   - spend resources to build another settlement with `BuildTile <TileId>`
   - end the turn with `EndTurn`
5. The first player to reach **3 victory points** wins.

## Why it is structured this way

- **Server-authoritative gameplay:** all important actions happen in `ACatanGameMode`, which keeps multiplayer behavior predictable.
- **Replicated state:** tiles, resources, turn ownership, and the log are stored in replicated state classes so every client sees the same match.
- **Simple interactions:** using console `Exec` commands keeps the prototype easy to test in PIE multiplayer without requiring UI widgets or input mapping assets.
- **Commented code:** the gameplay classes include comments describing the purpose of the major systems so the project is easy to extend.

## Useful play commands

Open the Unreal console while running a multiplayer PIE session and use:

- `RollDice`
- `BuildTile 3`
- `EndTurn`

The tile ids are rendered directly above the board tiles.
