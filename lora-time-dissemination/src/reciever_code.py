from machine import Pin, SPI, SoftI2C
from lora import LoRa
import ssd1306
import time
import network
import urequests

rst = Pin(16, Pin.OUT)
rst.value(1)
scl = Pin(15, Pin.OUT, Pin.PULL_UP)
sda = Pin(4, Pin.OUT, Pin.PULL_UP)
i2c = SoftI2C(scl=scl, sda=sda, freq=450000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c, addr=0x3c)
oled.poweron()

SCK = 5
MOSI = 27
MISO = 19
CS = 18
RX = 26

spi = SPI(
    1,
    baudrate=1000000,
    sck=Pin(SCK, Pin.OUT, Pin.PULL_DOWN),
    mosi=Pin(MOSI, Pin.OUT, Pin.PULL_UP),
    miso=Pin(MISO, Pin.IN, Pin.PULL_UP),
)
spi.init()

lora = LoRa(
    spi,
    cs=Pin(CS, Pin.OUT),
    rx=Pin(RX, Pin.IN),
)

# WiFi credentials (replace with your own for testing)
wifi_ssid = 'YOUR_WIFI_SSID'
wifi_password = 'YOUR_WIFI_PASSWORD'

# Telegram Bot configuration
telegram_bot_token = 'YOUR_TELEGRAM_BOT_TOKEN'
telegram_chat_id = 'YOUR_CHAT_ID'

def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(wifi_ssid, wifi_password)
    while not wlan.isconnected():
        print("Connecting to WiFi...")
        time.sleep(1)
    print("Connected to WiFi:", wlan.ifconfig())

def send_telegram_message(message):
    url = f"https://api.telegram.org/bot{telegram_bot_token}/sendMessage"
    data = {'chat_id': telegram_chat_id, 'text': message}
    try:
        response = urequests.post(url, json=data)
        response.close()
    except Exception as e:
        print("Failed to send Telegram message:", e)

def display_on_oled(received_time, adjusted_time, tx_time):
    oled.fill(0)
    oled.text("LoRa Data:", 0, 0)
    oled.text(received_time or "No Data", 0, 15)
    oled.text("Adj. Time:", 0, 35)
    oled.text(adjusted_time or "N/A", 0, 50)
    oled.text("Tx Time (ms):", 0, 60)
    oled.text(str(tx_time), 80, 60)
    oled.show()

def seconds_to_time(seconds):
    try:
        seconds = int(seconds)
        if seconds < 0:
            raise ValueError("Seconds cannot be negative.")
        hours = seconds // 3600
        minutes = (seconds % 3600) // 60
        remaining_seconds = seconds % 60
        return f"{hours:02}:{minutes:02}:{remaining_seconds}"
    except ValueError:
        return "Invalid Time"

# handler for LoRa packets
start_time = 0  
def handler(msg):
    global start_time
    if len(msg) > 0:
        print(msg)
        end_time = time.ticks_ms()
        transmission_time_ms = time.ticks_diff(end_time, start_time)
        try:
            data = msg.decode('utf-8').strip()
            print("Received packet:", data)
            received_time = seconds_to_time(data)
            adjusted_time = seconds_to_time(int(data) + (transmission_time_ms // 1000))
            
            display_on_oled(received_time, adjusted_time, transmission_time_ms)
            message = (f"[RECEIVER]/nReceived GPS Time: {received_time}\nAdjusted GPS Time: {adjusted_time}\nTransmission Time (ms): {transmission_time_ms}")
            send_telegram_message(message)
        except Exception as e:
            print("Error processing packet:", e)
            display_on_oled("Malformed", None, transmission_time_ms)
    else:
        print("No packet received.")
        display_on_oled(None, None, 0)

# main LoRa receiver loop
connect_wifi()

while True:
    try:
        start_time = time.ticks_ms()  
        lora.recv()
        lora.on_recv(handler) 
    except Exception as e:
        print("Error in LoRa receiving loop:", e)

    time.sleep(1) 
