import matplotlib.pyplot as plt
import numpy as np
import turtle
import copy

max_distance=1000
maze_scale=30
turtle_offset=[300,-500]
direction=0

#THE print_grid FUNCTION WAS ENTIRLY WRITTEN BY CHATGPT3.5
def print_grid(data, cell_borders):
    # Convert the list to a NumPy array
    data_array = np.array(data)

    # Create a color map
    cmap = plt.get_cmap('coolwarm')

    # Plot the matrix as a grid, with color based on the data values
    plt.matshow(data_array, cmap=cmap, vmin=0, vmax=np.max(data_array), interpolation='nearest')

    # Add a color bar
    plt.colorbar()

    # Add number annotation inside each square
    for i in range(data_array.shape[0]):
        for j in range(data_array.shape[1]):
            plt.text(j, i, f'{int(data_array[i, j])}', ha='center', va='center', color='black')

    # Add custom cell borders
    for i in range(data_array.shape[0]):
        for j in range(data_array.shape[1]):
            border = cell_borders[i][j]
            if border == 0:
                continue  # No borders
            if border % 2 == 1:
                plt.plot([j - 0.5, j + 0.5], [i - 0.5, i - 0.5], color='black', linewidth=1)  # Top border
            if border >= 2:
                plt.plot([j - 0.5, j - 0.5], [i - 0.5, i + 0.5], color='black', linewidth=1)  # Left border

    # Show the plot
    plt.show()

def print_matrix(matrix):
	for line in matrix:
		print(line)

#0 nothing, 1 top, 2 left, 3 both
# maze=[
#     [3, 3, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 2],
#     [2, 2, 3, 1, 2, 2, 1, 3, 1, 2, 2, 3, 0, 3, 1, 2, 1, 3, 1, 1, 2],
#     [2, 2, 2, 2, 2, 3, 0, 3, 0, 2, 2, 2, 3, 0, 2, 1, 1, 0, 3, 0, 2],
#     [2, 0, 2, 2, 2, 1, 1, 2, 2, 0, 0, 2, 2, 2, 2, 3, 1, 1, 0, 3, 2],
#     [3, 1, 0, 2, 1, 0, 2, 2, 1, 2, 1, 0, 2, 1, 2, 3, 1, 2, 1, 0, 2],
#     [2, 1, 3, 1, 3, 1, 2, 3, 0, 3, 1, 1, 1, 0, 2, 2, 2, 1, 2, 3, 2],
#     [3, 2, 2, 1, 0, 2, 0, 2, 3, 1, 1, 0, 3, 1, 1, 2, 3, 0, 2, 0, 2],
#     [2, 0, 3, 1, 1, 1, 2, 2, 1, 0, 2, 0, 3, 2, 2, 0, 2, 1, 2, 1, 2],
#     [2, 3, 1, 2, 3, 0, 2, 2, 3, 1, 1, 0, 0, 2, 3, 1, 1, 2, 1, 0, 2],
#     [2, 0, 2, 1, 2, 3, 0, 3, 0, 3, 1, 2, 2, 0, 2, 1, 2, 1, 3, 0, 2],
#     [2, 1, 3, 0, 0, 2, 1, 0, 3, 2, 0, 2, 2, 1, 3, 2, 3, 0, 0, 3, 2],
#     [3, 2, 1, 3, 1, 3, 0, 1, 0, 1, 2, 3, 1, 2, 2, 0, 3, 1, 2, 2, 2],
#     [2, 3, 0, 2, 2, 1, 1, 2, 3, 1, 0, 2, 2, 1, 2, 1, 2, 1, 2, 0, 2],
#     [2, 0, 2, 2, 3, 1, 2, 1, 0, 3, 3, 1, 1, 2, 1, 0, 3, 0, 2, 1, 2],
#     [3, 3, 0, 2, 1, 2, 1, 3, 1, 0, 2, 1, 2, 3, 1, 2, 2, 1, 2, 2, 2],
#     [2, 2, 1, 1, 0, 2, 2, 2, 1, 2, 3, 0, 1, 2, 2, 0, 3, 0, 0, 2, 2],
#     [2, 1, 1, 3, 1, 2, 2, 1, 0, 2, 1, 3, 0, 0, 3, 1, 2, 2, 3, 2, 2],
#     [2, 1, 1, 2, 2, 0, 1, 1, 1, 2, 2, 0, 3, 1, 2, 2, 0, 2, 2, 0, 2],
#     [2, 2, 2, 2, 2, 1, 0, 3, 0, 2, 3, 1, 1, 0, 2, 3, 1, 2, 2, 3, 2],
#     [2, 0, 2, 0, 3, 1, 1, 0, 3, 0, 2, 1, 1, 1, 0, 2, 2, 0, 2, 0, 2],
#     [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
# ]

