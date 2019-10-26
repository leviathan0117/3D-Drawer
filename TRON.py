from OpenGL.GL   import *
from OpenGL.GLU  import *
from OpenGL.GLUT import *
from PIL import Image
import sys
import math

##### Code structure:
# constants declaration
# variables declaration 
# local varibles declaration - their use outside TRON library is unlikely
# function declaration

################################################################################ CONSTANTS:
#!!!!! CONSTANTS WORK ONLY WITHIN DEFAULT FUNCTIONS !!!!!

# Makes camera revolve around a certain point
CONST_CameraRevolve = 1
# Makes camera look around FROM a specific point
CONST_CameraLookAround = 2
# Makes camera move in a plane - WASD keys are for changin plane coordinates, C or SPACE for rising/lowing down the plane
CONST_MoveField = 1

################################################################################ Variables:

######################################## CAMERA:
cameraMode = CONST_CameraRevolve
cameraAngle1 = 0
cameraAngle2 = 0
# Camera position in (X, Y, Z)
cameraPosX = 0
cameraPosY = 0
cameraPosZ = 0
# Point at which the camera looks in (X, Y, Z)
cameraLookAtX = 0
cameraLookAtY = 0
cameraLookAtZ = 0
# Regulates how sensitive camera movement would be to mouse movement. Original value = 0.005
cameraSensitivity = 0.005

#################### Parameters for CONST_CameraRevolve (0):


#################### Parameters for CONST_CameraLookAround (1):


######################################## Movement:
movementMode = CONST_MoveField
movementSpeed = 0.2

# Allows / prohibits changing movement speed
allowMovementSpeedChange = 1

######################################## DRAWING:
colorR = 1
colorG = 1
colorB = 1

######################################## LOG parameters:
printMouseButtonEvent = 0
printMouseMoveEvent = 0

################################################################################ Local variables:
mouseXPrev = 0
mouseYPrev = 0

################################################################################ FUNCTIONS:

def concatenateFunctions (*functions):
    def func (*args):
        for function in functions:
            if callable(function):
                function (*args)
    return func

def loadTexture (fileName): # This DOES work but isn't implemented yet
    image  = Image.open (fileName)
    width  = image.size [0]
    height = image.size [1]
    image  = image.tobytes ("raw", "RGBX", 0, -1)
    texture = glGenTextures (1)
    
    glBindTexture (GL_TEXTURE_2D, texture)   # 2d texture (x and y size)
    glPixelStorei (GL_UNPACK_ALIGNMENT,1)
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR)
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR)
    gluBuild2DMipmaps (GL_TEXTURE_2D, 3, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image)
    
    return texture

def displayEnd ():
    glutSwapBuffers ()

#Default function for mouse button events:
def mouseButtonFunction (buttonID, buttonState, mouseX, mouseY):
    if printMouseButtonEvent:
        if buttonID == GLUT_LEFT_BUTTON:
            if buttonState == GLUT_DOWN:
                print("Left button down", end="")
            else:
                print("Left button up", end="")
        else:
            if buttonState == GLUT_DOWN:
                print("Right button down", end="")
            else:
                print("Right button up", end="")
        print(" || Mouse position: (" + str(mouseX) + ", " + str(mouseY) + ")")

    global mouseXPrev, mouseYPrev
    mouseXPrev = mouseX
    mouseYPrev = mouseY

#Default function for mouse movements (if any button pressed):
def mouseMoveFunction (mouseX, mouseY):
    global mouseXPrev, mouseYPrev
    global cameraAngle1, cameraAngle2

    deltaX = mouseX - mouseXPrev
    deltaY = mouseY - mouseYPrev

    cameraAngle1 += deltaX * cameraSensitivity
    cameraAngle2 -= deltaY * cameraSensitivity
    #the 0.01 - is a bug fixer which doesn't allow for Pi/2 or close to Pi/2 angles (as it results in strange behavior)
    #TODO: fix this problem somehow
    if cameraAngle2 >= math.pi / 2 - 0.01: 
        cameraAngle2 = math.pi / 2 - 0.01
    elif cameraAngle2 <= -math.pi / 2 + 0.01:
        cameraAngle2 = -math.pi / 2 + 0.01

    mouseXPrev = mouseX
    mouseYPrev = mouseY

    if printMouseMoveEvent:
        print("Mouse moved || Mouse position: (" + str(mouseX) + ", " + str(mouseY) + ")")


# Temporary here for faster library development
# TODO: remove from here

keyState = {'a': 0, 'b': 0, 'c': 0, 'd': 0, 'e': 0, 'f': 0, 'g': 0, 'h': 0, 'i': 0, 'j': 0, 'k': 0, 'l': 0, 'm': 0, 'n': 0, 'o': 0, 'p': 0, 'q': 0, 'r': 0, 's': 0, 't': 0, 'u': 0, 'v': 0, 'w': 0, 'x': 0, 'y': 0, 'z': 0, ' ': 0}

