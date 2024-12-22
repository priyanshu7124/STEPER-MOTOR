import tkinter as tk
from tkinter import messagebox
import serial
import time
import threading

# Configure serial port (update 'COM3' to your Arduino's port)
arduino_port = 'COM7'  # Replace with your port
baud_rate = 9600
ser = serial.Serial(arduino_port, baud_rate, timeout=1)
time.sleep(2)  # Wait for the connection to establish

class MotorControlGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Motor Control GUI")
        
        # Create frames for each motor axis
        self.create_motor_frame("X")
        self.create_motor_frame("Y")
        self.create_motor_frame("Z")
        self.create_motor_frame("A")

        # Common Control Frame
        self.create_common_control_frame()

        # Start a thread to monitor elapsed time updates
        self.monitor_thread = threading.Thread(target=self.monitor_elapsed_time)
        self.monitor_thread.daemon = True
        self.monitor_thread.start()

    def create_motor_frame(self, axis):
        frame = tk.Frame(self.root, padx=10, pady=10)
        frame.pack(pady=5)
        
        tk.Label(frame, text=f"Motor {axis} Control").grid(row=0, columnspan=3)
        
        # Rotation Time input
        tk.Label(frame, text="Rotation Time (s):").grid(row=1, column=0)
        rotation_time_entry = tk.Entry(frame, width=5)
        rotation_time_entry.grid(row=1, column=1)
        rotation_time_entry.insert(0, "10")  # Default time
        setattr(self, f"rotation_time_{axis}", rotation_time_entry)
        
        # Start and Stop buttons
        start_button = tk.Button(frame, text="Start", command=lambda a=axis: self.send_command(f"START {a}"))
        start_button.grid(row=1, column=2)
        
        stop_button = tk.Button(frame, text="Stop", command=lambda a=axis: self.send_command(f"STOP {a}"))
        stop_button.grid(row=1, column=3)
        
        # Elapsed time display
        elapsed_label = tk.Label(frame, text="Elapsed Time: 0s")
        elapsed_label.grid(row=2, columnspan=4)
        setattr(self, f"elapsed_label_{axis}", elapsed_label)
        
        # Set time button
        set_time_button = tk.Button(frame, text="Set Time", command=lambda a=axis: self.set_rotation_time(a))
        set_time_button.grid(row=1, column=4)

    def create_common_control_frame(self):
        frame = tk.Frame(self.root, padx=10, pady=10, bg="lightblue")
        frame.pack(pady=10)

        tk.Label(frame, text="Common Control for All Motors", bg="lightblue", font=("Arial", 12)).pack()

        # Common Rotation Time Input
        tk.Label(frame, text="Set Rotation Time (s):", bg="lightblue").pack(side=tk.LEFT, padx=5)
        self.common_rotation_time_entry = tk.Entry(frame, width=5)
        self.common_rotation_time_entry.pack(side=tk.LEFT, padx=5)
        self.common_rotation_time_entry.insert(0, "10")  # Default time

        # Common Start and Stop Buttons
        start_all_button = tk.Button(frame, text="Start All", command=self.start_all_motors)
        start_all_button.pack(side=tk.LEFT, padx=5)

        stop_all_button = tk.Button(frame, text="Stop All", command=self.stop_all_motors)
        stop_all_button.pack(side=tk.LEFT, padx=5)

    def send_command(self, command):
        try:
            ser.write((command + '\n').encode())
            time.sleep(0.1)
            response = ser.readline().decode().strip()
            if response:
                print(f"Arduino: {response}")
        except serial.SerialException:
            messagebox.showerror("Error", "Failed to communicate with Arduino.")
            self.root.quit()

    def set_rotation_time(self, axis):
        try:
            rotation_time = getattr(self, f"rotation_time_{axis}").get()
            if rotation_time.isdigit() and int(rotation_time) > 0:
                command = f"TIME {axis} {rotation_time}"
                self.send_command(command)
                messagebox.showinfo("Success", f"Rotation time for Motor {axis} set to {rotation_time} seconds.")
            else:
                messagebox.showerror("Invalid Input", "Please enter a positive integer for rotation time.")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to set time: {e}")

    def start_all_motors(self):
        common_time = self.common_rotation_time_entry.get()
        if common_time.isdigit() and int(common_time) > 0:
            # Set common time for each axis
            for axis in ['X', 'Y', 'Z', 'A']:
                self.send_command(f"TIME {axis} {common_time}")
            # Start all motors
            for axis in ['X', 'Y', 'Z', 'A']:
                self.send_command(f"START {axis}")
            messagebox.showinfo("Success", f"All motors started with rotation time of {common_time} seconds.")
        else:
            messagebox.showerror("Invalid Input", "Please enter a positive integer for common rotation time.")

    def stop_all_motors(self):
        # Stop all motors
        for axis in ['X', 'Y', 'Z', 'A']:
            self.send_command(f"STOP {axis}")
        messagebox.showinfo("Info", "All motors stopped.")

    def monitor_elapsed_time(self):
        while True:
            if ser.in_waiting > 0:
                try:
                    line = ser.readline().decode().strip()
                    if line.startswith("Elapsed Time for"):
                        parts = line.split(": ")
                        axis = parts[0][-1]
                        elapsed_time = parts[1].split(" ")[0]
                        label = getattr(self, f"elapsed_label_{axis}")
                        label.config(text=f"Elapsed Time: {elapsed_time}s")
                    elif "stopped after reaching set time" in line:
                        print(f"Arduino: {line}")
                except Exception as e:
                    print(f"Error reading serial: {e}")
            time.sleep(0.1)

