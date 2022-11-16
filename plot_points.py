#This script will put the csv data points file inside a dataframe
#and will draw it using matplotlib

import pandas as pd
import matplotlib.pyplot as plt

with open('movements.csv', 'r') as file:
    xy = []
    line = file.read().splitlines()
    for caracter in line:
        data = caracter.split(',')
        s0 = data[0]
        s1 = data[1]

        if s0!= "0" and s1 != "0":
            xy.append([s0,s1])

      

points_df = pd.DataFrame(xy , columns = ['x', 'y'])

#The list obtained by reading the file has object elements. They must be a
#number in order to plot them.
points_df = points_df.apply(pd.to_numeric)


points_df.plot(x = 'x', y = 'y')

max_x = points_df['x'].max()
min_x = points_df['y'].min()

plt.title('Drawing')

plt.show()

