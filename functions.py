import TRON

def transferCoordinates(x1, y1, x2):
    firstcamx = (x2 - 640 / 2) / 40
    secondcamx = (x1 - 640 / 2) / 40
    firstcamz = (480 / 2 - y1) / 40
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

    # normalize coordinates
    x_fill *= -2
    z_fill *= -2

    if y_fill > 0 and y_fill < 30:
        return [x_fill, y_fill, z_fill]

def getArrayAverage(array):
    if len(array) == 0:
        return 0
    sum = 0
    for i in array:
        sum += i
    return sum / len(array)

def drawBase():
    lines_len = 20

    TRON.setColorRGB(0, 0, 1)
    TRON.drawLine(0, 0, 0, lines_len, 0, 0)
    for i in range(lines_len):
        TRON.drawSphere(i, 0, 0, 0.05, 10)
    TRON.setColorRGB(0, 1, 0)
    TRON.drawLine(0, 0, 0, 0, -lines_len, 0)
    for i in range(lines_len):
        TRON.drawSphere(0, -i, 0, 0.05, 10)
    TRON.setColorRGB(1, 0, 0)
    TRON.drawLine(0, 0, 0, 0, 0, lines_len)
    for i in range(lines_len):
        TRON.drawSphere(0, 0, i, 0.05, 10)