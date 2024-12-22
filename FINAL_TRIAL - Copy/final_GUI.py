import tkinter as tk
from tkinter import messagebox
import serial
import threading
import time

# Setup serial communication (adjust COM port and baud rate as necessary)
try:
    ser = serial.Serial('COM6', 9600, timeout=1)  # Replace 'COM3' with your Arduino port
except:
    ser = None
    print("Could not connect to Arduino. Check your COM port.")

def send_command(command):
    """Send a command to the Arduino."""
    if ser and ser.is_open:
        ser.write((command + "\n").encode())
        time.sleep(0.1)
    else:
        messagebox.showerror("Error", "Arduino is not connected!")

def start_motor(axis):
    send_command(f"START {axis}")

def stop_motor(axis):
    send_command(f"STOP {axis}")

def set_repetition(axis, count):
    send_command(f"REPEAT MOTOR {axis} {count}")

def set_speed(axis, speed):
    send_command(f"SPEED {axis} {speed}")

def set_time(axis, duration):
    send_command(f"TIME {axis} {duration}")

def set_direction(axis, direction):
    send_command(f"DIRECTION {axis} {direction}")

def universal_start():
    send_command("START ALL")

def universal_stop():
    send_command("STOP ALL")

def universal_speed(speed):
    send_command(f"SPEED ALL {speed}")

def universal_time(duration):
    send_command(f"TIME ALL {duration}")

def universal_repetition(repeat):
    send_command(f"REPEAT ALL {repeat}")

def universal_direction(direction):
    send_command(f"DIRECTION ALL {direction}")

def create_motor_frame(master, axis):
    """Create a control panel for an individual motor."""
    frame = tk.LabelFrame(master, text=f"Motor {axis}", padx=10, pady=10)
    
    tk.Button(frame, text="Start", command=lambda: start_motor(axis)).grid(row=0, column=0, pady=5)
    tk.Button(frame, text="Stop", command=lambda: stop_motor(axis)).grid(row=0, column=1, pady=5)
    
    tk.Label(frame, text="Repetitions").grid(row=1, column=0, sticky='w')
    repetitions = tk.Entry(frame, width=5)
    repetitions.grid(row=1, column=1)
    tk.Button(frame, text="Set", command=lambda: set_repetition(axis, repetitions.get())).grid(row=1, column=2)
    
    tk.Label(frame, text="Speed").grid(row=2, column=0, sticky='w')
    speed = tk.Entry(frame, width=5)
    speed.grid(row=2, column=1)
    tk.Button(frame, text="Set", command=lambda: set_speed(axis, speed.get())).grid(row=2, column=2)
    
    tk.Label(frame, text="Time (s)").grid(row=3, column=0, sticky='w')
    duration = tk.Entry(frame, width=5)
    duration.grid(row=3, column=1)
    tk.Button(frame, text="Set", command=lambda: set_time(axis, duration.get())).grid(row=3, column=2)
    
    tk.Label(frame, text="Direction").grid(row=4, column=0, sticky='w')
    direction_var = tk.StringVar(value="FORWARD")
    tk.OptionMenu(frame, direction_var, "FORWARD", "BACKWARD").grid(row=4, column=1)
    tk.Button(frame, text="Set", command=lambda: set_direction(axis, direction_var.get())).grid(row=4, column=2)
    
    return frame

