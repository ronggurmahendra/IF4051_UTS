# import paho.mqtt.client as mqtt

# # The callback for when the client receives a CONNACK response from the server.
# def on_connect(client, userdata, flags, reason_code, properties):
#     print(f"Connected with result code {reason_code}")
#     # Subscribing in on_connect() means that if we lose the connection and
#     # reconnect then subscriptions will be renewed.
#     client.subscribe("$SYS/#")

# # The callback for when a PUBLISH message is received from the server.
# def on_message(client, userdata, msg):
#     print(msg.topic+" "+str(msg.payload))

# mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
# mqttc.on_connect = on_connect
# mqttc.on_message = on_message

# mqttc.connect("mqtt.eclipseprojects.io", 1883, 60)

# # Blocking call that processes network traffic, dispatches callbacks and
# # handles reconnecting.
# # Other loop*() functions are available that give a threaded interface and a
# # manual interface.
# mqttc.loop_forever()

import paho.mqtt.client as mqtt
import time

# MQTT broker settings
MQTT_BROKER_HOST = "localhost"
MQTT_BROKER_PORT = 1883

# MQTT topic to subscribe to
TOPIC = "data"

# Callback function for when a message is received on the subscribed topic
def on_message(client, userdata, message):
    print("Received message on topic '{}': {}".format(message.topic, message.payload.decode()))

# Callback function for when a client subscribes to a topic
# client, userdata, flags, reason_code, properties
def on_subscribe(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    # print("Subscribed to topic: '{}'".format(TOPIC))

# MQTT client initialization
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

# Set the callback functions
client.on_message = on_message
client.on_subscribe = on_subscribe

# Connect to the MQTT broker
client.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT)

# Subscribe to the topic
client.subscribe(TOPIC)

# Loop to handle incoming messages
client.loop_start()

# Keep the script running to receive messages
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Exiting...")
    client.loop_stop()
    client.disconnect()

