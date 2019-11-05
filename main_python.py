import time
import cv2
import TRON
import sys
import functions
from imutils.video import WebcamVideoStream

currentFileVersion = 2
pointsX = []
pointsY = []
pointsZ = []
FPSMeter = TRON.FPS(2)
angleSpeed = 0
openCameras = 0
openVideo = 0
record = 0
countUnlikelyDetections = 0
colorPoints = False

print("Do you wish to open a file? ('y' to open, else 'n')")
workStyle = input()
if workStyle == "y" or workStyle == "Y":
    print("Enter file name to open")
else:
    print("Creating a new file")
    print("Enter file name")
fileName = input()
fileName = "files/" + fileName

if workStyle == "y" or workStyle == "Y":
    f = open(fileName + ".3dd", 'r')
    f.readline()
    tmp = int(f.readline())
    if tmp != currentFileVersion:
        print("FILE VERSION INVALID!!!")
        sys.exit(0)
    tmp = int(f.readline())
    for i in range(tmp):
        tmp = f.readline().split()
        pointsX.append(float(tmp[0]))
        pointsY.append(float(tmp[1]))
        pointsZ.append(float(tmp[2]))
    print("Data read successful")
    print("Would you like to open cameras for further recording? ('y' to open, else 'n')")
    tmp = input()
    if tmp == "y" or tmp == "Y":
        openCameras = 1
        print("Enter new file name to write to")
        fileName = input()
        fileName = "files/" + fileName
else:
    print("Do you wish to use cameras or video files as source? ('y' to open cameras, else 'n')")
    tmp = input()
    if tmp == "y" or tmp == "Y":
        openCameras = 1
        print("Do you wish to record? ('y' to open cameras, else 'n')")
        tmp = input()
        if tmp == "y" or tmp == "Y":
            record = 1
            print("Enter video set name")
            videoFileName = input()
            videoWriter1 = cv2.VideoWriter("video_files/" + videoFileName + "1.avi", cv2.VideoWriter_fourcc(*'XVID'), 30, (640, 480))
            videoWriter2 = cv2.VideoWriter("video_files/" + videoFileName + "2.avi", cv2.VideoWriter_fourcc(*'XVID'), 30, (640, 480))
    else:
        print("Enter video set name")
        videoFileName = input()
        cap1 = cv2.VideoCapture("video_files/" + videoFileName + "1.avi")
        cap2 = cv2.VideoCapture("video_files/" + videoFileName + "2.avi")
        tmp, frame1 = cap1.read()
        tmp, frame2 = cap2.read()
        cv2.imshow("Camera 1:", frame1)
        cv2.imshow("Camera 2:", frame2)
        openVideo = 1


hog = cv2.HOGDescriptor()
hog.load("detector/hog/detector_l.yml")

if openCameras:
    print("Please enter camera ID's one per line")
    cap1ID = int(input())
    cap2ID = int(input())
    print("Loading cameras")
    cap1 = WebcamVideoStream(src=cap1ID).start()
    cap2 = WebcamVideoStream(src=cap2ID).start()
    print("Loading complete")
    frame1 = cap1.read()
    frame2 = cap2.read()
    cv2.imshow("Camera 1:", frame1)
    cv2.imshow("Camera 2:", frame2)