maze=[
    [3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 2],
    [2, 3, 3, 1, 1, 0, 0, 3, 1, 1, 2, 1, 2],
    [2, 0, 1, 1, 1, 1, 2, 2, 3, 2, 2, 2, 2],
    [2, 3, 1, 1, 1, 0, 1, 0, 2, 0, 0, 2, 2],
    [2, 1, 1, 1, 1, 2, 3, 1, 0, 1, 2, 2, 2],
    [3, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2, 0, 2],
    [2, 1, 1, 1, 0, 2, 0, 3, 1, 1, 2, 3, 2],
    [2, 1, 1, 3, 2, 2, 1, 2, 3, 2, 2, 2, 2],
    [2, 1, 2, 2, 2, 1, 1, 0, 2, 2, 2, 0, 2],
    [3, 1, 0, 2, 2, 2, 1, 1, 0, 0, 3, 2, 2],
    [3, 2, 3, 0, 2, 2, 2, 1, 1, 3, 2, 2, 2],
    [2, 0, 1, 0, 2, 2, 1, 1, 1, 0, 2, 0, 2],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
]

# maze=[
#     [3, 1, 3, 1, 3, 3, 1, 1, 1, 1, 2],
#     [2, 1, 0, 2, 2, 2, 2, 3, 2, 1, 2],
#     [3, 1, 1, 0, 2, 0, 2, 2, 1, 0, 2],
#     [2, 2, 2, 1, 1, 1, 2, 1, 3, 1, 2],
#     [2, 1, 3, 3, 0, 2, 1, 0, 0, 2, 2],
#     [2, 2, 2, 0, 2, 2, 0, 1, 1, 2, 2],
#     [2, 2, 2, 3, 1, 2, 3, 1, 2, 0, 2],
#     [2, 2, 1, 2, 2, 1, 0, 2, 1, 2, 2],
#     [2, 3, 0, 2, 1, 1, 3, 1, 0, 2, 2],
#     [2, 1, 1, 1, 0, 3, 0, 3, 1, 0, 2],
#     [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
# ]


def neighbors(y,x,bias="None"):
    if bias==0:
        neighbors=[[y,x+1,"right"],[y-1,x,"up"],[y+1,x,"down"],[y,x-1,"left"]]
    elif bias==180:
        neighbors=[[y,x-1,"left"],[y-1,x,"up"],[y+1,x,"down"],[y,x+1,"right"]]
    elif bias==270:
        neighbors=[[y+1,x,"down"],[y,x-1,"left"],[y,x+1,"right"],[y-1,x,"up"]]
    else:
        neighbors=[[y-1,x,"up"],[y,x-1,"left"],[y,x+1,"right"],[y+1,x,"down"]]
    return(neighbors)

def maze_distance(walls,start_y,start_x):
    def recurse(distances,y,x):
        #check if cell is in bounds
        if not((y>=0 and y<len(distances)) and (x>=0 and x<len(distances[0]))):
            return distances
        
        #determine distance
        lowest_distance=distances[y][x]
        found_lower=0
        for cell in neighbors(y,x):
            if not((cell[0]>=0 and cell[0]<len(distances)) and (cell[1]>=0 and cell[1]<len(distances[0]))):
                 continue
            if lowest_distance>distances[cell[0]][cell[1]]: #does this neighboring cell have a lowest distance
                if (cell[2]=="up" and walls[y][x]!=1 and walls[y][x]!=3):
                    found_lower=1
                    lowest_distance=distances[cell[0]][cell[1]]
                elif (cell[2]=="right" and walls[y][x+1]!=2 and walls[y][x+1]!=3):
                    found_lower=1
                    lowest_distance=distances[cell[0]][cell[1]]
                elif (cell[2]=="down" and walls[y+1][x]!=1 and walls[y+1][x]!=3):
                    found_lower=1
                    lowest_distance=distances[cell[0]][cell[1]]
                elif (cell[2]=="left" and walls[y][x]!=2 and walls[y][x]!=3):
                    found_lower=1
                    lowest_distance=distances[cell[0]][cell[1]]
        if found_lower==1:
            distances[y][x]=lowest_distance+1
        else:
             distances[y][x]=lowest_distance
        if not(distances[y][x]<visited[y][x]):
            return distances
        visited[y][x]=distances[y][x]
        #recurse
        for cell in neighbors(y,x):
            if not((cell[0]>=0 and cell[0]<len(distances)) and (cell[1]>=0 and cell[1]<len(distances[0]))):
                continue
            if not(distances[cell[0]][cell[1]]<=lowest_distance):
                distances=recurse(distances,cell[0],cell[1])
            else:
                continue
        return distances
    
    #initialize distances matrix to all max
    distances=[]
    for i in range(len(walls)-1):
        working_list=[]
        for k in range(len(walls[0])-1):
              working_list.append(max_distance)
        distances.append(working_list)
    visited=[]
    for i in range(len(walls)-1):
        working_list=[]
        for k in range(len(walls[0])-1):
              working_list.append(max_distance)
        visited.append(working_list)
    #check start cell is in bounds
    if not((start_y>=0 and start_y<len(distances)) and (start_x>=0 and start_x<len(distances[0]))):
        print("INVALID START POINT")
        exit()
    distances[start_y][start_x]=0 #set distance at start cell to zero
    distances=recurse(distances,start_y,start_x)
    return([walls,distances])
    
