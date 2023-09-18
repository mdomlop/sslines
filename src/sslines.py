#!/usr/bin/python
# -*- coding: utf-8 -*-
# cpcssaver.py
# Works with python2 and python3
# Versión: 0.1 [dom sep 30 13:29:02 CEST 2012]
# TODO: Añadir sonido aleatorio con algo como winsound :(

import pygame
import random
import pyaudio
import numpy as np

width = 640
height = 480

linecolor = 0, 0, 0
bgcolor = 0, 0, 0
rgb = [0, 0, 0]

destino = 0, 0

timeout = 100
waitmm = 100
screen = pygame.display.set_mode((width, height))
p = pyaudio.PyAudio()

def beepsound(frequency, duration):
    import subprocess
    #aptitude install beep
    frequency *= 10
    duration *= 10
    cmd = ('beep', '-f', str(frequency), '-l', str(duration))
    subprocess.run(cmd)
    
def synthsound(freq, duration, volume=10):
    volume = volume / 10  # float range [0.0, 1.0]
    sample_freq = 44100  # sampling rate, Hz, must be integer
    
    freq = float(freq)  # sine frequency, Hz, may be float
    duration = float(duration) * 1000  # in seconds, may be float
    
    # for paFloat32 sample values must be in range [-1.0, 1.0]
    stream = p.open(format=pyaudio.paFloat32,
                    channels=1,
                    rate=sample_freq,
                    output=True)
        
    # generate samples, note conversion to float32 array
    samples = (np.sin(2 * np.pi * np.arange(sample_freq * duration) * freq / sample_freq)).astype(np.float32)

    # play. May repeat with different volume values (if done interactively) 
    stream.write(volume*samples)
    stream.stop_stream()
    stream.close()

def rsound():
    frequency = random.randrange(37,100)
    duration = random.randrange(1, 10)

    try:
        import winsound
    except ImportError:
        #beepsound(frequency, duration)
        synthsound(frequency, duration)
    else:
        winsound.Beep(frequency, duration)
        #winsound.Beep(random.randrange(37,1000),random.randrange(10) + 1)


def rcolor():
    n = 255
    rgb[0] = random.randrange(n)
    rgb[1] = random.randrange(n)
    rgb[2] = random.randrange(n)
    return(rgb)


def cls(x):
    a = random.randrange(x)
    if a == 1:
        screen.fill((0, 0, 0))


def control():
    event = pygame.event.poll()
    if event.type == pygame.QUIT:
        return(False)

    tecla = pygame.key.get_pressed()
    if tecla[pygame.K_q]:
        return(False)

    if tecla[pygame.K_c]:
        cls(timeout)

    if tecla[pygame.K_p]:
        pygame.event.wait()
        e = pygame.event.wait()
        if e.type == pygame.QUIT:
            return(False)
    return(True)

while control():

    linecolor = rcolor()

    origen = destino
    destino = random.randrange(width), random.randrange(height)

    pygame.draw.line(screen, linecolor, origen, destino)

#rsound()

    cls(timeout)

    pygame.display.flip()
    pygame.time.wait(waitmm)

p.terminate()
