##
## EPITECH PROJECT, 2025
## zappy
## File description:
## network
##

import socket

class NetworkClient:
    def __init__(self, host, port, team_name):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.connected = False

    def connect(self):
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            print("Socket connected, waiting for WELCOME...")
            welcome = self.socket.recv(1024).decode()
            print("Server says:", welcome.strip())
            if "WELCOME" in welcome:
                self.socket.send((self.team_name + '\n').encode())
                print(f"Sent team name: {self.team_name}")
                client_num = self.socket.recv(1024).decode()
                map_size = self.socket.recv(1024).decode()
                print(f"Connected! Client slots: {client_num.strip()}")
                print(f"Map size: {map_size.strip()}")
                self.connected = True
                return True
            return False
        except Exception as e:
            print(f"Connection failed: {e}")
            return False

    def send(self, message):
        """Envoie un message au serveur"""
        if self.socket and self.connected:
            self.socket.send((message + '\n').encode())

    def receive(self):
        """Reçoit un message du serveur"""
        if self.socket and self.connected:
            return self.socket.recv(1024).decode()
        return ""

    def disconnect(self):
        """Fermeture de la connexion"""
        if self.socket:
            self.socket.close()
            self.connected = False