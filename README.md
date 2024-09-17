# The Maze Project

## Introduction
The Maze is a 3D game developed using raycasting techniques. The project was implemented in C using the SDL2 library for rendering and input handling. The aim is to provide the player's perspective within a maze environment.

**Links:**
- [Repository](https://github.com/LinaAbdelkhalig/the_maze.git) | [Deployed Site](https://linaabdelkhalig.github.io/the_maze/) | [Blog Article](#)
- **Authors**:
  - *Lena Abdelrazig* - [LinkedIn Profile](https://www.linkedin.com/in/linaabdelkhalig) | [Github Profile](https://github.com/LinaAbdelkhalig)
  - *Nafisa Ali* - [LinkedIn Profile](https://www.linkedin.com/in/nafisa-hassan-ali) | [Github Profile](https://github.com/NafisaKaruri)

## Installation
To set up the project on your local machine, follow these steps:
1. **Clone the Repository**:
```sh
$ git clone https://github.com/LinaAbdelkhalig/the_maze.git
```
2. **Navigate to the Project Directory**:
```sh
$ cd the_maze
```
3. **Build the project**:
```sh
$ make
```

## Usage
1. **Launch the Game**: Run the game executable with the path to the map file as an argument:
```sh
$ ./maze maps/map.txt
```
2. **Game Controls**:
  - Rotate Left: <kbd>LEFT</kbd> or <kbd>a</kbd>
  - Rotate Right: <kbd>RIGHT</kbd> or <kbd>d</kbd>
  - Move Forward: <kbd>UP</kbd> or <kbd>w</kbd>
  - Move Backward: <kbd>DOWN</kbd> or <kbd>s</kbd>
  - Show/Hide Map: <kbd>m</kbd>
  - Make it Rain: <kbd>r</kbd>
  - Use Weapon: <kbd>SPACE</kbd>
  - Exit Game: <kbd>ESCAPE</kbd>

## Dependencies
- GCC compiler
- SDL2 library
- SDL_image library

## Features
- [x] Rendering walls with orientation-based coloring
- [x] Rotation and Moving controls
- [x] Collision detection
- [x] Map file parser
- [x] In-game map display
- [x] Wall, floor, and ceiling textures
- [x] Weapons
- [x] Rain effects
- [x] Sound effects
- [ ] Storyline
- [ ] Shadows
- [ ] Enemies
- [ ] Additional weapons

## Contributing
We welcome contribution to enhance the project. If you'd like to contribute, please follow these steps
1. Fork the repository.
2. Create a new branch (`git checkout -b feature/YourFeature`).
3. Commit your chandes (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a Pull Request with the details of your changes.

## Inspiration
This project was born out of our passion with video games and a deep curiosity about game development. We found ourselves working in this project and excited by the challenges to improve it and take it further.

## Challenges
As a first-time game developers, we encountered several challenges:
- We faced a steep learning curve, so we dedicated more time to study and bridge our knowledge gap.
- Implementing the raycasting algorithm was initially challenging due to its mathematical complexity.
- Find tutorials specific to SDL2 and raycasting was difficult. Most resources focused on either SDL2 for 2D games or raycasting with other libraries, so we adapted the general raycasting tutorials to SDL2.

## Resources
- [SDL2 Tutorials](https://lazyfoo.net/tutorials/SDL/index.php)
- [Raycasting Tutorials](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)
- [Alternative Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html)

Thank you for checking out The Maze Project! We hope you enjoy playing and exploring it as much as we enjoyed creating it. If you have any questions or feedback, feel free to reach out to us.