def create_universal_controls(master):
    """Create a control panel for universal settings."""
    frame = tk.LabelFrame(master, text="Universal Controls", padx=10, pady=10)
    
    tk.Button(frame, text="Start All", command=universal_start).grid(row=0, column=0, pady=5)
    tk.Button(frame, text="Stop All", command=universal_stop).grid(row=0, column=1, pady=5)
    
    tk.Label(frame, text="Speed").grid(row=1, column=0, sticky='w')
    speed = tk.Entry(frame, width=5)
    speed.grid(row=1, column=1)
    tk.Button(frame, text="Set", command=lambda: universal_speed(speed.get())).grid(row=1, column=2)
    
    tk.Label(frame, text="Time (s)").grid(row=2, column=0, sticky='w')
    duration = tk.Entry(frame, width=5)
    duration.grid(row=2, column=1)
    tk.Button(frame, text="Set", command=lambda: universal_time(duration.get())).grid(row=2, column=2)
    
    tk.Label(frame, text="Repetitions").grid(row=3, column=0, sticky='w')
    repetitions = tk.Entry(frame, width=5)
    repetitions.grid(row=3, column=1)
    tk.Button(frame, text="Set", command=lambda: universal_repetition(repetitions.get())).grid(row=3, column=2)
    
    tk.Label(frame, text="Direction").grid(row=4, column=0, sticky='w')
    direction_var = tk.StringVar(value="FORWARD")
    tk.OptionMenu(frame, direction_var, "FORWARD", "BACKWARD").grid(row=4, column=1)
    tk.Button(frame, text="Set", command=lambda: universal_direction(direction_var.get())).grid(row=4, column=2)
    
    return frame

def update_serial_output():
    """Continuously read and display serial output from Arduino."""
    while True:
        if ser and ser.in_waiting:
            try:
                output = ser.readline().decode().strip()
                if output:
                    console.insert(tk.END, output + "\n")
                    console.see(tk.END)
            except:
                pass
        time.sleep(0.1)

# Main application
root = tk.Tk()
root.title("4-Axis Motor Controller")

# Individual motor controls
motor_frame = tk.Frame(root)
motor_frame.pack(pady=10)

for axis in ['X', 'Y', 'Z', 'A']:
    create_motor_frame(motor_frame, axis).pack(side=tk.LEFT, padx=5)

# Universal controls
universal_frame = create_universal_controls(root)
universal_frame.pack(pady=10)

# Console output for feedback
console_frame = tk.LabelFrame(root, text="Console", padx=10, pady=10)
console_frame.pack(fill="both", expand=True, pady=10)

console = tk.Text(console_frame, height=10, state='normal')
console.pack(fill="both", expand=True)

# Start serial reading thread
serial_thread = threading.Thread(target=update_serial_output, daemon=True)
serial_thread.start()

root.mainloop()



# import tkinter as tk
# from tkinter import ttk
# import serial
# import threading

# # Set up serial communication
# try:
#     ser = serial.Serial('COM6', 9600, timeout=1)  # Replace 'COM6' with your correct port
# except Exception as e:
#     ser = None
#     print(f"Serial Error: {e}")

# # Send a command to the Arduino
# def send_command(command):
#     if ser and ser.is_open:
#         ser.write((command + "\n").encode('utf-8'))
#     else:
#         print("Serial communication is not active.")

# # GUI Class
# class MotorControllerApp:
#     def __init__(self, root):
#         self.root = root
#         self.root.title("Motor Control Panel")
#         self.root.geometry("600x400")

#         # Main Frame
#         main_frame = ttk.Frame(self.root, padding=10)
#         main_frame.pack(fill="both", expand=True)

#         # Title
#         ttk.Label(main_frame, text="Motor Control Panel", font=("Arial", 16, "bold")).pack(pady=10)

#         # Motor Selection
#         motor_frame = ttk.LabelFrame(main_frame, text="Select Motor:", padding=10)
#         motor_frame.pack(fill="x", pady=5)
#         self.motor_var = tk.StringVar(value="X")
#         for motor in ["X", "Y", "Z", "A", "ALL"]:
#             ttk.Radiobutton(motor_frame, text=motor, variable=self.motor_var, value=motor).pack(side="left", padx=5)

#         # Speed Setting
#         self.speed_var = tk.IntVar(value=500)
#         self.create_input_row(main_frame, "Set Speed (1-1000):", self.speed_var)

