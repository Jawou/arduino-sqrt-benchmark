# this code is ai generated

import serial
import serial.tools.list_ports
import time
import sys
from datetime import datetime

class ArduinoMonitor:
    def __init__(self, port=None, baud=9600):
        self.baud = baud
        self.port = port or self.find_arduino()
        self.ser = None

    def find_arduino(self):
        """Auto-detect Arduino port"""
        ports = serial.tools.list_ports.comports()
        for port in ports:
            # Common Arduino identifiers
            if 'Arduino' in port.description or 'ttyACM' in port.device or 'ttyUSB' in port.device:
                print(f"Found Arduino on {port.device}: {port.description}")
                return port.device

        # List all available ports
        print("Available ports:")
        for port in ports:
            print(f"  {port.device}: {port.description}")

        return None

    def connect(self):
        """Establish serial connection"""
        if not self.port:
            print("No port specified. Please provide a port.")
            sys.exit(1)

        try:
            self.ser = serial.Serial(self.port, self.baud, timeout=1)
            time.sleep(2)  # Wait for Arduino to reset
            print(f"Connected to {self.port} at {self.baud} baud")
            return True
        except serial.SerialException as e:
            print(f"Error connecting to {self.port}: {e}")
            return False

    def send_command(self, cmd):
        """Send command to Arduino"""
        if self.ser and self.ser.is_open:
            self.ser.write(cmd.encode())
            print(f"Sent: {cmd}")

    def monitor(self, save_to_file=None):
        """Monitor serial data"""

        log_file = None
        if save_to_file:
            log_file = open(save_to_file, 'a')
            print(f"Logging to {save_to_file}\n")

        try:
            while True:
                # Read from Arduino
                if self.ser.in_waiting > 0:
                    line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                    if line:
                        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                        output = f"[{timestamp}] {line}"
                        print(output)

                        if log_file:
                            log_file.write(output + '\n')
                            log_file.flush()

                # Check for user input (non-blocking)
                import select
                if select.select([sys.stdin], [], [], 0)[0]:
                    cmd = sys.stdin.readline().strip()
                    if cmd.lower() == 'q':
                        break
                    elif cmd:
                        self.send_command(cmd)

                time.sleep(0.01)

        except KeyboardInterrupt:
            print("\n\nMonitoring stopped by user")
        finally:
            if log_file:
                log_file.close()
            self.close()

    def close(self):
        """Close serial connection"""
        if self.ser and self.ser.is_open:
            self.ser.close()
            print("Connection closed")

def main():
    import argparse

    parser = argparse.ArgumentParser(description='Arduino Serial Monitor')
    parser.add_argument('-p', '--port', help='Serial port (e.g., /dev/ttyACM0)')
    parser.add_argument('-b', '--baud', type=int, default=9600, help='Baud rate (default: 9600)')
    parser.add_argument('-l', '--log', help='Log file path')

    args = parser.parse_args()

    monitor = ArduinoMonitor(port=args.port, baud=args.baud)

    if monitor.connect():
        monitor.monitor(save_to_file=args.log)

if __name__ == '__main__':
    main()
