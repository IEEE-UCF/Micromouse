# Notes for Maze Design:

**Micromouse competition maze specification** (From the official rules):
- The maze is composed of 18cm x 18cm unit squares arranged as 16 x 16 units.
- The walls of the units of the maze are 5 cm (~2 in) high and 1.2 cm (~0.5 in) thick.
- The outside wall encloses the entire maze.
- The sides of the maze walls are white, the tops of the walls are red, and the floor is black. The maze is made of wood, finished with non-gloss paint.
- Small square zones (posts), each 1.2 cm x 1.2 cm (0.5 in x 0.5 in), at the four corners of each unit square are called lattice points. The maze is so constituted that there is at least one wall at each lattice point.
- The destination goal is a gateway to the four-cell square at the center of the maze. The destination square has only one gateway.

**Description**:
We are making only one quarter of the maze (8 cells x 8 cells instead of 16 x 16) because a full maze would be pricey. The cuts are designed such that an expanded maze will just be 4 tiled versions of the quarter-size maze.
We used two 4' x 8' x 0.5" sheets of sanded plywood to make the floors and the walls, and painted them with non-glossy paint accordingly.
The walls are connected to the posts by gluing connectors to both sides of each wall segment, so that they easily slide into the posts. This makes it easy to move them around or take them down while still being sturdy.

**Math**:
- Maximum number of walls: 1+64*3 = 193
- Typical number of walls: Probably less than 100
- Length of walls = 18 cm - 2\*(0.5\*post width + connector width) = 17.111 cm = 6.736614 in ~= 6 3/4 in
- Number of walls given from cutting the the rightmost rectangle on the maze cutout picture: 5\*(8 ft/length of walls) = 5\*(8 ft/6.73 in ) = 70 (enough for now)
- Number of posts needed: 9\*9 = 81
- Number of connectors needed = Number of walls \* 2 = 70\*2 = 140
- Post hole diameter: Can be anything as long as it fits inside the post. I chose 0.25 inches because I have a drill bit that size.

**Updated Math for New Post Design**:
- Length of walls = 18cm - (post width + connector width) = 6.536614 in = 16.6 cm
    - This is a maximum measurement. The cut wall cannot be any longer than this but it can be up to ~ 0.1 in/0.25 cm less so we will cut the walls to the length of 6.5 in/16.5 cm. The new design allows for a little wiggle room so that the cuts can vary a little bit.

**Extra Notes**:
The walls are supposed to be 5 cm (2 in) high but ours are closer to 4.5 cm (1.75 in) because I sanded the boards to be the same height. I don't think this will be a problem.
