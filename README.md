# SnakegamePlus🐍

- [SnakegamePlus🐍](#snakegameplus)
  - [Brief Introduction](#brief-introduction)
    - [Part I. Single Player mode](#part-i-single-player-mode)
    - [Part II. Two Players mode](#part-ii-two-players-mode)
    - [Part III. Just Eat](#part-iii-just-eat)
    - [Part IV. Entertainment mode](#part-iv-entertainment-mode)
      - [Enjoy our game😊!](#enjoy-our-game)

## Brief Introduction
An advanced version based on the fundamental edition of the snakegame.
Completed by DZX, QJC and XKY in Summer, 2024. 
Supported by PDCurses and SFML.
Everything available on [Our GitHub](https://github.com/PasserbyZzz/SnakegamePlus).

Players can choose from four kinds of mode in our game: *Single Player*, *Two Players*, *Just Eat*, *Entertainment*.
Explanations are as follows.

### Part I. Single Player mode
In this mode, you'll control a green and white snake by `W A S D` or `↑ ↓ ← →`. Just follow the rules of the classic snakegame except for 3 aspects:

1. **Obstacles:** `White square` symbolize a obstacle. Please avoid crashing into them, or the game will over. However, you can press `Y` or `y`, which make you **UNSTOPPABLE** temporarily when you're about to crash into the obstacle. 
2. **Portal Gates:** `O` which appears in pairs stands for a portal gate. When you travel into one of them, you'll come out from another in the same color. 
3. **Blue Food:** `#` in white is just a simple food, while `#` in blue can provide you with a transitory acceleration. 

### Part II. Two Players mode
In this mode, P1 will control a blue and white snake, while P2 a red and white snake. Everything is the same with *Single Player mode* except that:
1.  `Y` and `y` are disabled. 
2.  Player who scores more points in the limited time will win. 
3.  Player who crashed into a obstacle or the wall immediately loses. 
4.  A tie will be reached when one snake's head crashes into the other's.

### Part III. Just Eat
In this mode, you'll see a large amount of food with different values.
 - Magenta food: 1 point
 - Red food: 2 points and a transitory acceleration
 - Yellow food: 3 points
 - Blue food: 5 points
  
  Obstacles, `Y` and `y` are reserved,  while portal gates are banned. Try to score as many as points as you can within the limited time.

### Part IV. Entertainment mode
In this mode, The game is divided into two states: **Scoring State and Penalty State**. Each round lasts 200 moves.

1. **Within each round**:

 - The first 100 moves are **the Scoring State**: Food is represented by a hash mark `#`.
 - The following 100 moves are **the Penalty State**: Food is represented by a square (Just like the obstacles).
 - After consuming a piece of food, **the score will increase or decrease**  accoring to the state by **the point value** of the food.
 - After consuming a piece of food, **the snake's length** will increase or decrease by **1**.


2. The Snake **won't die** when: 
- Touching the walls

- Touching the white obstacles

- Touching its own body
  
3. The Snake **will die** when:

- Its length **reaches 0** or **exceeds 30**.

- When transitioning from the Scoring State to the Penalty State, the snake will die if its length is less than 15.

- When transitioning from the Penalty State to the Scoring State, the snake will die if its length is greater than 15.

4. Additional hint:
-  About the length: The snake's length is displayed in the center of the screen to help players manage their length.
- **Bonus Scoring**: Achieving 20 or more food items consumed **during the Penalty State** of a round earns a bonus score, which is three times the points scored during the Scoring State of that round.
- Key Effect: Pressing the 'H' key accelerates the snake and slow down the passage of time, allowing it to consume more food in a short period of time.
- Portals: Portals can still be used as intended.

5.  Intention: This mode encourages players to consume high-point food in the Scoring State and low-point food in the Penalty State to ensure the snake's length changes and to try to earn more scores.

#### Enjoy our game😊!
