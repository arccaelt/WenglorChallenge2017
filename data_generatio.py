from PIL import Image
import os
import random

def generateBitmapImages(amount, path):
    for i in range(amount):
        width = random.randint(500, 2000)
        height = random.randint(500, 2000)

        img = Image.new("RGB", (width, height))

        print("Generating image #{} with ({}, {})".format(i, width, height))

        white_spots_amount = random.randint(1, 10)

        for ws in range(white_spots_amount):
            x = random.randint(0, width)
            y = random.randint(0, height)
            size = random.randint(30, 500)

            print("White spot #{} starts at ({}, {}) and has size {}".format(ws, x, y, size))

            for px in range(x, x + size):
                for py in range(y, y + size):
                    if px < width and py < height:
                        img.putpixel((px, py), (255, 255, 255))


        img.save(path + "\\output{}.bmp".format(i + 1))

generateBitmapImages(10, "C:\\Users\\arco\\Desktop\\images")
