import pandas as pd
import numpy as np
from imageio import imwrite

in_filename = '../data/boat.csv'
with open(in_filename, 'r') as csv:
    line = csv.readline()
    width, height = [int(item) for item in line.split(' ')]
df = pd.read_csv(in_filename, sep=' ', skiprows=1, header=None)
items, dimension = df.shape
image_data = df.to_numpy().reshape(width, height, dimension)
image_data = np.transpose(image_data, axes=(1, 0, 2))
imwrite('../data/output.jpeg', image_data)