def display():
    global cap1, cap2, frame1, frame2, FPSMeter, pointsX, pointsY, pointsZ, angleSpeed, openCameras, countUnlikelyDetections, openVideo, colorPoints, record

    X1 = -1
    Y1 = 0
    X2 = -1
    Y2 = 0

    if openCameras or openVideo:
        if openCameras:
            frame1 = cap1.read()
            frame2 = cap2.read()
        else:
            tmp, frame1 = cap1.read()
            tmp, frame2 = cap2.read()

        if record == 2:
            videoWriter1.write(frame1)
            videoWriter2.write(frame2)
            record = 1
        elif record == 0:
            (rects1, weights1) = hog.detectMultiScale(frame1, winStride=(8, 8), padding=(32, 32), scale=1.05, finalThreshold=5)
            (rects2, weights2) = hog.detectMultiScale(frame2, winStride=(8, 8), padding=(32, 32), scale=1.05, finalThreshold=5)

            for (x, y, w, h) in rects1:
                cv2.rectangle(frame1, (x, y), (x + w, y + h), (0, 255, 0), 2)
                X1 = x + w / 2
                Y1 = y + h / 2
            for (x, y, w, h) in rects2:
                cv2.rectangle(frame2, (x, y), (x + w, y + h), (0, 255, 0), 2)
                X2 = x + w / 2
                Y2 = y + h / 2

        cv2.imshow("Camera 1:", frame1)
        cv2.imshow("Camera 2:", frame2)
        if cv2.waitKey(10) == 27:
            sys.exit()

    if X1 != -1 and X2 != -1 and abs(Y1 - Y2) < 10:
        arr = functions.transferCoordinates(X1, (Y1 + Y2) / 2, X2)
        if arr is not None:
            pointsX.append(arr[0])
            pointsY.append(arr[1])
            pointsZ.append(arr[2])
    elif abs (Y1 - Y2) >= 10:
        countUnlikelyDetections += 1

    if countUnlikelyDetections >= 50:
        print("It seems that cameras are set not well enough")
        countUnlikelyDetections = 0

    if openVideo:
        print("Processed " + str(int(cap1.get(cv2.CAP_PROP_POS_FRAMES))) + " out of " + str(int(cap1.get(cv2.CAP_PROP_FRAME_COUNT))))
        if cap1.get(cv2.CAP_PROP_POS_FRAMES) == cap1.get(cv2.CAP_PROP_FRAME_COUNT):
            openVideo = 0
            cv2.destroyAllWindows()
            cap1.release()
            cap2.release()
            print("FINISHED!!!")

    else:
        FPSMeter.updateAndPrint()

    TRON.cameraAngle1 -= angleSpeed

    functions.drawBase()

    for i in range(len(pointsX)):
        if colorPoints:
            if i/len(pointsX) <= 0.34:
                TRON.setColorRGB(1, i / len(pointsX) * 3 - 0.05, i / len(pointsX) * 3 - 0.05)
            elif i/len(pointsX) <= 0.67:
                TRON.setColorRGB(1, 1, 1 - (i - len(pointsX) / 3) / len(pointsX) * 3)
            else:
                TRON.setColorRGB(1 - (i - 2 * len(pointsX) / 3) / len(pointsX) * 3, 1, (i - 2 * len(pointsX) / 3) / len(pointsX) * 3)
        TRON.drawSphere(pointsY[i] - functions.getArrayAverage(pointsY), pointsZ[i], pointsX[i], 0.05, 10)


fileSaveState = 0


def aditionalKeyboardHandle():
    global angleSpeed, pointsX, pointsY, pointsZ, fileSaveState, colorPoints, record

    if TRON.keyState['r'] == 1:
        pointsX = []
        pointsY = []
        pointsZ = []

    if TRON.keyState['i'] == 1 and fileSaveState == 0:
        f = open(fileName + ".3dd", 'w')
        f.write("File version:\n")
        f.write(str(currentFileVersion) + "\n")
        f.write(str(len(pointsX)) + "\n")
        for i in range(len(pointsX)):
            f.write(str(pointsX[i]) + " " + str(pointsY[i]) + " " + str(pointsZ[i]) + "\n")
        f.close()
        fileSaveState = 1
        print("Data saved")
    if TRON.keyState['i'] == 0:
        fileSaveState = 0

    if TRON.keyState['e']:
        angleSpeed = 0.1
    if TRON.keyState['q']:
        angleSpeed = -0.1
    if TRON.keyState['f']:
        angleSpeed = 0
    if TRON.keyState['t']:
        TRON.cameraLookAtX = TRON.cameraLookAtY = TRON.cameraLookAtZ = 0
    colorPoints = 0
    if TRON.keyState['g']:
        colorPoints = 1
    if TRON.keyState['x']:
        record = 2


TRON.Prepare(b"3D Drawer", 1000, 720, display, aditionalKeyboardHandle, None, None, None, 700, 200)

TRON.cameraDistanceToObject = 10

TRON.Launch()