def next_move(walls,start_coords,goal_coords):
    if start_coords == goal_coords:
        return("No move needed; Already at goal")
    distances=maze_distance(walls,goal_coords[0],goal_coords[1])[1]
    start_distance=distances[start_coords[0]][start_coords[1]]
    y=start_coords[0]
    x=start_coords[1]
    found_lower=0
    for cell in neighbors(y,x,direction):
        if not((cell[0]>=0 and cell[0]<len(distances)) and (cell[1]>=0 and cell[1]<len(distances[0]))): #Is the neighboring cell in bounds
                continue
        if start_distance>distances[cell[0]][cell[1]]: #does this neighboring cell have a lower distance
            if (cell[2]=="up" and walls[y][x]!=1 and walls[y][x]!=3):
                found_lower=1
                return("Up")
            elif (cell[2]=="right" and walls[y][x+1]!=2 and walls[y][x+1]!=3):
                found_lower=1
                return("Right")
            elif (cell[2]=="down" and walls[y+1][x]!=1 and walls[y+1][x]!=3):
                found_lower=1
                return("Down")
            elif (cell[2]=="left" and walls[y][x]!=2 and walls[y][x]!=3):
                found_lower=1
                return("Left")
    if found_lower == 0:
        return("Unsolvable Maze")

def measure():
    memory[current_position[0]][current_position[1]]=maze[current_position[0]][current_position[1]]
    if (maze[current_position[0]+1][current_position[1]]==1 or maze[current_position[0]+1][current_position[1]]==3):
        if memory[current_position[0]+1][current_position[1]]==3:
            pass
        elif memory[current_position[0]+1][current_position[1]]==2:
            memory[current_position[0]+1][current_position[1]]=3
        else:
            memory[current_position[0]+1][current_position[1]]=1
    if (maze[current_position[0]][current_position[1]+1]==2 or maze[current_position[0]][current_position[1]+1]==3):
        if memory[current_position[0]][current_position[1]+1]==3:
            pass
        elif memory[current_position[0]][current_position[1]+1]==1:
            memory[current_position[0]][current_position[1]+1]=3
        else:
            memory[current_position[0]][current_position[1]+1]=2
    draw_cell(current_position[0],current_position[1],memory[current_position[0]][current_position[1]],"green")
    draw_cell(current_position[0]+1,current_position[1],memory[current_position[0]+1][current_position[1]],"green")
    draw_cell(current_position[0],current_position[1]+1,memory[current_position[0]][current_position[1]+1],"green")

def navigate(objective):
    measure()
    move=0
    while (move != "No move needed; Already at goal"):
        measure()
        move=next_move(memory,current_position,objective)
        make_move(move)
        print(move)
    print("Done!")

def sprint(objective):
    global current_position
    global direction
    print("Returning to start")
    navigate(start_coords)
    real_direction=copy.deepcopy(direction)
    moves=[]
    move=0
    print("Calculating shortest route")
    while (move != "No move needed; Already at goal"):
        move=next_move(memory,current_position,objective)
        if move == "Up":
            direction=90
            current_position[0]=current_position[0]-1
        elif move == "Left":
            direction=180
            current_position[1]=current_position[1]-1
        elif move == "Right":
            direction=0
            current_position[1]=current_position[1]+1
        elif move == "Down":
            direction=270
            current_position[0]=current_position[0]+1
        moves.append(move)
    print("Done!")
    current_position=copy.deepcopy(start_coords)
    direction=copy.deepcopy(real_direction)
    previous_move="Sprint!"
    move_counter=1
    for move in moves:
        if move == previous_move:
            move_counter=move_counter+1
            previous_move=move
            continue
        else:   
            make_move(previous_move,move_counter)
            if move_counter==1:
                print(previous_move)
            else:
                print(str(previous_move)+" "+str(move_counter)+" times")
            previous_move=move
            move_counter=1
    print("Done!")

