#!/usr/bin/env python3
##
## EPITECH PROJECT, 2025
## zappy
## File description:
## main
##

import sys
import argparse
import time
import subprocess
from network import NetworkClient
from logic import AILogic

def print_usage():
    print("USAGE: ./zappy_ai -p port -n name -h machine")
    print("  -p port     port number")
    print("  -n name     name of the team")
    print("  -h machine  name of the machine; localhost by default")

def parse_args():
    if len(sys.argv) == 2 and sys.argv[1] == "help":
        print_usage()
        sys.exit(0)
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('-p', '--port', type=int, required=True)
    parser.add_argument('-n', '--name', type=str, required=True)
    parser.add_argument('-h', '--host', type=str, default='localhost')
    try:
        return parser.parse_args()
    except:
        print_usage()
        sys.exit(1)

def spawn_new_ai(port, team_name, host):
    subprocess.Popen(["./zappy_ai", "-p", str(port), "-n", team_name, "-h", host])

def main():
    args = parse_args()
    print(f"Zappy AI starting...")
    print(f"Connecting to {args.host}:{args.port} as team '{args.name}'")
    try:
        network = NetworkClient(args.host, args.port, args.name)
        if not network.connect():
            print("Failed to connect to server.")
            return
        logic = AILogic()
        print("AI initialized and connected!")
        network.send("Inventory")
        response = network.receive()
        logic.update_inventory(response)
        network.send("Look")
        response = network.receive()
        logic.update_from_look(response)
        while True:
            if not logic.command_queue:
                logic.decide_next_action()
            if logic.command_queue:
                cmd = logic.command_queue.pop(0)
                print(f">> Sending: {cmd}")
                network.send(cmd)
                response = network.receive()
                print(f"<< Response: {response.strip()}")
                cmd_lower = cmd.lower()
                if cmd_lower == "inventory":
                    logic.update_inventory(response)
                elif cmd_lower == "look":
                    logic.update_from_look(response)
                elif "Current level:" in response and not response.startswith("message"):
                    try:
                        level_str = response.strip().split("Current level:")[1]
                        new_level = int(level_str)
                        print(f"AI level up: {logic.level} -> {new_level}")
                        logic.level = new_level
                        logic.command_queue.insert(0, "Fork")
                        spawn_new_ai(args.port, args.name, args.host)
                    except ValueError:
                        print(f"Failed to parse level from response: {response}")
                elif cmd_lower.startswith("set") and "ok" in response:
                    obj = cmd.split(" ", 1)[1]
                    if logic.inventory.get(obj, 0) > 0:
                        logic.inventory[obj] -= 1
                elif cmd_lower.startswith("take") or cmd_lower in ["forward", "left", "right"]:
                    network.send("Inventory")
                    inv_resp = network.receive()
                    logic.update_inventory(inv_resp)
                    network.send("Look")
                    look_resp = network.receive()
                    logic.update_from_look(look_resp)
            time.sleep(0.2)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()