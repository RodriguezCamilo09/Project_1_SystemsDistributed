#!/usr/bin/env bash
echo "Compilacion"
g++ *.cpp -l pthread -o distributed
echo "Compilado