# Initialize GUI
root = tk.Tk()
app = MotorControlGUI(root)
root.mainloop()          


# import tkinter as tk
# from tkinter import messagebox
# import serial
# import time
# import threading

# # Configure serial port (update 'COM7' to your Arduino's port)
# arduino_port = 'COM7'  # Replace with your port
# baud_rate = 9600
# ser = serial.Serial(arduino_port, baud_rate, timeout=1)
# time.sleep(2)  # Wait for the connection to establish


# class MotorControlGUI:
#     def __init__(self, root):
#         self.root = root
#         self.root.title("Motor Control GUI")
        
#         # Create frames for each motor axis
#         self.create_motor_frame("X")
#         self.create_motor_frame("Y")
#         self.create_motor_frame("Z")
#         self.create_motor_frame("A")

#         # Common Control Frame
#         self.create_common_control_frame()

#         # Start a thread to monitor elapsed time updates
#         self.monitor_thread = threading.Thread(target=self.monitor_elapsed_time)
#         self.monitor_thread.daemon = True
#         self.monitor_thread.start()

#     def create_motor_frame(self, axis):
#         frame = tk.Frame(self.root, padx=10, pady=10)
#         frame.pack(pady=5)
        
#         tk.Label(frame, text=f"Motor {axis} Control").grid(row=0, columnspan=3)
        
#         # Rotation Time input
#         tk.Label(frame, text="Rotation Time (s):").grid(row=1, column=0)
#         rotation_time_entry = tk.Entry(frame, width=5)
#         rotation_time_entry.grid(row=1, column=1)
#         rotation_time_entry.insert(0, "10")  # Default time
#         setattr(self, f"rotation_time_{axis}", rotation_time_entry)
        
#         # Start and Stop buttons
#         start_button = tk.Button(frame, text="Start", command=lambda a=axis: self.send_command(f"START {a}"))
#         start_button.grid(row=1, column=2)
        
#         stop_button = tk.Button(frame, text="Stop", command=lambda a=axis: self.send_command(f"STOP {a}"))
#         stop_button.grid(row=1, column=3)
        
#         # Elapsed time display
#         elapsed_label = tk.Label(frame, text="Elapsed Time: 0s")
#         elapsed_label.grid(row=2, columnspan=4)
#         setattr(self, f"elapsed_label_{axis}", elapsed_label)
        
