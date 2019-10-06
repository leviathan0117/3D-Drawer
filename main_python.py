import numpy as np
import time
import cv2
import imutils
import TRON
import sys

hog = cv2.HOGDescriptor()
hog.load("hog/detector_l.yml")
cap1 = cv2.VideoCapture(0)
#print(cap1.get(cv2.CAP_PROP_FRAME_WIDTH), cap1.get(cv2.CAP_PROP_FRAME_HEIGHT))
cap2 = cv2.VideoCapture(2)
hasFrame1, frame1 = cap1.read()
hasFrame2, frame2 = cap2.read()

cv2.imshow("Camera 1:", frame1)
cv2.imshow("Camera 2:", frame2)

start_time = time.time()
interval = 1 # displays the frame rate every 1 secon
counter = 0

x_fill = 0
y_fill = 0
z_fill = 0

#cv2.namedWindow("Camera 1:", cv2.WINDOW_AUTOSIZE )


def display():
	global cap1, cap2
	global hasFrame1, frame1
	global hasFrame2, frame2
	global start_time, interval, counter
	global x_fill, y_fill, z_fill

	hasFrame1, frame1 = cap1.read()
	if not hasFrame1:
		sys.exit ()
	image1 = frame1
	hasFrame2, frame2 = cap2.read()
	if not hasFrame2:
		sys.exit ()
	image2 = frame2

	X1 = -1
	Y1 = 0
	X2 = -1
	Y2 = 0

	(rects1, weights1) = hog.detectMultiScale(image1, winStride=(8, 8),
		padding=(16, 16), scale=1.05, finalThreshold=50)
	for (x, y, w, h) in rects1:
		cv2.rectangle(image1, (x, y), (x + w, y + h), (0, 255, 0), 2)
		X1 = x + w / 2
		Y1 = y + h / 2

	(rects2, weights2) = hog.detectMultiScale(image2, winStride=(8, 8),
		padding=(16, 16), scale=1.05, finalThreshold=50)
	for (x, y, w, h) in rects2:
		cv2.rectangle(image2, (x, y), (x + w, y + h), (0, 255, 0), 2)
		X2 = x + w / 2
		Y2 = y + h / 2


	cv2.imshow ("Camera 1:", image1)
	cv2.imshow ("Camera 2:", image2)
	if cv2.waitKey (10) == 27:
		sys.exit ()

	if X1 != -1 and X2 != -1:
		firstcamx=(X2 - 640 / 2) / 40
		secondcamx=(X1 - 640 / 2) / 40
		firstcamz=(480 / 2 - Y1) / 40
		firstcamx=(X2 - 640 / 2) / 40
		secondcamx=(X1 - 640 / 2) / 40
		firstcamz=(480 / 2 - Y1) / 40
		kinda23 = 30.8724
		if firstcamx != 0:
			a = kinda23 / (firstcamx)
		if secondcamx != 0:
			b = kinda23 / (secondcamx)
		if firstcamz != 0:
			c = kinda23 / (firstcamz)


		if firstcamx != 0 and secondcamx != 0 and firstcamz != 0:
			x_fill = (a + b) / (a - b)
			y_fill = (2 * a * b) / (a - b)
			z_fill = y_fill / c
		if firstcamz == 0 and firstcamx != 0 and secondcamx != 0:
			x_fill = (a + b) / (a - b)
			y_fill = (2 * a * b) / (a - b)
			z_fill = 0
		if firstcamx == 0 and firstcamz != 0:
			x_fill = 1
			y_fill = 2 * b
			z_fill = y_fill / c
		if secondcamx == 0 and firstcamz != 0:
			x_fill = -1
			y_fill = -2 * a
			z_fill = y_fill / c
		if firstcamx == 0 and firstcamz == 0:
			x_fill = 1
			y_fill = 2 * b
			z_fill = 0
		if secondcamx == 0 and firstcamz == 0:
			x_fill = 1
			y_fill = -2 * a
			z_fill = 0

		print(x_fill, y_fill, z_fill)

	counter += 1
	if (time.time() - start_time) > interval:
		print("FPS: ", counter / (time.time() - start_time))
		counter = 0
		start_time = time.time()

	TRON.drawSphere(y_fill / 2, -z_fill * 2, x_fill * 2, 0.1, 20)
	TRON.setColorRGB(0, 0, 1)
	TRON.drawLine(0, 0, 0, 100, 0, 0)
	TRON.setColorRGB(0, 1, 0)
	TRON.drawLine(0, 0, 0, 0, -100, 0)
	TRON.setColorRGB(1, 0, 0)
	TRON.drawLine(0, 0, 0, 0, 0, 100)


TRON.Prepare("bla", 1280, 720, display)

TRON.Launch()