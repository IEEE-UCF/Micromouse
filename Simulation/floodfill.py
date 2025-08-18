import API
import sys

def log(string):
    sys.stderr.write("{}\n".format(string))
    sys.stderr.flush()

def initializeFloodfill():
    if num_rows == num_columns and num_rows % 2 == 0 and num_columns % 2 == 0:
        center_rows = [num_rows // 2 - 1, num_rows // 2]  # Center square rows
        center_cols = [num_columns // 2 - 1, num_columns // 2]  # Center square columns
    else:
        sys.exit("maze parameters not defined for the program")

    # Initialize the 2D array
    floodfill_array = [[0 for _ in range(num_rows)] for _ in range(num_columns)]

    # Fill the array with Manhattan distances
    for i in range(num_rows):
        for j in range(num_columns):
            # Compute the minimum Manhattan distance to the center square
            distance = min(
                abs(i - center_rows[0]) + abs(j - center_cols[0]),
                abs(i - center_rows[0]) + abs(j - center_cols[1]),
                abs(i - center_rows[1]) + abs(j - center_cols[0]),
                abs(i - center_rows[1]) + abs(j - center_cols[1])
            )
            floodfill_array[i][j] = distance
            API.setText(i,j,distance)
    return floodfill_array

# initialization
position = [0,0,"U"] #x,y, U=Up, R=Right, D=Down, L=Left
API.setColor(0, 0, "G")
num_rows = 16
num_columns = 16
not_bordering_top_outside = True
not_bordering_right_outside = True
not_bordering_bottom_outside = False
not_bordering_left_outside = False

wall_array = [[0b00000000 for _ in range(num_columns)] for _ in range(num_rows)]
    # Note: byte arrays are arranged like this:
    # UNKNOWN(00), ABSENT(01), PRESENT(11), VIRTUAL(10)
    # in the order: U R D L

floodfill_array = initializeFloodfill()

def findGoal():
    while floodfill_array[position[0]][position[1]] != 0:
        log(f"Current position: {position}")

        updateWallStatus()

        # determine where the walls are around the current cell
        wall_up = wall_array[position[0]][position[1]] & 0b11000000 == 0b11000000
        wall_right = wall_array[position[0]][position[1]] & 0b00110000 == 0b00110000
        wall_down = wall_array[position[0]][position[1]] & 0b00001100 == 0b00001100
        wall_left = wall_array[position[0]][position[1]] & 0b00000011 == 0b00000011

        # retrieve the surrounding floodfill values
        cur_cell_floodfill = floodfill_array[position[0]][position[1]]
        up_cell_floodfill = floodfill_array[position[0]][position[1] + 1] if not_bordering_top_outside and not wall_up else 1000
        right_cell_floodfill = floodfill_array[position[0] + 1][position[1]] if not_bordering_right_outside and not wall_right else 1000
        down_cell_floodfill = floodfill_array[position[0]][position[1] - 1] if not_bordering_bottom_outside and not wall_down else 1000
        left_cell_floodfill = floodfill_array[position[0] - 1][position[1]] if not_bordering_left_outside and not wall_left else 1000

        # log floodfill values for debugging
        log(f"Floodfill value for current cell: {cur_cell_floodfill}")
        log("Floodfill values for adjacent cells:")
        log(f"up: {up_cell_floodfill}, right: {right_cell_floodfill}, down: {down_cell_floodfill}, left: {left_cell_floodfill}")

        # determine which direction floodfill says to go, if applicable
        minimum_neighbor = min(up_cell_floodfill,right_cell_floodfill,down_cell_floodfill,left_cell_floodfill)
        if minimum_neighbor < cur_cell_floodfill:
            target_dir = "up" # default direction
            if right_cell_floodfill == minimum_neighbor:
                target_dir = "right"
            if down_cell_floodfill == minimum_neighbor:
                target_dir = "down"
            if left_cell_floodfill == minimum_neighbor:
                target_dir = "left"
            log(f"Target direction: {target_dir}")
            
        else:
            queue = [(position[0], position[1], floodfill_array[position[0]][position[1]])] # put the current cell into the queue
            floodfill_visited = []
            floodFill(floodfill_array, position[0], position[1], queue, floodfill_visited)
            findGoal()
        
        # move in the direction decided by floodfill
        match target_dir:
            case "up":
                faceUp()
            case "right":
                faceRight()
            case "down":
                faceDown()
            case "left":
                faceLeft()
        moveForward()

        log("") # newline
    sys.exit()

def floodFill(floodfill_array, x, y, queue, visited):
    log("")
    log(f"Entering floodfill at ({x},{y})")

    # Check if the current cell is already visited
    if (x, y) in visited:
        log("Exiting floodfill:")
        findGoal() #continue
    
    # Mark the current cell as visited
    visited.append((x, y))
    
    # Remove the first element from the queue
    queue = queue[1:]

    neighbors = getNeighbors(x, y, floodfill_array)

    min_neighbor = 1000
    for neighbor in neighbors:
        if neighbor[2] < min_neighbor:  # Check the floodfill value (third element)
            min_neighbor = neighbor[2]
    
    log(f"The minimum neighbor value is {min_neighbor}")

    # If the current cell value is greater than or equal to than any neighbor, update it
    if floodfill_array[x][y] <=  min_neighbor:
        log("All neighbors have higher value:")
        floodfill_array[x][y] = 1 + min_neighbor
        
        # Add only unvisited neighbors to the queue
        for neighbor in neighbors:
            if (neighbor[0], neighbor[1]) not in visited:
                queue.append(neighbor)  # Append neighbors to the queue
        log(f"Queue: {queue}")

        API.setText(x, y, floodfill_array[x][y])

    # Continue processing cells in the queue
    while queue:
        x, y = queue[0][0], queue[0][1]  # Get coordinates of the first element
        floodFill(floodfill_array, x, y, queue, visited)

    log("Finished with floodfill: continuing")

def getNeighbors(x, y, floodfill_array):
    neighbors = []
    log(f"Getting the neighbors of ({x},{y})")
    
    # Ensure x and y are within bounds for floodfill_array and wall_array
    if x < 0 or y < 0 or x >= len(floodfill_array) or y >= len(floodfill_array[0]):
        return neighbors  # Return an empty list if out of bounds
    
    # Check walls for each direction using bitwise logic
    if not (wall_array[x][y] & 0b11000000 == 0b11000000):  # Check up wall
        if y + 1 < len(floodfill_array):
            up = (x, y + 1, floodfill_array[x][y + 1])
            neighbors.append(up)
    
    if not (wall_array[x][y] & 0b00110000 == 0b00110000):  # Check right wall
        if x + 1 < len(floodfill_array[0]):
            right = (x + 1, y, floodfill_array[x + 1][y])
            neighbors.append(right)
    
    if not (wall_array[x][y] & 0b00001100 == 0b00001100):  # Check down wall
        if y - 1 >= 0:
            down = (x, y - 1, floodfill_array[x][y - 1])
            neighbors.append(down)
    
    if not (wall_array[x][y] & 0b00000011 == 0b00000011):  # Check left wall
        if x - 1 >= 0:
            left = (x - 1, y, floodfill_array[x - 1][y])
            neighbors.append(left)
    
    log(neighbors)
    return neighbors

def updateWallStatus():
    # check if the robot is at any of the borders
    global not_bordering_top_outside, not_bordering_right_outside, not_bordering_bottom_outside, not_bordering_left_outside
    not_bordering_top_outside = position[1] < (num_rows - 1)
    not_bordering_right_outside = position[0] < (num_columns - 1)
    not_bordering_bottom_outside = position[1] > 0
    not_bordering_left_outside = position[0] > 0
    #log(f"On the border status: top: {not_bordering_top_outside}, right: {not_bordering_right_outside}, bottom: {not_bordering_bottom_outside}, left: {not_bordering_left_outside}")

    # Update current values for sensors depending on orientation
    if position[2] == 'U':
        wall_up = API.wallFront()
        wall_right = API.wallRight()
        wall_down = False
        wall_left = API.wallLeft()
    elif position[2] == 'R':
        wall_right = API.wallFront()
        wall_down = API.wallRight()
        wall_left = False
        wall_up = API.wallLeft()
    elif position[2] == 'D':
        wall_down = API.wallFront()
        wall_left = API.wallRight()
        wall_up = False
        wall_right = API.wallLeft()
    elif position[2] == 'L':
        wall_left = API.wallFront()
        wall_up = API.wallRight()
        wall_right = False
        wall_down = API.wallLeft()

    # Retrieve the current wall byte
    cur_wall_byte = wall_array[position[0]][position[1]]
    #log(f"wall byte before update -> {wall_array[position[0]][position[1]]:08b}")

    # Update the byte to say that the cell has been checked
    if cellHasNotBeenChecked():
        checkCell()
            
    # update the existance of the walls and their neighbors
    if wall_up and cur_wall_byte & 0b01000000 == 0b00000000:
        wall_array[position[0]][position[1]] |= 0b10000000
        API.setWall(position[0],position[1], "n")
        if not_bordering_top_outside:
            wall_array[position[0]][position[1] + 1] |= 0b00001000
            
    if wall_right and cur_wall_byte & 0b00010000 == 0b00000000:
        wall_array[position[0]][position[1]] |= 0b00100000
        API.setWall(position[0],position[1], "e")
        if not_bordering_right_outside:
            wall_array[position[0] + 1][position[1]] |= 0b00000010

    if wall_down and cur_wall_byte & 0b00000100 == 0b00000000:
        wall_array[position[0]][position[1]] |= 0b00001000
        API.setWall(position[0],position[1], "s")
        if not_bordering_bottom_outside:
            wall_array[position[0]][position[1] - 1] |= 0b10000000
    
    if wall_left and cur_wall_byte & 0b00000001 == 0b00000000:
        wall_array[position[0]][position[1]] |= 0b00000010
        API.setWall(position[0],position[1], "w")
        if not_bordering_left_outside:
            wall_array[position[0] - 1][position[1]] |= 0b00100000

    # Logging for debugging
    # prints the current cell wall status as an 8 bit binary number
    #log(f"wall byte array after update -> {wall_array[position[0]][position[1]]:08b}")

def checkCell():
    # update the wall array data to say that the cell and its neighbors have been checked
    log(not_bordering_right_outside)
    if position[2] == 'U':
        wall_array[position[0]][position[1]] |= 0b01010001
        if not_bordering_top_outside:
            wall_array[position[0]][position[1] + 1] |= 0b00000100
        if not_bordering_right_outside:
            wall_array[position[0] + 1][position[1]] |= 0b00000001
        if not_bordering_left_outside:
            wall_array[position[0] - 1][position[1]] |= 0b00010000

    elif position[2] == 'R':
        wall_array[position[0]][position[1]] |= 0b01010100
        if not_bordering_top_outside:
            wall_array[position[0]][position[1] + 1] |= 0b00000100
        if not_bordering_right_outside:
            wall_array[position[0] + 1][position[1]] |= 0b00000001
        if not_bordering_bottom_outside:
            wall_array[position[0]][position[1] - 1] |= 0b01000000

    elif position[2] == 'D':
        wall_array[position[0]][position[1]] |= 0b00010101
        if not_bordering_right_outside:
            wall_array[position[0] + 1][position[1]] |= 0b00000001
        if not_bordering_bottom_outside:
            wall_array[position[0]][position[1] - 1] |= 0b01000000
        if not_bordering_left_outside:
            wall_array[position[0] - 1][position[1]] |= 0b00010000 

    elif position[2] == 'L':
        wall_array[position[0]][position[1]] |= 0b01000101
        if not_bordering_top_outside:
            wall_array[position[0]][position[1] + 1] |= 0b00000100
        if not_bordering_bottom_outside:
            wall_array[position[0]][position[1] - 1] |= 0b01000000
        if not_bordering_left_outside:
            wall_array[position[0] - 1][position[1]] |= 0b00010000
    
def cellHasNotBeenChecked():
    if position[2] == 'U' and wall_array[position[0]][position[1]] & 0b01010001 != 0b01010001:
        return True 
    elif position[2] == 'R' and wall_array[position[0]][position[1]] & 0b01010100 != 0b01010100:
        return True 
    elif position[2] == 'D' and wall_array[position[0]][position[1]] & 0b00010101 != 0b00010101:
        return True 
    elif position[2] == 'L' and wall_array[position[0]][position[1]] & 0b01000101 != 0b01000101:
        return True 
    else:
        return False

def isWallLeft(): # determine if there is a wall on the left based on the wall array
    if position[2] == 'U' and wall_array[position[0]][position[1]] & 0b00000011 == 0b00000011:
        return True 
    elif position[2] == 'R' and wall_array[position[0]][position[1]] & 0b11000000 == 0b11000000:
        return True 
    elif position[2] == 'D' and wall_array[position[0]][position[1]] & 0b00110000 == 0b00110000:
        return True 
    elif position[2] == 'L' and wall_array[position[0]][position[1]] & 0b00001100 == 0b00001100:
        return True 
    else:
        return False

def isWallAhead(): # determine if there is a wall ahead based on the wall array
    if position[2] == 'U' and wall_array[position[0]][position[1]] & 0b10000000 == 0b10000000:
        return True
    elif position[2] == 'R' and wall_array[position[0]][position[1]] & 0b00100000 == 0b00100000:
        return True
    elif position[2] == 'D' and wall_array[position[0]][position[1]] & 0b00001000 == 0b00001000:
        return True
    elif position[2] == 'L' and wall_array[position[0]][position[1]] & 0b00000010 == 0b00000010:
        return True
    else:
        return False

def turnRight():
    log("turning right")
    API.turnRight()
    if(position[2]=="U"):
        position[2]="R"
    elif(position[2]=="R"):
        position[2]="D"
    elif(position[2]=="D"):
        position[2]="L"
    elif(position[2]=="L"):
        position[2]="U"

def turnLeft():
    log("turning left")
    API.turnLeft()
    if(position[2]=="U"):
        position[2]="L"
    elif(position[2]=="L"):
        position[2]="D"
    elif(position[2]=="D"):
        position[2]="R"
    elif(position[2]=="R"):
        position[2]="U"

def moveForward():
    log("moving forward")
    API.moveForward()
    if(position[2]=="U"):
        position[1]+=1
    elif(position[2]=="D"):
        position[1]-=1
    elif(position[2]=="R"):
        position[0]+=1
    elif(position[2]=="L"):
        position[0]-=1

def faceUp():
    if position[2] == "U":
        return
    elif position[2] == "R":
        turnLeft()
    elif position[2] == "D":
        turnRight()
        turnRight()
    elif position[2] == "L":
        turnRight()
    position[2] = "U"

def faceRight():
    if position[2] == "U":
        turnRight()
    elif position[2] == "R":
        return
    elif position[2] == "D":
        turnLeft()
    elif position[2] == "L":
        turnRight()
        turnRight()
    position[2] = "R"

def faceDown():
    if position[2] == "U":
        turnRight()
        turnRight()
    elif position[2] == "R":
        turnRight()
    elif position[2] == "D":
        return
    elif position[2] == "L":
        turnLeft()
    position[2] = "D"

def faceLeft():
    if position[2] == "U":
        turnLeft()
    elif position[2] == "R":
        turnRight()
        turnRight()
    elif position[2] == "D":
        turnRight()
    elif position[2] == "L":
        return
    position[2] = "L"

if __name__ == "__main__":
    findGoal()
