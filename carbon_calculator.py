import serial
import time

arduino = serial.Serial('/dev/cu.usbmodem14101', 9600)

time.sleep(2)

print("Carbon Footprint Calculator started!")

while True:

    if arduino.in_waiting:

        message = arduino.readline().decode().strip()

        if message.startswith("LIGHT:"):

            light_value = int(message.split(":")[1])

            print("Light sensor reading:", light_value)

            # ----------------
            # TEST calculation
            # ----------------
            footprint = light_value / 500
            # prototype sensor-to-CO₂ mapping

            # Determine level
            if footprint < 1.0:
                state = 0
                level = "LOW"

            elif footprint <= 2.0:
                state = 1
                level = "MEDIUM"

            else:
                state = 2
                level = "HIGH"

            print(f"Footprint: {footprint:.2f} kg CO2")
            print("Level:", level)

            # Send result back to Arduino
            result = f"RESULT:{footprint:.2f},{state}\n"

            arduino.write(result.encode())