import serial
from time import sleep
from enum import Enum

class JoystickMovement(Enum):
    UP = 0
    DOWN = 1
    LEFT = 2
    RIGHT = 3
    SELECT = 4

class ArmController:
    def __init__(self, port, baudrate=9600):
        self.port = port
        self.baudrate = baudrate
        self.ser = serial.Serial(port, baudrate)

        self._reset_board()
        self._await_response()

    def _reset_board(self):
        self.ser.setDTR(False)
        sleep(1)
        self.ser.flushInput()
        self.ser.setDTR(True)

    def _await_response(self):
        while True:
            line = self.ser.readline()

            try:
                line = line.decode().lower().strip()
            except UnicodeDecodeError:
                continue

            if line == "ok":
                return True
            elif line == "error":
                return False
            else:
                print(f"Received response: {line}")

    def send(self, msg):
        msg_encoded = f"{msg}\n".encode()
        self.ser.write(msg_encoded)

        response = self._await_response()
        
        return response
    
    def move_to_square(self, square):
        return self.send(f"move {square}")
    
    def discard_piece(self):
        return self.send("discard")
    
    def grab_piece(self):
        return self.send("grab")
    
    def release_piece(self):
        return self.send("release")
    
    def reset(self):
        return self.send("reset")
    
    def wait(self, time):
        return self.send(f"wait {time}")
    
    def get_joystick_movement(self):
        while True:
            line = self.ser.readline()

            try:
                line = line.decode().lower().strip()
            except UnicodeDecodeError:
                continue

            if line.startswith("joystick"):
                movement = line.split(" ")[1]
                if movement == "up":
                    return JoystickMovement.UP
                elif movement == "down":
                    return JoystickMovement.DOWN
                elif movement == "left":
                    return JoystickMovement.LEFT
                elif movement == "right":
                    return JoystickMovement.RIGHT
            elif line == "select":
                return JoystickMovement.SELECT
            else:
                print(f"Received response: {line}")

def main():
    arm = ArmController("COM4")
    arm.move_to_square("e2")

if __name__ == "__main__":
    main()
    

    