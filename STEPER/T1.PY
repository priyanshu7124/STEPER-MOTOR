import tkinter as tk
from tkinter import messagebox
import serial
import time

# Initialize serial communication with Arduino (update COM port as needed)
try:
    arduino = serial.Serial(port='COM7', baudrate=9600, timeout=1)
except Exception as e:
    print("Error connecting to Arduino:", e)

# Function to send a command to Arduino
def send_command(command):
    if arduino.is_open:
        arduino.write((command + '\n').encode())
        time.sleep(0.1)  # Wait for Arduino to process command
        response = arduino.readline().decode().strip()
        print("Arduino Response:", response)
    else:
        messagebox.showerror("Connection Error", "Arduino not connected.")

# Command functions for GUI
def set_speed():
    speed = speed_entry.get()
    if speed.isdigit():
        send_command(f"S{speed}")
    else:
        messagebox.showerror("Input Error", "Enter a valid speed.")

def set_acceleration():
    acceleration = acceleration_entry.get()
    if acceleration.isdigit():
        send_command(f"A{acceleration}")
    else:
        messagebox.showerror("Input Error", "Enter a valid acceleration.")

def set_steps():
    steps = steps_entry.get()
    if steps.isdigit():
        send_command(f"M{steps}")
    else:
        messagebox.showerror("Input Error", "Enter a valid step count.")

def move_for_time():
    time_val = time_entry.get()
    if time_val.isdigit():
        send_command(f"T{time_val}")
    else:
        messagebox.showerror("Input Error", "Enter a valid time in seconds.")

def start_motors():
    send_command("START")

def stop_motors():
    send_command("STOP")

# GUI setup
root = tk.Tk()
root.title("Motor Control Interface")
root.geometry("400x300")

# Speed control
tk.Label(root, text="Speed:").grid(row=0, column=0, padx=10, pady=10)
speed_entry = tk.Entry(root)
speed_entry.grid(row=0, column=1)
tk.Button(root, text="Set Speed", command=set_speed).grid(row=0, column=2, padx=5)

# Acceleration control
tk.Label(root, text="Acceleration:").grid(row=1, column=0, padx=10, pady=10)
acceleration_entry = tk.Entry(root)
acceleration_entry.grid(row=1, column=1)
tk.Button(root, text="Set Acceleration", command=set_acceleration).grid(row=1, column=2, padx=5)

# Steps control
tk.Label(root, text="Steps:").grid(row=2, column=0, padx=10, pady=10)
steps_entry = tk.Entry(root)
steps_entry.grid(row=2, column=1)
tk.Button(root, text="Set Steps", command=set_steps).grid(row=2, column=2, padx=5)

# Time control
tk.Label(root, text="Time (seconds):").grid(row=3, column=0, padx=10, pady=10)
time_entry = tk.Entry(root)
time_entry.grid(row=3, column=1)
tk.Button(root, text="Move for Time", command=move_for_time).grid(row=3, column=2, padx=5)

# Start and Stop buttons
tk.Button(root, text="Start Motors", command=start_motors, bg="green", fg="white").grid(row=4, column=0, columnspan=2, pady=20)
tk.Button(root, text="Stop Motors", command=stop_motors, bg="red", fg="white").grid(row=4, column=2, pady=20)

# Run the GUI loop
root.mainloop()

# Close serial connection on exit
if arduino.is_open:
    arduino.close()
