#!/bin/bash

make clean
make all
wine ../emulator/bgb.exe game.gb  