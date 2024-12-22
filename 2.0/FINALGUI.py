import tkinter as tk
from tkinter import ttk, messagebox
import serial
import time

# Serial communication setup
SERIAL_PORT = "COM7"  # Change to your Arduino's port
BAUD_RATE = 9600

try:
    arduino = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except Exception as e:
    messagebox.showerror("Error", f"Unable to connect to Arduino: {e}")
    arduino = None


def send_command(command):
    """Send a command to the Arduino."""
    if arduino and arduino.is_open:
        arduino.write((command + "\n").encode("utf-8"))
        time.sleep(0.1)
        return arduino.readline().decode("utf-8").strip()
    else:
        return "Arduino not connected."


class MotorControlApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Motor Controller")
        self.root.geometry("600x600")
        self.create_widgets()

    def create_widgets(self):
        # Notebook for tabs
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill=tk.BOTH, expand=True)

        # Tabs for each motor
        self.motor_tabs = {}
        for motor in ['X', 'Y', 'Z', 'A']:
            tab = ttk.Frame(self.notebook)
            self.notebook.add(tab, text=f"Motor {motor}")
            self.motor_tabs[motor] = tab
            self.create_motor_tab(tab, motor)

        # All Motors tab
        self.all_motors_tab = ttk.Frame(self.notebook)
        self.notebook.add(self.all_motors_tab, text="All Motors")
        self.create_all_motors_tab(self.all_motors_tab)

        # Log area
        self.log_area = tk.Text(self.root, height=8, bg="lightgray", state="disabled")
        self.log_area.pack(fill=tk.BOTH, padx=10, pady=10)

    def create_motor_tab(self, tab, motor):
        """Create widgets for controlling an individual motor."""
        row = 0
        tk.Label(tab, text=f"Control Motor {motor}", font=("Arial", 14, "bold")).grid(row=row, column=0, columnspan=3, pady=10)

        row += 1
        tk.Label(tab, text="Speed (1-1000):").grid(row=row, column=0, sticky="w", padx=10)
        speed_entry = ttk.Entry(tab)
        speed_entry.grid(row=row, column=1)

        row += 1
        tk.Label(tab, text="Rotation Time (H:M:S):").grid(row=row, column=0, sticky="w", padx=10)
        time_entry = ttk.Entry(tab)
        time_entry.grid(row=row, column=1)

        row += 1
        tk.Label(tab, text="Repetition Time (H:M:S):").grid(row=row, column=0, sticky="w", padx=10)
        repeat_time_entry = ttk.Entry(tab)
        repeat_time_entry.grid(row=row, column=1)

        row += 1
        tk.Label(tab, text="Repetitions:").grid(row=row, column=0, sticky="w", padx=10)
        repetitions_entry = ttk.Entry(tab)
        repetitions_entry.grid(row=row, column=1)

        row += 1
        tk.Label(tab, text="Direction:").grid(row=row, column=0, sticky="w", padx=10)
        direction_var = tk.StringVar(value="FORWARD")
        direction_menu = ttk.Combobox(tab, textvariable=direction_var, values=["FORWARD", "BACKWARD"], state="readonly")
        direction_menu.grid(row=row, column=1)

        # Set and control buttons
        row += 1
        ttk.Button(tab, text="Set", command=lambda: self.set_motor(motor, speed_entry, time_entry, repeat_time_entry, repetitions_entry, direction_var)).grid(row=row, column=0, pady=10)
        ttk.Button(tab, text="Start", command=lambda: self.start_motor(motor)).grid(row=row, column=1, pady=10)
        ttk.Button(tab, text="Stop", command=lambda: self.stop_motor(motor)).grid(row=row, column=2, pady=10)

    def create_all_motors_tab(self, tab):
        """Create widgets for controlling all motors simultaneously."""
        row = 0
        tk.Label(tab, text="Control All Motors", font=("Arial", 14, "bold")).grid(row=row, column=0, columnspan=3, pady=10)

        row += 1
        tk.Label(tab, text="Speed (1-1000):").grid(row=row, column=0, sticky="w", padx=10)
        speed_entry = ttk.Entry(tab)
        speed_entry.grid(row=row, column=1)

        row += 1
        tk.Label(tab, text="Rotation Time (H:M:S):").grid(row=row, column=0, sticky="w", padx=10)
        time_entry = ttk.Entry(tab)
        time_entry.grid(row=row, column=1)

        row += 1
        tk.Label(tab, text="Repetition Time (H:M:S):").grid(row=row, column=0, sticky="w", padx=10)
        repeat_time_entry = ttk.Entry(tab)
        repeat_time_entry.grid(row=row, column=1)

        row += 1
        tk.Label(tab, text="Repetitions:").grid(row=row, column=0, sticky="w", padx=10)
        repetitions_entry = ttk.Entry(tab)
        repetitions_entry.grid(row=row, column=1)

        row += 1
        tk.Label(tab, text="Direction:").grid(row=row, column=0, sticky="w", padx=10)
        direction_var = tk.StringVar(value="FORWARD")
        direction_menu = ttk.Combobox(tab, textvariable=direction_var, values=["FORWARD", "BACKWARD"], state="readonly")
        direction_menu.grid(row=row, column=1)

        # Set and control buttons
        row += 1
        ttk.Button(tab, text="Set All", command=lambda: self.set_all_motors(speed_entry, time_entry, repeat_time_entry, repetitions_entry, direction_var)).grid(row=row, column=0, pady=10)
        ttk.Button(tab, text="Start All", command=self.start_all_motors).grid(row=row, column=1, pady=10)
        ttk.Button(tab, text="Stop All", command=self.stop_all_motors).grid(row=row, column=2, pady=10)

    def log_message(self, message):
        """Add a message to the log."""
        self.log_area.config(state="normal")
        self.log_area.insert(tk.END, message + "\n")
        self.log_area.see(tk.END)
        self.log_area.config(state="disabled")

    def set_motor(self, motor, speed_entry, time_entry, repeat_time_entry, repetitions_entry, direction_var):
        """Set motor parameters."""
        speed = speed_entry.get()
        rotation_time = time_entry.get()
        repeat_time = repeat_time_entry.get()
        repetitions = repetitions_entry.get()
        direction = direction_var.get()
        if speed and rotation_time and repeat_time and repetitions:
            send_command(f"SET {motor} {speed} {rotation_time} {repeat_time} {repetitions} {direction}")
            self.log_message(f"Motor {motor} set: Speed={speed}, Time={rotation_time}, Repeats={repeat_time}x{repetitions}, Direction={direction}")
        else:
            messagebox.showerror("Input Error", "All fields are required to set motor parameters.")

    def start_motor(self, motor):
        """Start the specified motor."""
        send_command(f"START {motor}")
        self.log_message(f"Motor {motor} started.")

    def stop_motor(self, motor):
        """Stop the specified motor."""
        send_command(f"STOP {motor}")
        self.log_message(f"Motor {motor} stopped.")

    def set_all_motors(self, speed_entry, time_entry, repeat_time_entry, repetitions_entry, direction_var):
        """Set parameters for all motors."""
        speed = speed_entry.get()
        rotation_time = time_entry.get()
        repeat_time = repeat_time_entry.get()
        repetitions = repetitions_entry.get()
        direction = direction_var.get()
        if speed and rotation_time and repeat_time and repetitions:
            send_command(f"SET ALL {speed} {rotation_time} {repeat_time} {repetitions} {direction}")
            self.log_message(f"All motors set: Speed={speed}, Time={rotation_time}, Repeats={repeat_time}x{repetitions}, Direction={direction}")
        else:
            messagebox.showerror("Input Error", "All fields are required to set parameters for all motors.")

    def start_all_motors(self):
        """Start all motors."""
        send_command("START ALL")
        self.log_message("All motors started.")

    def stop_all_motors(self):
        """Stop all motors."""
        send_command("STOP ALL")
        self.log_message("All motors stopped.")


if __name__ == "__main__":
    root = tk.Tk()
    app = MotorControlApp(root)
    root.mainloop()
