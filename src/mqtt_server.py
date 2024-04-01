import paho.mqtt.server as mqtt
import time

# MQTT broker settings
MQTT_BROKER_HOST = "localhost"
MQTT_BROKER_PORT = 1883

# MQTT topic to subscribe to
TOPIC = "test_topic"

# Callback function for when a message is received on the subscribed topic
def on_message(client, userdata, message):
    print("Received message on topic '{}': {}".format(message.topic, message.payload.decode()))

# Callback function for when a client subscribes to a topic
def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed to topic: '{}'".format(TOPIC))

# MQTT server initialization
mqtt_server = mqtt.MQTTServer(MQTT_BROKER_HOST, MQTT_BROKER_PORT)

# Set the callback functions
mqtt_server.on_message = on_message
mqtt_server.on_subscribe = on_subscribe

# Start the MQTT server
mqtt_server.start()

# Connect to the MQTT broker
client = mqtt.Client()
client.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT)

# Subscribe to the topic
client.subscribe(TOPIC)

# Keep the script running to receive messages
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Exiting...")
    mqtt_server.stop()
