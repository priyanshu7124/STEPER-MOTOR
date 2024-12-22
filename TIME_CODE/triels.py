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


# ##################################################################################################################
# import tkinter as tk
# from tkinter import ttk
# import serial
# import time

# class MotorControlGUI:
#     def __init__(self, root):
#         self.root = root
#         self.root.title("Motor Control GUI")
        
#         # Serial setup
#         self.serial_port = serial.Serial('COM7', 9600, timeout=1)  # Adjust 'COM3' to your port
#         time.sleep(2)  # Give time to establish connection

#         # Motor control panels for each axis
#         self.create_motor_control_panel("X")
#         self.create_motor_control_panel("Y")
#         self.create_motor_control_panel("Z")
#         self.create_motor_control_panel("A")

#     def create_motor_control_panel(self, axis):
#         frame = ttk.LabelFrame(self.root, text=f"Motor {axis} Controls", padding=(10, 10))
#         frame.pack(padx=10, pady=10, fill="x")

#         # Speed Control
#         ttk.Label(frame, text="Set Speed:").grid(row=0, column=0, padx=5, pady=5)
#         speed_entry = ttk.Entry(frame)
#         speed_entry.grid(row=0, column=1, padx=5, pady=5)
#         ttk.Button(frame, text="Set Speed", 
#                    command=lambda: self.send_command(f"SPEED {axis} {speed_entry.get()}")).grid(row=0, column=2, padx=5, pady=5)

#         # Acceleration Control
#         ttk.Label(frame, text="Set Acceleration:").grid(row=1, column=0, padx=5, pady=5)
#         accel_entry = ttk.Entry(frame)
#         accel_entry.grid(row=1, column=1, padx=5, pady=5)
#         ttk.Button(frame, text="Set Acceleration", 
#                    command=lambda: self.send_command(f"ACCEL {axis} {accel_entry.get()}")).grid(row=1, column=2, padx=5, pady=5)

#         # Time Control
#         ttk.Label(frame, text="Rotation Time (sec):").grid(row=2, column=0, padx=5, pady=5)
#         time_entry = ttk.Entry(frame)
#         time_entry.grid(row=2, column=1, padx=5, pady=5)
#         ttk.Button(frame, text="Set Time", 
#                    command=lambda: self.send_command(f"TIME {axis} {time_entry.get()}")).grid(row=2, column=2, padx=5, pady=5)

#         # Start/Stop Buttons
#         ttk.Button(frame, text="Start", command=lambda: self.send_command(f"START {axis}")).grid(row=3, column=0, padx=5, pady=5)
#         ttk.Button(frame, text="Stop", command=lambda: self.send_command(f"STOP {axis}")).grid(row=3, column=1, padx=5, pady=5)

#     def send_command(self, command):
#         self.serial_port.write(f"{command}\n".encode())
#         print(f"Sent command: {command}")

#     def close_serial(self):
#         self.serial_port.close()

# if __name__ == "__main__":
#     root = tk.Tk()
#     app = MotorControlGUI(root)
#     root.protocol("WM_DELETE_WINDOW", lambda: (app.close_serial(), root.destroy()))
#     root.mainloop()

##########################################################################################################333
# import tkinter as tk
# from tkinter import ttk
# import serial
# import time

# class MotorControlGUI:
#     def __init__(self, root):
#         self.root = root
#         self.root.title("Stepper Motor Control GUI")
        
#         # Serial setup
#         self.serial_port = serial.Serial('COM7', 9600, timeout=1)  # Adjust 'COM7' to your specific port
#         time.sleep(2)  # Wait to establish connection with the Arduino
        
#         # Create motor control panels for each axis
#         self.create_motor_control_panel("X")
#         self.create_motor_control_panel("Y")
#         self.create_motor_control_panel("Z")
#         self.create_motor_control_panel("A")

#     def create_motor_control_panel(self, axis):
#         """Create a motor control panel for a specific axis"""
#         frame = ttk.LabelFrame(self.root, text=f"Motor {axis} Controls", padding=(10, 10))
#         frame.pack(padx=10, pady=10, fill="x")

#         # Speed Control
#         ttk.Label(frame, text="Set Speed:").grid(row=0, column=0, padx=5, pady=5)
#         speed_entry = ttk.Entry(frame)
#         speed_entry.grid(row=0, column=1, padx=5, pady=5)
#         ttk.Button(frame, text="Set Speed", 
#                    command=lambda: self.send_command(f"SPEED {axis} {speed_entry.get()}")).grid(row=0, column=2, padx=5, pady=5)

#         # Acceleration Control
#         ttk.Label(frame, text="Set Acceleration:").grid(row=1, column=0, padx=5, pady=5)
#         accel_entry = ttk.Entry(frame)
#         accel_entry.grid(row=1, column=1, padx=5, pady=5)
#         ttk.Button(frame, text="Set Acceleration", 
#                    command=lambda: self.send_command(f"ACCEL {axis} {accel_entry.get()}")).grid(row=1, column=2, padx=5, pady=5)

#         # Rotation Time Control
#         ttk.Label(frame, text="Rotation Time (sec):").grid(row=2, column=0, padx=5, pady=5)
#         time_entry = ttk.Entry(frame)
#         time_entry.grid(row=2, column=1, padx=5, pady=5)
#         ttk.Button(frame, text="Set Time", 
#                    command=lambda: self.send_command(f"TIME {axis} {time_entry.get()}")).grid(row=2, column=2, padx=5, pady=5)

#         # Start/Stop Buttons
#         ttk.Button(frame, text="Start", command=lambda: self.send_command(f"START {axis}")).grid(row=3, column=0, padx=5, pady=5)
#         ttk.Button(frame, text="Stop", command=lambda: self.send_command(f"STOP {axis}")).grid(row=3, column=1, padx=5, pady=5)

#     def send_command(self, command):
#         """Send a command to the serial port"""
#         self.serial_port.write(f"{command}\n".encode())
#         print(f"Sent command: {command}")

#     def close_serial(self):
#         """Close the serial connection"""
#         self.serial_port.close()

# if __name__ == "__main__":
#     root = tk.Tk()
#     app = MotorControlGUI(root)
#     root.protocol("WM_DELETE_WINDOW", lambda: (app.close_serial(), root.destroy()))
#     root.mainloop()
