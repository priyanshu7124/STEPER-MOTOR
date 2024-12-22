import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import serial
import time

# Establish serial communication
try:
    ser = serial.Serial("COM6", 9600, timeout=1)  # Replace "COM3" with your port
    time.sleep(2)  # Allow time for the connection to establish
except Exception as e:
    ser = None
    print(f"Serial connection error: {e}")

# Function to send commands to Arduino
def send_command(command):
    if ser and ser.is_open:
        ser.write((command + '\n').encode())
        time.sleep(0.1)  # Allow Arduino to process
    else:
        messagebox.showerror("Connection Error", "Serial connection not established!")

# Motor commands
def start_motor():
    motor = motor_var.get()
    send_command(f"START {motor}")

def stop_motor():
    motor = motor_var.get()
    send_command(f"STOP {motor}")

def set_speed():
    motor = motor_var.get()
    speed = speed_var.get()
    if 0 < speed <= 1000:
        send_command(f"SPEED {motor} {speed}")
    else:
        messagebox.showerror("Invalid Speed", "Speed must be between 1 and 1000!")

def set_direction():
    motor = motor_var.get()
    direction = direction_var.get()
    send_command(f"DIRECTION {motor} {direction}")

def set_time():
    motor = motor_var.get()
    hh = time_hours.get()
    mm = time_minutes.get()
    ss = time_seconds.get()
    send_command(f"TIME {motor} {hh:02}:{mm:02}:{ss:02}")

def set_repeat():
    motor = motor_var.get()
    hh = repeat_hours.get()
    mm = repeat_minutes.get()
    ss = repeat_seconds.get()
    send_command(f"REPEAT {motor} {hh:02}:{mm:02}:{ss:02}")

def set_cycle():
    motor = motor_var.get()
    cycles = cycle_var.get()
    send_command(f"CYCLE {motor} {cycles}")

# Create GUI window
root = tk.Tk()
root.title("Motor Control Interface")
root.geometry("500x500")

# Motor selection
ttk.Label(root, text="Select Motor:").grid(row=0, column=0, pady=10, sticky=tk.W)
motor_var = tk.StringVar(value="X")
ttk.Combobox(root, textvariable=motor_var, values=["X", "Y", "Z", "A"]).grid(row=0, column=1, pady=10)

# Direction
ttk.Label(root, text="Direction:").grid(row=1, column=0, pady=10, sticky=tk.W)
direction_var = tk.StringVar(value="FORWARD")
ttk.Combobox(root, textvariable=direction_var, values=["FORWARD", "BACKWARD"]).grid(row=1, column=1, pady=10)
ttk.Button(root, text="Set Direction", command=set_direction).grid(row=1, column=2, padx=10)

# Speed control
ttk.Label(root, text="Speed (1-1000):").grid(row=2, column=0, pady=10, sticky=tk.W)
speed_var = tk.IntVar(value=500)
ttk.Entry(root, textvariable=speed_var).grid(row=2, column=1, pady=10)
ttk.Button(root, text="Set Speed", command=set_speed).grid(row=2, column=2, padx=10)

# Time control
ttk.Label(root, text="Set Rotation Time (HH:MM:SS):").grid(row=3, column=0, pady=10, sticky=tk.W)
time_hours = tk.IntVar(value=0)
time_minutes = tk.IntVar(value=0)
time_seconds = tk.IntVar(value=0)
tk.Spinbox(root, from_=0, to=23, width=3, textvariable=time_hours).grid(row=3, column=1, padx=5, sticky=tk.W)
tk.Spinbox(root, from_=0, to=59, width=3, textvariable=time_minutes).grid(row=3, column=1, padx=50, sticky=tk.W)
tk.Spinbox(root, from_=0, to=59, width=3, textvariable=time_seconds).grid(row=3, column=1, padx=100, sticky=tk.W)
ttk.Button(root, text="Set Time", command=set_time).grid(row=3, column=2, padx=10)

# Repeat interval
ttk.Label(root, text="Set Repeat Interval (HH:MM:SS):").grid(row=4, column=0, pady=10, sticky=tk.W)
repeat_hours = tk.IntVar(value=0)
repeat_minutes = tk.IntVar(value=0)
repeat_seconds = tk.IntVar(value=0)
tk.Spinbox(root, from_=0, to=23, width=3, textvariable=repeat_hours).grid(row=4, column=1, padx=5, sticky=tk.W)
tk.Spinbox(root, from_=0, to=59, width=3, textvariable=repeat_minutes).grid(row=4, column=1, padx=50, sticky=tk.W)
tk.Spinbox(root, from_=0, to=59, width=3, textvariable=repeat_seconds).grid(row=4, column=1, padx=100, sticky=tk.W)
ttk.Button(root, text="Set Repeat Interval", command=set_repeat).grid(row=4, column=2, padx=10)

# Cycle control
ttk.Label(root, text="Number of Cycles:").grid(row=5, column=0, pady=10, sticky=tk.W)
cycle_var = tk.IntVar(value=1)
ttk.Entry(root, textvariable=cycle_var).grid(row=5, column=1, pady=10)
ttk.Button(root, text="Set Cycles", command=set_cycle).grid(row=5, column=2, padx=10)

# Start/Stop buttons
ttk.Button(root, text="Start Motor", command=start_motor).grid(row=6, column=0, pady=20)
ttk.Button(root, text="Stop Motor", command=stop_motor).grid(row=6, column=1, pady=20)

# Close serial on exit
def on_closing():
    if ser and ser.is_open:
        ser.close()
    root.destroy()

root.protocol("WM_DELETE_WINDOW", on_closing)
root.mainloop()