#         # Rotation Time Setting
#         self.rotation_time_var = tk.IntVar(value=10)
#         self.create_input_row(main_frame, "Rotation Time (seconds):", self.rotation_time_var)

#         # Repeat Interval Setting
#         self.repeat_var = tk.IntVar(value=10)
#         self.create_input_row(main_frame, "Repeat Interval (seconds):", self.repeat_var)

#         # Direction Setting
#         direction_frame = ttk.LabelFrame(main_frame, text="Direction:", padding=10)
#         direction_frame.pack(fill="x", pady=5)
#         self.direction_var = tk.StringVar(value="FORWARD")
#         ttk.Radiobutton(direction_frame, text="FORWARD", variable=self.direction_var, value="FORWARD").pack(side="left", padx=5)
#         ttk.Radiobutton(direction_frame, text="BACKWARD", variable=self.direction_var, value="BACKWARD").pack(side="left", padx=5)

#         # Buttons
#         button_frame = ttk.Frame(main_frame, padding=10)
#         button_frame.pack(fill="x", pady=10)
#         ttk.Button(button_frame, text="Set Parameters", command=self.set_parameters).pack(side="left", padx=5)
#         ttk.Button(button_frame, text="Start Motor", command=self.start_motor).pack(side="left", padx=5)
#         ttk.Button(button_frame, text="Stop Motor", command=self.stop_motor).pack(side="left", padx=5)
#         ttk.Button(button_frame, text="Start All Motors", command=lambda: send_command("START ALL")).pack(side="left", padx=5)
#         ttk.Button(button_frame, text="Stop All Motors", command=lambda: send_command("STOP ALL")).pack(side="left", padx=5)
#         ttk.Button(button_frame, text="Exit", command=self.root.destroy).pack(side="right", padx=5)

#         # Console Output (Log)
#         self.log_frame = tk.Text(main_frame, height=10, state="disabled", wrap="word")
#         self.log_frame.pack(fill="both", pady=10)

#         # Start Serial Reading Thread
#         self.serial_thread = threading.Thread(target=self.read_serial, daemon=True)
#         self.serial_thread.start()

#     def create_input_row(self, parent, label_text, variable):
#         row = ttk.Frame(parent, padding=5)
#         row.pack(fill="x", pady=5)
#         ttk.Label(row, text=label_text).pack(side="left", padx=5)
#         ttk.Entry(row, textvariable=variable, width=10).pack(side="left", padx=5)

#     def set_parameters(self):
#         motor = self.motor_var.get()
#         speed = self.speed_var.get()
#         rotation_time = self.rotation_time_var.get()
#         repeat_interval = self.repeat_var.get()
#         direction = self.direction_var.get()

#         # Send commands to Arduino
#         send_command(f"SPEED {motor} {speed}")
#         send_command(f"TIME {motor} {rotation_time}")
#         send_command(f"REPEAT {motor} {repeat_interval}")
#         send_command(f"DIRECTION {motor} {direction}")
#         self.log_message(f"Parameters set for Motor {motor}: Speed={speed}, Time={rotation_time}, Repeat={repeat_interval}, Direction={direction}")

#     def start_motor(self):
#         motor = self.motor_var.get()
#         send_command(f"START {motor}")
#         self.log_message(f"Started Motor {motor}")

#     def stop_motor(self):
#         motor = self.motor_var.get()
#         send_command(f"STOP {motor}")
#         self.log_message(f"Stopped Motor {motor}")

#     def log_message(self, message):
#         self.log_frame.config(state="normal")
#         self.log_frame.insert("end", message + "\n")
#         self.log_frame.config(state="disabled")
#         self.log_frame.see("end")

#     def read_serial(self):
#         while True:
#             if ser and ser.in_waiting > 0:
#                 line = ser.readline().decode('utf-8').strip()
#                 self.log_message(f"Arduino: {line}")

# # Run the Application
# if __name__ == "__main__":
#     root = tk.Tk()
#     app = MotorControllerApp(root)
#     root.mainloop()
