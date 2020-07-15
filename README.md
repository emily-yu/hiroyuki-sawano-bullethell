# touhou-opengl

The inspiration for this project is through various bullet-hell style games or [danmaku弾幕](https://en.touhouwiki.net/wiki/Danmaku), primarily the [Touhou Project](https://www.youtube.com/watch?v=8DRacK3_0wE). The primary interest in this project is built as a way to experiment and simplify bullet patterns representation in OpenGL games.

- [ ] Player class
  - [ ] lives counter
  - [ ] power accumulated as time goes on
  - [ ] score counter
- [ ] BulletPattern class
  - [ ] basic (to implement as baseline): 
    - [x] horizontal
    - [ ] straight line
    - [x] circle 
  - collisions
    - [ ] contact with player subtracts life
    - [ ] port out BulletPattern to track location for collisions
  - extra bulletpattern considerations:
    - [x] priority 1: spiral
    - [x] priority 2: multiple pivot points for spirals/circle
- [ ] background music
  - [x] pulse sound when a new wave is fired
  - [ ] change in background music when surviving level
- [ ] Level class
  - game mechanics
    - [ ] reset screen with bullets at last saved time (if lives > 0) if player dies
  - bullet construction
    - [x] bullet construction at developer set times
    - [ ] sync bullet timings for level with a song (better extract and sync with beat to make it autonomous)
  - time based tracking
    - [ ] score tracking
    - [ ] power level tracking
  - enemies spawn as a source for bullets, and move around to make it hard to destroy
    - [ ] spawn bullets relative to position of enemies (moving enemies with child BulletPatterns)
    - [ ] move enemy pivot points around based on location of player
    - [ ] construct structure to store xyz amount of enemies before final boss
- [ ] Assets
  - [ ] player spritesheet
  - [ ] bullet sprites (at least 5)
  - [ ] enemy sprites
  - [ ] find music backtrack to sync level bullets to manually

extras!
- [ ] custom artwork sprites for bullets
- [ ] custom player sprites
- [ ] connection to an leaderboard
  - linking opengl with a website
    - [ ] make http post request
    - [ ] make leaderboard
    - [ ] link data to website
