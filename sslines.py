#!/usr/bin/python2
# -*- coding: utf-8 -*-
# cpcssaver.py
# Versión: 0.1 [dom sep 30 13:29:02 CEST 2012]
# TODO: Añadir sonido aleatorio con algo como winsound :(

import pygame
import random

width = 640
height = 480

linecolor = 0, 0, 0
bgcolor = 0, 0, 0
rgb = [0, 0, 0]

destino = 0, 0

timeout = 100
waitmm = 100
screen = pygame.display.set_mode((width, height))


def rsound():
	frequency = random.randrange(37,100)
	duration = random.randrange(1, 10)

	try:
		import winsound
	except ImportError:
		import os
		#aptitude install beep
		os.system('beep -f %s -l %s' % (frequency * 10, duration * 10))
		#os.system('echo beep -f %s -l %s' % (frequency * 10, duration * 10))
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
		return( False )

	tecla = pygame.key.get_pressed()
	if tecla[pygame.K_q]:
		return( False )

	if tecla[pygame.K_c]:
		cls(timeout)

	if tecla[pygame.K_p]:
		pygame.event.wait()
		e = pygame.event.wait()
		if e.type == pygame.QUIT:
			return( False )
	return( True )




while control():

	linecolor = rcolor()

	origen = destino
	destino = random.randrange(width), random.randrange(height)

	pygame.draw.line(screen, linecolor, origen, destino)

	#rsound()

	cls(timeout)

	pygame.display.flip()
	pygame.time.wait(waitmm)
