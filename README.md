# touhou-opengl

The inspiration for this project is through various bullet-hell style games or [danmaku弾幕](https://en.touhouwiki.net/wiki/Danmaku), primarily the [Touhou Project](https://www.youtube.com/watch?v=8DRacK3_0wE). The primary interest in this project is built as a way to experiment and simplify bullet patterns representation in OpenGL games.

- [ ] Player class
  - [ ] lives counter (-1 for each time touched by a bullet, which 
  - [ ] power accumulated as time goes on
  - [ ] score goes up when surviving
- [ ] BulletPattern class
  - [ ] basic (to implement as baseline): 
    - [ ] horizontal
    - [ ] straight line
    - [ ] circle 
  - extra bulletpattern considerations:
    - [ ] priority 1: spiral
    - [ ] priority 2: multiple pivot points for spirals/circle
- [ ] background music
  - [ ] pulse sound when a new wave is fired
  - [ ] change in background music when surviving level
- [ ] Level class
  - enemies spawn as a source for bullets, and move around to make it hard to destroy
    - [ ] spawn bullets relative to position of enemies
    - [ ] moving enemies
    
extras!
- [ ] custom artwork sprites for bullets
- [ ] custom player sprites
- [ ] connection to an leaderboard
  - linking opengl with a website
    - [ ] make http post request
    - [ ] make leaderboard
    - [ ] link data to website
