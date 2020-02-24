#!/bin/bash

make clean
make all
cp game.gb ../emulators/game_.gb
wine ../emulator/bgb.exe game.gb  