def keyboardFunction (*args):
    if args [0] == b'\x1b':
        sys.exit ()

    global keyState

    if args [0] == b'a':
        keyState['a'] = 1
    if args [0] == b'b':
        keyState['b'] = 1
    if args [0] == b'c':
        keyState['c'] = 1
    if args [0] == b'd':
        keyState['d'] = 1
    if args [0] == b'e':
        keyState['e'] = 1
    if args [0] == b'f':
        keyState['f'] = 1
    if args [0] == b'g':
        keyState['g'] = 1
    if args [0] == b'h':
        keyState['h'] = 1
    if args [0] == b'i':
        keyState['i'] = 1
    if args [0] == b'j':
        keyState['j'] = 1
    if args [0] == b'k':
        keyState['k'] = 1
    if args [0] == b'l':
        keyState['l'] = 1
    if args [0] == b'm':
        keyState['m'] = 1
    if args [0] == b'n':
        keyState['n'] = 1
    if args [0] == b'o':
        keyState['o'] = 1
    if args [0] == b'p':
        keyState['p'] = 1
    if args [0] == b'q':
        keyState['q'] = 1
    if args [0] == b'r':
        keyState['r'] = 1
    if args [0] == b's':
        keyState['s'] = 1
    if args [0] == b't':
        keyState['t'] = 1
    if args [0] == b'u':
        keyState['u'] = 1
    if args [0] == b'v':
        keyState['v'] = 1
    if args [0] == b'w':
        keyState['w'] = 1
    if args [0] == b'x':
        keyState['x'] = 1
    if args [0] == b'y':
        keyState['y'] = 1
    if args [0] == b'z':
        keyState['z'] = 1
    if args [0] == b' ':
        keyState[' '] = 1

def keyboardUpFunction (*args):
    global keyState

    if args [0] == b'a':
        keyState['a'] = 0
    if args [0] == b'b':
        keyState['b'] = 0
    if args [0] == b'c':
        keyState['c'] = 0
    if args [0] == b'd':
        keyState['d'] = 0
    if args [0] == b'e':
        keyState['e'] = 0
    if args [0] == b'f':
        keyState['f'] = 0
    if args [0] == b'g':
        keyState['g'] = 0
    if args [0] == b'h':
        keyState['h'] = 0
    if args [0] == b'i':
        keyState['i'] = 0
    if args [0] == b'j':
        keyState['j'] = 0
    if args [0] == b'k':
        keyState['k'] = 0
    if args [0] == b'l':
        keyState['l'] = 0
    if args [0] == b'm':
        keyState['m'] = 0
    if args [0] == b'n':
        keyState['n'] = 0
    if args [0] == b'o':
        keyState['o'] = 0
    if args [0] == b'p':
        keyState['p'] = 0
    if args [0] == b'q':
        keyState['q'] = 0
    if args [0] == b'r':
        keyState['r'] = 0
    if args [0] == b's':
        keyState['s'] = 0
    if args [0] == b't':
        keyState['t'] = 0
    if args [0] == b'u':
        keyState['u'] = 0
    if args [0] == b'v':
        keyState['v'] = 0
    if args [0] == b'w':
        keyState['w'] = 0
    if args [0] == b'x':
        keyState['x'] = 0
    if args [0] == b'y':
        keyState['y'] = 0
    if args [0] == b'z':
        keyState['z'] = 0
    if args [0] == b' ':
        keyState[' '] = 0

def processKey ():
    global cameraPosX, cameraPosY, cameraPosZ
    global cameraLookAtX, cameraLookAtY, cameraLookAtZ
    global cameraMode, movementMode
    global movementSpeed, allowMovementSpeedChange
    global keyState

    if movementMode == CONST_MoveField:
        if keyState['w']:
            cameraLookAtZ -= movementSpeed * math.cos(cameraAngle1 - math.pi / 2)
            cameraLookAtX -= movementSpeed * math.sin(cameraAngle1 - math.pi / 2)
        if keyState['s']:
            cameraLookAtZ += movementSpeed * math.cos(cameraAngle1 - math.pi / 2)
            cameraLookAtX += movementSpeed * math.sin(cameraAngle1 - math.pi / 2)
        if keyState['d']:
            cameraLookAtZ -= movementSpeed * math.cos(cameraAngle1)
            cameraLookAtX -= movementSpeed * math.sin(cameraAngle1)
        if keyState['a']:
            cameraLookAtZ += movementSpeed * math.cos(cameraAngle1)
            cameraLookAtX += movementSpeed * math.sin(cameraAngle1)
        if keyState['c']:
            cameraLookAtY += movementSpeed
        if keyState[' ']:
            cameraLookAtY -= movementSpeed
        if keyState['e'] and allowMovementSpeedChange:
            movementSpeed += 0.01
        if keyState['q'] and allowMovementSpeedChange:
            movementSpeed -= 0.01
            if movementSpeed < 0:
                movementSpeed = 0

