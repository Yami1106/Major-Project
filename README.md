# 🔥 FireGuard Rescue Bot 🔍

Welcome to the **FireGuard Rescue Bot** project – an AI-powered rescue solution designed to enhance fire detection, human safety, and real-time object monitoring. This project integrates a **custom-trained object detection model**, **fire prediction system**, **ESP32 communication module**, and a **Flask-based application**, all coordinated through **Raspberry Pi** and accessible remotely via **Ngrok**.

---

## 🚀 Project Description

The FireGuard Rescue Bot is designed to:

- Detect and predict the presence of **fire** in real-time using a trained neural network.
- Identify key objects like **Humans**, **Pens**, **Mobile Phones**, and **Fire** using a YOLO-based custom object detection model.
- Communicate sensor and image data through **ESP32** acting as a proxy server.
- Host a web-based app using **Flask** on Raspberry Pi for monitoring and control.
- Enable secure remote access via **Ngrok tunneling** for both development and deployment environments.

Our model has been **trained on a custom dataset** for the following object categories:

- **Human**
- **Fire**
- **Pen**
- **Mobile Phone**

---

## 📦 Project Structure

```
?? FireGuard Rescue Bot
+-- colourdetection/
¦   +-- colourdetection.ino              # Arduino code for color detection
¦
+-- server_esp32/
¦   +-- server_esp32.ino                 # ESP32 server code
¦
+-- firepredictor/
¦   +-- firepredictor.ino                # Arduino code for fire prediction
¦
+-- bot_code/
¦   +-- bot_code.ino                     # Main bot logic code for Arduino
¦
+-- esp32-proxy/
¦   +-- server.js                        # Node.js server for ESP32 proxy
¦   +-- package.json                     # Node package config
¦   +-- package-lock.json                # Lockfile for dependencies
¦   +-- node_modules/                    # Node.js dependencies
¦
+-- livestream/
¦   +-- app.py                           # Flask app for object detection
¦   +-- obj.names                        # Custom class labels
¦   +-- yolov4-tiny-custom.cfg           # YOLOv4-tiny config file
¦   +-- yolov4-tiny-custom_best.weights  # Trained weights for object detection
¦   +-- templates/
¦       +-- index.html                   # Web interface for the stream
```

---

## 🧪 1. Execution of the Fire Prediction Model

This is the model which is used by the microcontroller to predict fire, this file can be executed directly through the arduino IDE 

---

## 💻 2. How to Run the App on System for Testing

The bot which is controlled through an app can be executed by the following steps:
    - Open the command prompt in the respective folder path 
    - Enter the command `npx expo start --clear` , this command generates a QR code which can be scanned through the expo go mobile app to view the interface, or by typing `a` in the command prompt the interface can be viewed in android studio.

---

## 📡 3. How to Run the ESP32 Proxy
The commands from the app need to be received to the esp32 this is sent trough a proxy, it can be executed as follows
    - open the terminal in the folder path and execute the command node server.js
    - This command ensures that the command from the app is received securely by the esp32 

---

## 🍓 4. How to Run the Object Detection Model and Flask on Raspberry Pi
This model is implemented in the raspberry pi to help the bot recognize and confirm fires 
    - Open the terminal in the path where the app.py file is present 
    - Type in the command `python app.py` 

---

## 🌐 5. How to Run Ngrok in Raspberry Pi from Terminal

Here’s how to start a public URL for your Flask server using Ngrok on Raspberry Pi:

```bash
# Step 1: Unzip and authenticate (if not already)
./ngrok config add-authtoken YOUR_AUTHTOKEN

# Step 2: Start a tunnel for port 5000 (or your Flask port)
./ngrok http 5000

# Step 1: Navigate to the directory containing ngrok.exe
cd C:\path\to\ngrok

# Step 2: Add your authtoken (only once)
ngrok config add-authtoken YOUR_AUTHTOKEN

# Step 3: Start tunnel
ngrok http 5000


