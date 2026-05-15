import pygame
import serial

pygame.init()
pygame.joystick.init()

arduino = serial.Serial('COM5', 9600)  # Adjust COM port as needed

if pygame.joystick.get_count() == 0:
    print("No joystick detected")
    exit()
else:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print(f"Joystick detected: {joystick.get_name()}")
    while True:
        left_right = 0
        up_down = 0
        if abs(joystick.get_axis(0)) > 0.1:
            left_right = joystick.get_axis(0)
        if abs(joystick.get_axis(1)) > 0.1:
            up_down = joystick.get_axis(1)
        arduino.write(f"{left_right},{up_down}\n", encoding='utf-8')
        wait = pygame.time.wait(5)  # Adjust delay as needed
