import tkinter as tk
from tkinter import messagebox
import serial

# Serial Communication Setup
SERIAL_PORT = 'COM7'  # Change this as per your configuration
BAUD_RATE = 9600

# Initialize Serial Communication
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except Exception as e:
    ser = None
    print(f"Serial connection error: {e}")

# Send command to Arduino
def send_command(command):
    if ser and ser.is_open:
        ser.write((command + '\n').encode('utf-8'))
    else:
        messagebox.showerror("Connection Error", "Serial connection is not available.")

# GUI Application
class MotorControlGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Motor Control GUI")
        self.root.geometry("600x600")

        # Title
        tk.Label(root, text="Motor Control Panel", font=("Arial", 16, "bold")).pack(pady=10)

        # Motor Selection
        tk.Label(root, text="Select Motor:").pack()
        self.motor_var = tk.StringVar(value="X")
        motor_frame = tk.Frame(root)
        motor_frame.pack()
        for motor in ['X', 'Y', 'Z', 'A', 'ALL']:
            tk.Radiobutton(motor_frame, text=motor, variable=self.motor_var, value=motor).pack(side=tk.LEFT, padx=10)

        # Speed Input
        tk.Label(root, text="Set Speed (1-1000):").pack(pady=5)
        self.speed_var = tk.StringVar(value="500")
        tk.Entry(root, textvariable=self.speed_var, width=10).pack()

        # Rotation Time Input
        tk.Label(root, text="Rotation Time (seconds):").pack(pady=5)
        self.rotation_time_var = tk.StringVar(value="10")
        tk.Entry(root, textvariable=self.rotation_time_var, width=10).pack()

        # Repeat Interval Input
        tk.Label(root, text="Repeat Interval (seconds):").pack(pady=5)
        self.repeat_interval_var = tk.StringVar(value="10")
        tk.Entry(root, textvariable=self.repeat_interval_var, width=10).pack()

        # Direction Control
        tk.Label(root, text="Direction:").pack(pady=5)
        self.direction_var = tk.StringVar(value="FORWARD")
        direction_frame = tk.Frame(root)
        direction_frame.pack()
        tk.Radiobutton(direction_frame, text="FORWARD", variable=self.direction_var, value="FORWARD").pack(side=tk.LEFT, padx=10)
        tk.Radiobutton(direction_frame, text="BACKWARD", variable=self.direction_var, value="BACKWARD").pack(side=tk.LEFT, padx=10)

        # Set and Start Buttons
        button_frame = tk.Frame(root)
        button_frame.pack(pady=20)

        tk.Button(button_frame, text="Set Parameters", command=self.set_parameters).pack(side=tk.LEFT, padx=10)
        tk.Button(button_frame, text="Start Motor", command=self.start_motor).pack(side=tk.LEFT, padx=10)
        tk.Button(button_frame, text="Stop Motor", command=self.stop_motor).pack(side=tk.LEFT, padx=10)
        tk.Button(button_frame, text="Start All Motors", command=self.start_all_motors).pack(side=tk.LEFT, padx=10)
        tk.Button(button_frame, text="Stop All Motors", command=self.stop_all_motors).pack(side=tk.LEFT, padx=10)
        tk.Button(button_frame, text="Exit", command=self.exit_app).pack(side=tk.LEFT, padx=10)

        # Log Display
        self.log_text = tk.Text(root, height=10, width=70, state=tk.DISABLED)
        self.log_text.pack(pady=10)

    def log(self, message):
        self.log_text.config(state=tk.NORMAL)
        self.log_text.insert(tk.END, message + '\n')
        self.log_text.config(state=tk.DISABLED)
        self.log_text.see(tk.END)

    def set_parameters(self):
        motor = self.motor_var.get()
        speed = self.speed_var.get()
        rotation_time = self.rotation_time_var.get()
        repeat_interval = self.repeat_interval_var.get()
        direction = self.direction_var.get()

        try:
            speed = int(speed)
            rotation_time = int(rotation_time)
            repeat_interval = int(repeat_interval)
            if not (1 <= speed <= 1000):
                raise ValueError("Speed out of range.")
            if rotation_time <= 0 or repeat_interval <= 0:
                raise ValueError("Time values must be positive.")
        except ValueError as e:
            messagebox.showerror("Input Error", f"Invalid input: {e}")
            return

        # Send parameter commands to Arduino
        send_command(f"SPEED {motor} {speed}")
        send_command(f"TIME {motor} {rotation_time}")
        send_command(f"REPEAT {motor} {repeat_interval}")
        send_command(f"DIRECTION {motor} {direction}")

        # Log actions
        self.log(f"Motor {motor}: Parameters set (Speed={speed}, Rotation Time={rotation_time}s, Repeat Interval={repeat_interval}s, Direction={direction})")

    def start_motor(self):
        motor = self.motor_var.get()
        send_command(f"START {motor}")
        self.log(f"Motor {motor}: Started.")

    def stop_motor(self):
        motor = self.motor_var.get()
        send_command(f"STOP {motor}")
        self.log(f"Motor {motor}: Stopped.")

    def start_all_motors(self):
        send_command("START ALL")
        self.log("All motors: Started.")

    def stop_all_motors(self):
        send_command("STOP ALL")
        self.log("All motors: Stopped.")

    def exit_app(self):
        if ser:
            ser.close()
        self.root.destroy()

# Start the GUI application
if __name__ == "__main__":
    root = tk.Tk()
    app = MotorControlGUI(root)
    root.mainloop()