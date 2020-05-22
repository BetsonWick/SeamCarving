import csv
from imageio import imread

in_filename = '../data/boat.jpeg'
image = imread(in_filename)
out_filename = '../data/boat.csv'
height, width, dimension = image.shape

with open(out_filename, 'w') as csv_file:
    image_writer = csv.writer(csv_file, delimiter=' ')
    image_writer.writerow([width, height])
    for i in range(width):
        for j in range(height):
            r, g, b = image[j, i, :]
            image_writer.writerow([r, g, b])

