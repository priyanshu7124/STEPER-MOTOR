import tkinter as tk
from tkinter import ttk
import serial
import threading

# Set up serial communication
ser = serial.Serial('COM6', 9600, timeout=1)  # Replace 'COM3' with the correct port for your Arduino

# Send a command to the Arduino
def send_command(command):
    ser.write((command + "\n").encode('utf-8'))

# GUI class
class MotorControllerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("4-Axis Motor Controller")

        # Create tabs for each motor
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill="both", expand=True)

        # Add motor control tabs
        for motor in ['X', 'Y', 'Z', 'A']:
            self.add_motor_tab(motor)

        # Thread to read serial output from Arduino
        self.serial_thread = threading.Thread(target=self.read_serial, daemon=True)
        self.serial_thread.start()

    def add_motor_tab(self, motor):
        tab = ttk.Frame(self.notebook)
        self.notebook.add(tab, text=f"Motor {motor}")

        # Motor controls
        ttk.Label(tab, text=f"Motor {motor} Controls").pack(pady=10)

        # Start and Stop buttons
        button_frame = ttk.Frame(tab)
        button_frame.pack(pady=10)
        ttk.Button(button_frame, text="Start", command=lambda: send_command(f"START {motor}")).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Stop", command=lambda: send_command(f"STOP {motor}")).pack(side="left", padx=5)

        # Speed control
        speed_frame = ttk.Frame(tab)
        speed_frame.pack(pady=10)
        ttk.Label(speed_frame, text="Speed (1-1000):").pack(side="left", padx=5)
        speed_var = tk.IntVar(value=500)
        speed_entry = ttk.Entry(speed_frame, textvariable=speed_var, width=10)
        speed_entry.pack(side="left", padx=5)
        ttk.Button(speed_frame, text="Set Speed", command=lambda: send_command(f"SPEED {motor} {speed_var.get()}")).pack(side="left", padx=5)

        # Rotation time control
        time_frame = ttk.Frame(tab)
        time_frame.pack(pady=10)
        ttk.Label(time_frame, text="Rotation Time (s):").pack(side="left", padx=5)
        time_var = tk.IntVar(value=10)
        time_entry = ttk.Entry(time_frame, textvariable=time_var, width=10)
        time_entry.pack(side="left", padx=5)
        ttk.Button(time_frame, text="Set Time", command=lambda: send_command(f"TIME {motor} {time_var.get()}")).pack(side="left", padx=5)

        # Repetition time control
        repeat_frame = ttk.Frame(tab)
        repeat_frame.pack(pady=10)
        ttk.Label(repeat_frame, text="Repetition Time (s):").pack(side="left", padx=5)
        repeat_var = tk.IntVar(value=5)
        repeat_entry = ttk.Entry(repeat_frame, textvariable=repeat_var, width=10)
        repeat_entry.pack(side="left", padx=5)
        ttk.Button(repeat_frame, text="Set Repeat", command=lambda: send_command(f"REPEAT {motor} {repeat_var.get()}")).pack(side="left", padx=5)

        # Direction control
        direction_frame = ttk.Frame(tab)
        direction_frame.pack(pady=10)
        ttk.Label(direction_frame, text="Direction:").pack(side="left", padx=5)
        ttk.Button(direction_frame, text="Forward", command=lambda: send_command(f"DIRECTION {motor} FORWARD")).pack(side="left", padx=5)
        ttk.Button(direction_frame, text="Backward", command=lambda: send_command(f"DIRECTION {motor} BACKWARD")).pack(side="left", padx=5)

    def read_serial(self):
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                print(f"Arduino: {line}")

# Run the GUI application
if __name__ == "__main__":
    root = tk.Tk()
    app = MotorControllerApp(root)
    root.mainloop()
