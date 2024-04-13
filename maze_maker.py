import tkinter as tk
import tkinter.scrolledtext as scrolledtext

#THIS PROGRAM WAS WRITTEN ENTIRLY BY CHATGPT3.5

# Define color constants
WHITE = 0
RED = 1
BLUE = 2
GREEN = 3

def create_grid():
    try:
        width = int(width_entry.get())
        length = int(length_entry.get())
    except ValueError:
        result_label.config(text="Please enter valid integers for width and length.")
        return
    
    for widget in grid_frame.winfo_children():
        widget.destroy()
    
    for i in range(length):
        row_colors = []  # Initialize list to store colors of the current row
        for j in range(width):
            square = tk.Label(grid_frame, bg="white", relief="ridge", width=2, height=1)
            square.grid(row=i, column=j, padx=1, pady=1)
            square.bind("<Button-1>", lambda event, row=i, col=j: toggle_color(event, row, col))  # Bind left click event to toggle_color function
            
            # Add square color to colors list
            row_colors.append(WHITE)
        colors.append(row_colors)  # Add row colors to the colors list

def toggle_color(event, row, col):
    square = event.widget
    current_color = square["bg"]
    new_color = WHITE  # Default to white
    if current_color == "white":
        square.config(bg="red")
        new_color = RED
    elif current_color == "red":
        square.config(bg="blue")
        new_color = BLUE
    elif current_color == "blue":
        square.config(bg="green")
        new_color = GREEN
    elif current_color == "green":
        square.config(bg="white")
    
    # Update colors list with the new color of the clicked square
    colors[row][col] = new_color
    update_text_area()

def update_text_area():
    text_area.delete(1.0, tk.END)
    text_area.insert(tk.END, "[\n")
    for row in colors:
        text_area.insert(tk.END, "    [")
        for i, color in enumerate(row):
            text_area.insert(tk.END, f"{color}")
            if i < len(row) - 1:
                text_area.insert(tk.END, ", ")
        text_area.insert(tk.END, "],\n")
    text_area.insert(tk.END, "]\n")

def copy_to_clipboard():
    root.clipboard_clear()
    root.clipboard_append(text_area.get(1.0, tk.END))

def clear_grid():
    for widget in grid_frame.winfo_children():
        widget.destroy()
    width_entry.delete(0, tk.END)
    length_entry.delete(0, tk.END)
    result_label.config(text="")
    text_area.delete(1.0, tk.END)

def fill_grid_from_input():
    input_text = input_text_area.get(1.0, tk.END)
    try:
        input_data = eval(input_text)
        if isinstance(input_data, list) and all(isinstance(row, list) for row in input_data):
            # Resize colors list and grid frame
            rows = len(input_data)
            cols = max(len(row) for row in input_data)
            resize_grid(rows, cols)

            # Fill the grid
            for i, row in enumerate(input_data):
                for j, color in enumerate(row):
                    colors[i][j] = color
            update_grid()
        else:
            result_label.config(text="Invalid input format. Please provide a list of lists.")
    except Exception as e:
        result_label.config(text=f"Error: {e}")

def resize_grid(rows, cols):
    # Resize the colors list
    while len(colors) < rows:
        colors.append([WHITE] * cols)
    while len(colors) > rows:
        colors.pop()
    for row in colors:
        while len(row) < cols:
            row.append(WHITE)
        while len(row) > cols:
            row.pop()

    # Resize the grid frame
    for widget in grid_frame.winfo_children():
        widget.destroy()
    for i in range(rows):
        for j in range(cols):
            square = tk.Label(grid_frame, bg="white", relief="ridge", width=2, height=1)
            square.grid(row=i, column=j, padx=1, pady=1)
            square.bind("<Button-1>", lambda event, row=i, col=j: toggle_color(event, row, col))

def update_grid():
    for i, row in enumerate(colors):
        for j, color in enumerate(row):
            square = grid_frame.grid_slaves(row=i, column=j)[0]
            if color == WHITE:
                square.config(bg="white")
            elif color == RED:
                square.config(bg="red")
            elif color == BLUE:
                square.config(bg="blue")
            elif color == GREEN:
                square.config(bg="green")

def update_grid():
    for i, row in enumerate(colors):
        for j, color in enumerate(row):
            square = grid_frame.grid_slaves(row=i, column=j)[0]
            if color == WHITE:
                square.config(bg="white")
            elif color == RED:
                square.config(bg="red")
            elif color == BLUE:
                square.config(bg="blue")
            elif color == GREEN:
                square.config(bg="green")

# Create main window
root = tk.Tk()
root.title("Grid Generator")

# Create frame for grid
grid_frame = tk.Frame(root, borderwidth=2, relief="ridge")
grid_frame.pack(padx=10, pady=10)

# Create frame for input fields
input_frame = tk.Frame(root)
input_frame.pack(pady=5)

# Create width label and entry
width_label = tk.Label(input_frame, text="Width:")
width_label.grid(row=0, column=0)

width_entry = tk.Entry(input_frame)
width_entry.grid(row=0, column=1)

# Create length label and entry
length_label = tk.Label(input_frame, text="Length:")
length_label.grid(row=1, column=0)

length_entry = tk.Entry(input_frame)
length_entry.grid(row=1, column=1)

# Create buttons
generate_button = tk.Button(root, text="Generate Grid", command=create_grid)
generate_button.pack(side="left", padx=5)

clear_button = tk.Button(root, text="Clear Grid", command=clear_grid)
clear_button.pack(side="left", padx=5)

copy_button = tk.Button(root, text="Copy to Clipboard", command=copy_to_clipboard)
copy_button.pack(side="left", padx=5)

# Create label for displaying errors
result_label = tk.Label(root, fg="red")
result_label.pack(pady=5)

# Create colors list to store the colors of the squares
colors = []

# Create text area to display the list of colors
text_area = scrolledtext.ScrolledText(root, width=40, height=10)
text_area.pack(pady=5)

# Create input text area for filling grid from input
input_text_area = scrolledtext.ScrolledText(root, width=40, height=5)
input_text_area.pack(pady=5)

# Create button to fill grid from input
fill_button = tk.Button(root, text="Fill Grid from Input", command=fill_grid_from_input)
fill_button.pack(pady=5)

root.mainloop()
