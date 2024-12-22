
import tkinter as tk
from tkinter import ttk
import serial
import threading

# Set up serial communication
try:
    ser = serial.Serial('COM6', 9600, timeout=1)  # Replace 'COM6' with your correct port
except Exception as e:
    ser = None
    print(f"Serial Error: {e}")

# Send a command to the Arduino
def send_command(command):
    if ser and ser.is_open:
        ser.write((command + "\n").encode('utf-8'))
    else:
        print("Serial communication is not active.")

# GUI Class
class MotorControllerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Motor Control Panel")
        self.root.geometry("600x400")

        # Main Frame
        main_frame = ttk.Frame(self.root, padding=10)
        main_frame.pack(fill="both", expand=True)

        # Title
        ttk.Label(main_frame, text="Motor Control Panel", font=("Arial", 16, "bold")).pack(pady=10)

        # Motor Selection
        motor_frame = ttk.LabelFrame(main_frame, text="Select Motor:", padding=10)
        motor_frame.pack(fill="x", pady=5)
        self.motor_var = tk.StringVar(value="X")
        for motor in ["X", "Y", "Z", "A", "ALL"]:
            ttk.Radiobutton(motor_frame, text=motor, variable=self.motor_var, value=motor).pack(side="left", padx=5)

        # Speed Setting
        self.speed_var = tk.IntVar(value=500)
        self.create_input_row(main_frame, "Set Speed (1-1000):", self.speed_var)

        # Rotation Time Setting
        self.rotation_time_var = tk.IntVar(value=10)
        self.create_input_row(main_frame, "Rotation Time (seconds):", self.rotation_time_var)

        # Repeat Interval Setting
        self.repeat_var = tk.IntVar(value=10)
        self.create_input_row(main_frame, "Repeat Interval (seconds):", self.repeat_var)

        # Direction Setting
        direction_frame = ttk.LabelFrame(main_frame, text="Direction:", padding=10)
        direction_frame.pack(fill="x", pady=5)
        self.direction_var = tk.StringVar(value="FORWARD")
        ttk.Radiobutton(direction_frame, text="FORWARD", variable=self.direction_var, value="FORWARD").pack(side="left", padx=5)
        ttk.Radiobutton(direction_frame, text="BACKWARD", variable=self.direction_var, value="BACKWARD").pack(side="left", padx=5)

        # Buttons
        button_frame = ttk.Frame(main_frame, padding=10)
        button_frame.pack(fill="x", pady=10)
        ttk.Button(button_frame, text="Set Parameters", command=self.set_parameters).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Start Motor", command=self.start_motor).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Stop Motor", command=self.stop_motor).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Start All Motors", command=lambda: send_command("START ALL")).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Stop All Motors", command=lambda: send_command("STOP ALL")).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Exit", command=self.root.destroy).pack(side="right", padx=5)

        # Console Output (Log)
        self.log_frame = tk.Text(main_frame, height=10, state="disabled", wrap="word")
        self.log_frame.pack(fill="both", pady=10)

        # Start Serial Reading Thread
        self.serial_thread = threading.Thread(target=self.read_serial, daemon=True)
        self.serial_thread.start()

    def create_input_row(self, parent, label_text, variable):
        row = ttk.Frame(parent, padding=5)
        row.pack(fill="x", pady=5)
        ttk.Label(row, text=label_text).pack(side="left", padx=5)
        ttk.Entry(row, textvariable=variable, width=10).pack(side="left", padx=5)

    def set_parameters(self):
        motor = self.motor_var.get()
        speed = self.speed_var.get()
        rotation_time = self.rotation_time_var.get()
        repeat_interval = self.repeat_var.get()
        direction = self.direction_var.get()

        # Send commands to Arduino
        send_command(f"SPEED {motor} {speed}")
        send_command(f"TIME {motor} {rotation_time}")
        send_command(f"REPEAT {motor} {repeat_interval}")
        send_command(f"DIRECTION {motor} {direction}")
        self.log_message(f"Parameters set for Motor {motor}: Speed={speed}, Time={rotation_time}, Repeat={repeat_interval}, Direction={direction}")

    def start_motor(self):
        motor = self.motor_var.get()
        send_command(f"START {motor}")
        self.log_message(f"Started Motor {motor}")

    def stop_motor(self):
        motor = self.motor_var.get()
        send_command(f"STOP {motor}")
        self.log_message(f"Stopped Motor {motor}")

    def log_message(self, message):
        self.log_frame.config(state="normal")
        self.log_frame.insert("end", message + "\n")
        self.log_frame.config(state="disabled")
        self.log_frame.see("end")

    def read_serial(self):
        while True:
            if ser and ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                self.log_message(f"Arduino: {line}")

# Run the Application
if __name__ == "__main__":
    root = tk.Tk()
    app = MotorControllerApp(root)
    root.mainloop()