#         # Set time button
#         set_time_button = tk.Button(frame, text="Set Time", command=lambda a=axis: self.set_rotation_time(a))
#         set_time_button.grid(row=1, column=4)

#     def create_common_control_frame(self):
#         frame = tk.Frame(self.root, padx=10, pady=10, bg="lightblue")
#         frame.pack(pady=10)

#         tk.Label(frame, text="Common Control for All Motors", bg="lightblue", font=("Arial", 12)).pack()

#         # Common Rotation Time Input
#         tk.Label(frame, text="Set Rotation Time (s):", bg="lightblue").pack(side=tk.LEFT, padx=5)
#         self.common_rotation_time_entry = tk.Entry(frame, width=5)
#         self.common_rotation_time_entry.pack(side=tk.LEFT, padx=5)
#         self.common_rotation_time_entry.insert(0, "10")  # Default time

#         # Common Start and Stop Buttons
#         start_all_button = tk.Button(frame, text="Start All", command=self.start_all_motors)
#         start_all_button.pack(side=tk.LEFT, padx=5)

#         stop_all_button = tk.Button(frame, text="Stop All", command=self.stop_all_motors)
#         stop_all_button.pack(side=tk.LEFT, padx=5)

#         # Elapsed time display for common control
#         self.common_elapsed_label = tk.Label(frame, text="Common Elapsed Time: 0s", bg="lightblue")
#         self.common_elapsed_label.pack(side=tk.LEFT, padx=5)

#     def send_command(self, command):
#         try:
#             ser.write((command + '\n').encode())
#             time.sleep(0.1)
#             response = ser.readline().decode().strip()
#             if response:
#                 print(f"Arduino: {response}")
#         except serial.SerialException:
#             messagebox.showerror("Error", "Failed to communicate with Arduino.")
#             self.root.quit()

#     def set_rotation_time(self, axis):
#         try:
#             rotation_time = getattr(self, f"rotation_time_{axis}").get()
#             if rotation_time.isdigit() and int(rotation_time) > 0:
#                 command = f"TIME {axis} {rotation_time}"
#                 self.send_command(command)
#                 messagebox.showinfo("Success", f"Rotation time for Motor {axis} set to {rotation_time} seconds.")
#             else:
#                 messagebox.showerror("Invalid Input", "Please enter a positive integer for rotation time.")
#         except Exception as e:
#             messagebox.showerror("Error", f"Failed to set time: {e}")

#     def start_all_motors(self):
#         common_time = self.common_rotation_time_entry.get()
#         if common_time.isdigit() and int(common_time) > 0:
#             # Set common time for each axis
#             for axis in ['X', 'Y', 'Z', 'A']:
#                 self.send_command(f"TIME {axis} {common_time}")
#             # Start all motors
#             self.send_command("START_ALL")
#             messagebox.showinfo("Success", f"All motors started with rotation time of {common_time} seconds.")
#         else:
#             messagebox.showerror("Invalid Input", "Please enter a positive integer for common rotation time.")

#     def stop_all_motors(self):
#         # Stop all motors
#         self.send_command("STOP_ALL")
#         messagebox.showinfo("Info", "All motors stopped.")

#     def monitor_elapsed_time(self):
#         while True:
#             if ser.in_waiting > 0:
#                 try:
#                     line = ser.readline().decode().strip()
#                     if line.startswith("Elapsed Time for"):
#                         parts = line.split(": ")
#                         axis = parts[0][-1]
#                         elapsed_time = parts[1].split(" ")[0]
#                         label = getattr(self, f"elapsed_label_{axis}")
#                         label.config(text=f"Elapsed Time: {elapsed_time}s")
#                     elif line.startswith("Common Elapsed Time"):
#                         elapsed_time = line.split(": ")[1]
#                         self.common_elapsed_label.config(text=f"Common Elapsed Time: {elapsed_time}s")
#                 except Exception as e:
#                     print(f"Error reading serial: {e}")
#             time.sleep(0.1)


# # Initialize GUI
# root = tk.Tk()
# app = MotorControlGUI(root)
# root.mainloop()
