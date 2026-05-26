import pygame
import serial
import serial.tools.list_ports
import math

pygame.init()
pygame.joystick.init()

# Open COM7 at 115200 baud (adjust if needed)
def open_serial(port='COM7', baud=115200):
    try:
        return serial.Serial(port, baud, timeout=1)
    except serial.SerialException as e:
        print(f"Failed to open serial port {port}: {e}")
        print("Available serial ports:")
        ports = serial.tools.list_ports.comports()
        for p in ports:
            print(f" - {p.device}: {p.description}")
        raise SystemExit(1)

arduino = open_serial('COM7', 115200)

if pygame.joystick.get_count() == 0:
    print("No joystick detected")
    exit()
else:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print(f"Joystick detected: {joystick.get_name()}")
    running = True
clock = pygame.time.Clock()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        left_right = 0.0
        up_down = 0.0

        # Axis motion (sticks, triggers)
        if event.type == pygame.JOYAXISMOTION:
            # Read left stick axes (adjust axis indices if needed)
            left_right = joystick.get_axis(0)  # Left stick horizontal
            up_down = joystick.get_axis(1)     # Left stick vertical
        clock.tick(60)


        # Convert joystick axes to polar coordinates
        # Note: joystick axis 1 is often positive when pushed down; invert so up is positive
        x = float(left_right)
        y = float(-up_down)
        radius = math.hypot(x, y)
        if radius > 1.0:
            radius = 1.0
        angle_deg = math.degrees(math.atan2(y, x)) if radius != 0 else 0.0

        # Send radius and angle (degrees) to Arduino as CSV: radius,angle\n
        msg = f"{radius:.3f},{angle_deg:.1f}\n"
        arduino.write(msg.encode('utf-8'))
        pygame.time.wait(5)  # Adjust delay as needed