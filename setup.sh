#!/bin/bash

echo "Checking required libs..."

when_installed() {
        echo "libsdl2-dev is installed"
        echo "Running Makefile..."
        make
        echo "Cleaning..."
        make clean
}

if dpkg -s libsdl2-dev; then
        when_installed
else
        echo "libsdl2-dev is NOT installed"
        echo "Installing SDL2 libs..."
        sudo apt install libsdl2-dev

        if dpkg -s libsdl2-dev; then
                when_installed;
        else
                echo "libsdl2-dev could NOT installed"
                echo "Please try again."
                kill $$
        fi
fi