def processCamera ():
    global cameraAngle1, cameraAngle2
    global cameraPosX, cameraPosY, cameraPosZ
    global cameraLookAtX, cameraLookAtY, cameraLookAtZ

    if cameraMode == CONST_CameraRevolve:
        lx = -math.cos(cameraAngle1) * math.cos(cameraAngle2)
        ly = math.sin(cameraAngle2)
        lz = math.sin(cameraAngle1) * math.cos(cameraAngle2)
        cameraPosX = cameraLookAtX + lx * 4
        cameraPosY = cameraLookAtY + ly * 4
        cameraPosZ = cameraLookAtZ + lz * 4
    
    if cameraMode != 0:
        gluLookAt (cameraPosX, cameraPosY, cameraPosZ,
                    cameraLookAtX, cameraLookAtY, cameraLookAtZ,
                    0, -1, 0)

def displaySetting ():
    glClear        (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glMatrixMode   (GL_MODELVIEW)
    glLoadIdentity ()

    processKey()
    processCamera()

def pointCamera (eyeX, eyeY, eyeZ, lookPointX, lookPointY, lookPointZ):
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glMatrixMode (GL_MODELVIEW)
    glLoadIdentity ()
    gluLookAt (eyeX, eyeY, eyeZ, lookPointX, lookPointY, lookPointZ, 0, -1, 0)

def idleFunction ():
    glutPostRedisplay ()

def init ():
    glClearColor (0.0, 0.0, 0.0, 0.0)
    glClearDepth (1.0) 
    glDepthFunc (GL_LEQUAL)
    glEnable (GL_DEPTH_TEST)
    glEnable (GL_TEXTURE_2D)
    glHint (GL_POLYGON_SMOOTH_HINT,         GL_NICEST)
    glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

def reshapeFunction (width, height):
    glViewport (0, 0, width, height)
    glMatrixMode (GL_PROJECTION)
    glLoadIdentity ()
    gluPerspective (60.0, float(width) / float (height), 1.0, 60.0)
    glMatrixMode (GL_MODELVIEW)
    glLoadIdentity ()
    
def Prepare (windowName, windowSizeX, windowSizeY, userDisplayFunction, userKeyboardFunction = None, userMouseButtonFunction = None, userMouseMoveFunction = None, windowPositionX = 100, windowPositionY = 100):
    glutInit (sys.argv)
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
    glutInitWindowSize (windowSizeX, windowSizeY)
    glutInitWindowPosition (windowPositionX, windowPositionY) 
    
    glutCreateWindow (windowName)
    glutDisplayFunc (concatenateFunctions (displaySetting, userKeyboardFunction, userDisplayFunction, displayEnd))
    glutIdleFunc (idleFunction)
    glutReshapeFunc (reshapeFunction)
    glutKeyboardFunc (keyboardFunction)
    glutKeyboardUpFunc (keyboardUpFunction)
    glutMouseFunc (concatenateFunctions (mouseButtonFunction, userMouseButtonFunction))
    glutMotionFunc (concatenateFunctions (mouseMoveFunction, userMouseMoveFunction))

def Launch ():
    init ()
    glutMainLoop ()


################################################## Drawing functions:

def setColorRGB256 (colorRIn, colorGIn, colorBIn):
    global colorR, colorG, colorB

    colorR = colorRIn / 255
    colorG = colorGIn / 255
    colorB = colorBIn / 255

def setColorRGB (colorRIn, colorGIn, colorBIn):
    if colorRIn > 1 or colorGIn > 1 or colorBIn > 1:
        print ("WRONG COLOR FUNCTION USED (0..1 instead of 0..255)")
    else:
        global colorR, colorG, colorB

        colorR = colorRIn
        colorG = colorGIn
        colorB = colorBIn

def drawSphere (xPosition, yPosition, zPosition, radius, quality):
    glColor3f(colorR, colorG, colorB)

    glPushMatrix()

    glTranslatef (xPosition, yPosition, zPosition)
    glutSolidSphere(radius, quality, quality)

    glPopMatrix()

def drawLine (xPosition1, yPosition1, zPosition1, xPosition2, yPosition2, zPosition2):
    glColor3f(colorR, colorG, colorB)

    glBegin(GL_LINES);
    glVertex3f(xPosition1, yPosition1, zPosition1);
    glVertex3f(xPosition2, yPosition2, zPosition2);
    glEnd();    
