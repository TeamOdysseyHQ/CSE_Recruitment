import time
import random

while True:
    speed = round(random.uniform(0, 5), 2)
    battery = random.randint(20, 100)

    log = f"[LOG] Speed: {speed} m/s | Battery: {battery}%"
    
    print(log)

    # optionally save to file
    with open("telemetry.log", "a") as f:
        f.write(log + "\n")

    time.sleep(2)
