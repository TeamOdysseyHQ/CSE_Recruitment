from flask import Flask, jsonify, send_file
import random
import time

app = Flask(__name__)

#if i was to implemet a real system, here's where i would connect to the rover hardware to get teh telemetry data.
def get_telemetry():
    return {
        "speed": round(random.uniform(0, 5), 2),
        "battery": random.randint(20, 100),
        "status": "CONNECTED"
    }

@app.route("/")
def home():
    return send_file("index.html")

@app.route("/telemetry")
def telemetry():
    return jsonify(get_telemetry())

@app.route("/start")
def start():
    return jsonify({"message": "Rover's started"})

@app.route("/stop")
def stop():
    return jsonify({"message": "Rover's stopped"})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)