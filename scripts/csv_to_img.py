import pandas as pd
from imageio import imwrite

in_filename = '../data/boat.csv'
df = pd.read_csv(in_filename, sep=' ')
height = 900
width = 1350
dimension = 3
image_data = df.drop(columns=['x','y']).to_numpy().reshape(height, width, dimension)
imwrite('../data/output.jpeg', image_data)
