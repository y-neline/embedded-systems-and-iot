from machine import Pin, SPI, SoftI2C, UART
from lora import LoRa
import ssd1306
import time
import urequests
import network

rst = Pin(16, Pin.OUT)
rst.value(1)
scl = Pin(15, Pin.OUT, Pin.PULL_UP)
sda = Pin(4, Pin.OUT, Pin.PULL_UP)
i2c = SoftI2C(scl=scl, sda=sda, freq=450000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c, addr=0x3c)
oled.poweron()

oled.fill(0)
oled.text('turned on', 0, 0)
oled.show()

SCK  = 5
MOSI = 27
MISO = 19
CS   = 18
RX   = 26

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

gps_uart = UART(1, baudrate=9600, tx=17, rx=16)

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
    data = {
        'chat_id': telegram_chat_id,
        'text': message
    }
    response = urequests.post(url, json=data)
    response.close()

def get_gps_time(): 
    while gps_uart.any():
        time.sleep(0.5)
        try:
            gps_data = gps_uart.readline().decode('ascii')  
            print(gps_data)
            if "$GPRMC" in gps_data:
                parts = gps_data.split(',')
                hhmmss = parts[1] if len(parts) > 1 else "000000"
                try:
                    hours, minutes, seconds = int(hhmmss[0:2]), int(hhmmss[2:4]), int(hhmmss[4:6])
                    hours = (hours + 5) % 24
                    
                    formatted_time = f"[TRANSMITTER] GPS Time: {hours:02}:{minutes:02}:{seconds:02}"
                    send_telegram_message(formatted_time)
                    
                    total_seconds = hours * 3600 + minutes * 60 + seconds
                    
                    return total_seconds
                except (ValueError, IndexError):
                    print(f"Error parsing time from GPS data: {gps_data.strip()}")
                    return 0 
        except UnicodeError:
            print("Unicode decode error in GPS data.")
            return 0  
    return 0 

connect_wifi()

while True:
    gps_time = get_gps_time()
    
    packet = f"{gps_time}" 
    text_lora = f"[TRANSMITTER] Packet waiting to be sent: {packet}"
    print(text_lora)
    send_telegram_message(text_lora)
    
    lora.send(packet.encode('utf-8'))  
    text_lora = f"[TRANSMITTER] Sent packet to LoRa: {packet}"
    send_telegram_message(text_lora)
    print(text_lora)
        
    time.sleep(5)  