def draw_cell(cell_y,cell_x,borders,color):
    turtle.color(color)
    turtle.speed(0)
    if borders==1 or borders==3:
        turtle.penup()
        turtle.setpos(cell_x*maze_scale+turtle_offset[1],-1*cell_y*maze_scale+turtle_offset[0])
        turtle.pendown()
        turtle.setheading(0)
        turtle.forward(maze_scale)
        turtle.penup()
    if borders==2 or borders==3:
        turtle.penup()
        turtle.setpos(cell_x*maze_scale+turtle_offset[1],-1*cell_y*maze_scale+turtle_offset[0])
        turtle.pendown()
        turtle.setheading(270)
        turtle.forward(maze_scale)
        turtle.penup()
    if color!="black":
        turtle.penup()
        turtle.speed(0)
        turtle.setpos(current_position[1]*maze_scale+maze_scale*.5+turtle_offset[1],-1*(current_position[0]*maze_scale+maze_scale*.5)+turtle_offset[0])
        turtle.setheading(direction)
        turtle.speed(3)
        turtle.color(current_color)


def draw_maze(maze):
    for i in range(len(maze)):
        for k in range(len(maze[0])):
            draw_cell(i,k,maze[i][k],"black")
    turtle.setpos(turtle_offset[1],turtle_offset[0])
    turtle.speed(3)

def draw_start_and_end(start_coords,end_coords):
    def draw_colored_square(coords,color):
        turtle.color(color)
        turtle.penup()
        turtle.speed(0)
        turtle.setpos(coords[1]*maze_scale+maze_scale*.33+turtle_offset[1],-1*(coords[0]*maze_scale+maze_scale*.33)+turtle_offset[0])
        turtle.setheading(0)
        turtle.pendown()
        for i in range(4):
            turtle.forward(maze_scale*.33)
            turtle.right(90)
        turtle.penup()
    draw_colored_square(start_coords,"green")
    draw_colored_square(end_coords,"red")

def turnright():
    global direction
    turtle.right(90)
    direction=direction-90
    while direction<0:
        direction=direction+360

def turnleft():
    global direction
    turtle.left(90)
    direction=direction+90
    while direction>=360:
        direction=direction-360

def make_move(way,number=1):
    turtle.penup()
    turtle.speed(0)
    turtle.color(current_color)
    turtle.setpos(current_position[1]*maze_scale+maze_scale*.5+turtle_offset[1],-1*(current_position[0]*maze_scale+maze_scale*.5)+turtle_offset[0])
    turtle.setheading(direction)
    turtle.speed(3)
    turtle.pendown()
    if way=="Up":
        if direction==90:
            pass
        elif direction==0:
            turnleft()
        elif direction==180:
            turnright()
        else:
            turnright()
            turnright()
        for i in range(number):
            turtle.forward(maze_scale)
            current_position[0]=current_position[0]-1
    elif way=="Left":
        if direction==180:
            pass
        elif direction==90:
            turnleft()
        elif direction==270:
            turnright()
        else:
            turnright()
            turnright()
        for i in range(number):
            current_position[1]=current_position[1]-1
            turtle.forward(maze_scale)
    elif way=="Right":
        if direction==0:
            pass
        elif direction==90:
            turnright()
        elif direction==270:
            turnleft()
        else:
            turnright()
            turnright()
        for i in range(number):
            current_position[1]=current_position[1]+1
            turtle.forward(maze_scale)
    elif way=="Down":
        if direction==270:
            pass
        elif direction==0:
            turnright()
        elif direction==180:
            turnleft()
        else:
            turnright()
            turnright()
        for i in range(number):
            current_position[0]=current_position[0]+1
            turtle.forward(maze_scale)
    turtle.penup()

memory=[]
for i in range(len(maze)):
    working_list=[]
    for k in range(len(maze[0])):
            working_list.append(0)
    memory.append(working_list)
goal_coords=[10,10]#y,x
start_coords=[0,0]
current_position=copy.deepcopy(start_coords)
print_matrix(memory)
output=maze_distance(maze,goal_coords[0],goal_coords[1]) #print distances
print_grid(output[1],output[0]) #print distances
turtle.width(1)
#draw_maze(maze)
turtle.width(4)
draw_start_and_end(start_coords,goal_coords)
turtle.setpos(start_coords[1]*maze_scale+maze_scale*.5+turtle_offset[1],-1*(start_coords[0]*maze_scale+maze_scale*.5)+turtle_offset[0])
turtle.speed(2)
current_color="blue"
# memory=maze
print("Navigating to goal")
navigate(goal_coords)
print("Navigating back to start")
current_color="yellow"
navigate(start_coords)
current_color="red"
sprint(goal_coords)

turtle.done()
