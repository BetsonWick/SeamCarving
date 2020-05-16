import csv
from imageio import imread

in_filename = '../data/boat.jpeg'
image = imread(in_filename)
out_filename = '../data/boat.csv'
height, width, dimension = image.shape

with open(out_filename, 'w') as csv_file:
    image_writer = csv.writer(csv_file, delimiter=' ')
    image_writer.writerow([height, width])
    for i in range(height):
        for j in range(width):
            r, g, b = image[i, j, :]
            image_writer.writerow([r, g, b])

