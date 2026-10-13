



import csv
import time
from datetime import datetime
from pathlib import Path

import serial

# Change this if your Arduino uses another port.
SERIAL_PORT = "/dev/cu.usbmodem14101"
BAUD_RATE = 9600

# Initial thresholds based on your earlier sensor readings.
# Calibrate these using repeatable measurements.
LOW_MAX = 300
MEDIUM_MAX = 700

CSV_FILE = Path("carbon_tracker_log.csv")


def classify_reading(light_value):
    """Return a state and a prototype index, not actual CO2 emissions."""
    demo_index = light_value / 500.0

    if light_value < LOW_MAX:
        state = 0
        level = "LOW"
    elif light_value < MEDIUM_MAX:
        state = 1
        level = "MEDIUM"
    else:
        state = 2
        level = "HIGH"

    return state, level, demo_index


def initialize_csv():
    """Create the CSV and its header if it doesn't exist."""
    if not CSV_FILE.exists():
        with CSV_FILE.open("w", newline="") as file:
            writer = csv.writer(file)
            writer.writerow([
                "timestamp",
                "light_raw",
                "demo_index",
                "state",
                "level",
            ])


def log_reading(light_value, demo_index, state, level):
    """Append one sensor measurement to the CSV."""
    with CSV_FILE.open("a", newline="") as file:
        writer = csv.writer(file)
        writer.writerow([
            datetime.now().isoformat(timespec="seconds"),
            light_value,
            f"{demo_index:.2f}",
            state,
            level,
        ])


def main():
    initialize_csv()

    # The timeout lets Python continue to handle keyboard interrupts.
    with serial.Serial(
        SERIAL_PORT,
        BAUD_RATE,
        timeout=1,
    ) as arduino:

        # Opening a serial connection may reset an Arduino.
        time.sleep(2)
        arduino.reset_input_buffer()

        print("Carbon Tracker monitoring started.")
        print(f"Logging data to: {CSV_FILE.resolve()}")

        latest_reading = None

        try:
            while True:
                message = (
                    arduino.readline()
                    .decode("utf-8", errors="replace")
                    .strip()
                )

                if not message:
                    continue

                if message == "READY":
                    print("Arduino is ready.")
                    continue

                if message == "NEXT":
                    if latest_reading is None:
                        print("Waiting for the first sensor reading.")
                    else:
                        raw, index, state, level = latest_reading
                        print(
                            f"Current reading: {raw}, "
                            f"index: {index:.2f}, level: {level}"
                        )
                    continue

                if not message.startswith("LIGHT:"):
                    print(f"Ignoring unknown message: {message}")
                    continue

                try:
                    light_value = int(message.split(":", 1)[1])

                    if not 0 <= light_value <= 1023:
                        raise ValueError("Reading outside ADC range")

                except ValueError:
                    print(f"Invalid sensor message: {message}")
                    continue

                state, level, demo_index = classify_reading(light_value)

                latest_reading = (
                    light_value,
                    demo_index,
                    state,
                    level,
                )

                # Protocol: RESULT:<state>:<demo_index>
                response = f"RESULT:{state}:{demo_index:.2f}\n"
                arduino.write(response.encode("utf-8"))

                log_reading(light_value, demo_index, state, level)

                print(
                    f"Light: {light_value:4d} | "
                    f"Demo index: {demo_index:.2f} | "
                    f"Level: {level}"
                )

        except KeyboardInterrupt:
            print("\nMonitoring stopped.")

        finally:
            print("Serial connection closed.")


if __name__ == "__main__":
    main()
