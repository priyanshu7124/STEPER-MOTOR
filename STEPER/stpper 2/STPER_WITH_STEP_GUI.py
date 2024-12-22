import tkinter as tk
from tkinter import messagebox
import serial
import time

# Connect to the Arduino (adjust the port and baud rate if needed)
try:
    arduino = serial.Serial(port='COM6', baudrate=9600, timeout=1)
    time.sleep(2)
except Exception as e:
    print(f"Error connecting to Arduino: {e}")
    arduino = None

# Function to send a command to Arduino
def send_command(command):
    if arduino:
        arduino.write(f"{command}\n".encode())
        time.sleep(0.1)
        response = arduino.readline().decode().strip()
        if response:
            print(response)
    else:
        messagebox.showerror("Error", "Arduino not connected")

# Set speed command
def set_speed():
    speed = speed_entry.get()
    if speed.isdigit():
        send_command(f"S{speed}")
    else:
        messagebox.showerror("Invalid Input", "Speed must be a number")

# Set acceleration command
def set_acceleration():
    acceleration = accel_entry.get()
    if acceleration.isdigit():
        send_command(f"A{acceleration}")
    else:
        messagebox.showerror("Invalid Input", "Acceleration must be a number")

# Set move distance command
def set_move_distance():
    move_distance = move_entry.get()
    if move_distance.isdigit():
        send_command(f"M{move_distance}")
    else:
        messagebox.showerror("Invalid Input", "Move distance must be a number")

# Movement commands for each motor
def move_motor(motor, direction):
    command = motor if direction == "clockwise" else motor.lower()
    send_command(command)

# Start and Stop functions
# def start_motors():
#     send_command("START")

# def stop_motors():
#     send_command("STOP")

# Create the main GUI window
root = tk.Tk()
root.title("Arduino Motor Control GUI")

# Speed and Acceleration Controls
tk.Label(root, text="Set Speed:").grid(row=0, column=0, padx=5, pady=5)
speed_entry = tk.Entry(root)
speed_entry.grid(row=0, column=1, padx=5, pady=5)
tk.Button(root, text="Set Speed", command=set_speed).grid(row=0, column=2, padx=5, pady=5)

tk.Label(root, text="Set Acceleration:").grid(row=1, column=0, padx=5, pady=5)
accel_entry = tk.Entry(root)
accel_entry.grid(row=1, column=1, padx=5, pady=5)
tk.Button(root, text="Set Acceleration", command=set_acceleration).grid(row=1, column=2, padx=5, pady=5)

# Move Distance Control
tk.Label(root, text="Set Move Distance:").grid(row=2, column=0, padx=5, pady=5)
move_entry = tk.Entry(root)
move_entry.grid(row=2, column=1, padx=5, pady=5)
tk.Button(root, text="Set Move Distance", command=set_move_distance).grid(row=2, column=2, padx=5, pady=5)

# Motor Control Buttons
motors = ["X", "Y", "Z", "A"]
for i, motor in enumerate(motors):
    tk.Label(root, text=f"Motor {motor}").grid(row=3 + i, column=0, padx=5, pady=5)
    tk.Button(root, text="Clockwise", command=lambda m=motor: move_motor(m, "clockwise")).grid(row=3 + i, column=1, padx=5, pady=5)
    tk.Button(root, text="Anticlockwise", command=lambda m=motor: move_motor(m, "anticlockwise")).grid(row=3 + i, column=2, padx=5, pady=5)

# Start and Stop Buttons
# tk.Button(root, text="Start Motors", command=start_motors).grid(row=7, column=0, padx=5, pady=5)
# tk.Button(root, text="Stop Motors", command=stop_motors).grid(row=7, column=1, padx=5, pady=5)

root.mainloop()

# Close the serial connection when the GUI is closed
if arduino:
    arduino.close()
