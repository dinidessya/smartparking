# -*- coding: utf-8 -*-
"""
Created on Wed Mar 10 17:29:21 2021

@author: Who
"""

from sklearn.neighbors import KNeighborsClassifier
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.preprocessing import StandardScaler, MinMaxScaler
from sklearn.model_selection import train_test_split
from sklearn.pipeline import Pipeline
from sklearn.model_selection import KFold
from sklearn import metrics
import numpy as np
from sklearn.metrics import confusion_matrix
import pickle

data = pd.read_csv('dataset/testkuy_labelled.csv')

sensors_data = data.iloc[:,3:9]

#sensors_data.set_index(" hh:mm:ss")

#print(sensors_data.head())

X = sensors_data.iloc[:, 1:5]
Y = sensors_data.iloc[:, -1]

#split dataset into train and test data
X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size = 0.2, random_state=68, stratify=Y)

# Create KNN classifier
knn = KNeighborsClassifier(n_neighbors = 3)

pl = Pipeline([
    ("scale", MinMaxScaler()), # Chose between MinMaxScaler or StandardScaler
    ("knn", knn) # tune hyperparam
    ])

# Fit the classifier to the data
pl.fit(X_train,y_train)

print(y_test[0:5].values)
print(pl.predict(X_test)[0:5])

#check accuracy of our model on the test data
print(pl.score(X_test, y_test))

from sklearn.model_selection import cross_val_score

#create a new KNN model
knn_cv = KNeighborsClassifier(n_neighbors=3)

#train model with cv of 5 
cv_scores = cross_val_score(knn_cv, X, Y, cv=5)

#print each cv score (accuracy) and average them
print(cv_scores)
print('cv_scores mean:{}'.format(np.mean(cv_scores)))

from sklearn.model_selection import GridSearchCV

#create new a knn model
knn2 = KNeighborsClassifier()

#create a dictionary of all values we want to test for n_neighbors
param_grid = {'n_neighbors': np.arange(1, 25)}

#use gridsearch to test all values for n_neighbors
knn_gscv = GridSearchCV(knn2, param_grid, cv=5)

#fit model to data
knn_gscv.fit(X, Y)

#check top performing n_neighbors value
print(knn_gscv.best_params_)

#check mean score for the top performing value of n_neighbors
print(knn_gscv.best_score_)

# save the model to disk
filename = 'knn_finalized_model.sav'
pickle.dump(pl, open("model/"+filename, 'wb'))
