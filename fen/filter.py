#!/usr/bin/env python3
def is_fen( s):
    if s.find( '/') == -1: return False
    return True

while True:
    try:
        s = input()
        if is_fen( s): print( s)
    except EOFError: break
        
