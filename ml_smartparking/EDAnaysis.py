# -*- coding: utf-8 -*-
"""
Created on Wed Mar 10 13:41:14 2021

@author: Who
"""

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data = pd.read_csv('dataset/real4_labelled.csv')
print(data.iloc[:,4:8].head())

sensors_data = data.iloc[:,4:8]
print(sensors_data.info())

print(sensors_data.describe())

print(sensors_data.plot(title="4 Sensors Data"))

sensors_data[[" Infrared", " Cahaya "]].plot(title="Sensor with value above 1000 and below 3000")
plt.xlabel("Time")

sensors_data.iloc[:, 1:4].plot(title="Sensor with value above 1000", secondary_y=" Magnetik")
plt.xlabel("Time")

sensors_data.hist(bins=10)

print(sensors_data.corr())

sns.heatmap(sensors_data.corr(), annot=True)

sns.pairplot(sensors_data)

# Looking at outlier
print(data)