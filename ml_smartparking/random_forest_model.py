# -*- coding: utf-8 -*-
"""
Created on Wed Mar 10 15:15:55 2021

@author: Who
"""

from sklearn.ensemble import RandomForestClassifier
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.preprocessing import StandardScaler, MinMaxScaler
from sklearn.pipeline import Pipeline
from sklearn.model_selection import KFold
from sklearn import metrics
import numpy as np
from sklearn.metrics import confusion_matrix
import pickle

data = pd.read_csv('dataset/real4_labelled.csv')

sensors_data = data.iloc[:,3:9]

#sensors_data.set_index(" hh:mm:ss")

#print(sensors_data.head())

X = sensors_data.iloc[:, 1:5]
Y = sensors_data.iloc[:, -1]

pl = Pipeline([
    ("scale", MinMaxScaler()), # Chose between MinMaxScaler or StandardScaler
    ("rf", RandomForestClassifier(n_estimators=500, max_depth=10, bootstrap=True, n_jobs=-1)) # tune hyperparam
    ])

scores = []
cv = KFold(n_splits=5, random_state=42, shuffle=True)
for train_index, test_index in cv.split(X):
    #print("Train Index: ", train_index, "\n")
    #print("Test Index: ", test_index)

    X_train, X_test, y_train, y_test = X.iloc[train_index], X.iloc[test_index], Y[train_index], Y[test_index]
    pl.fit(X_train, y_train)
    
    scores.append(pl.score(X_test, y_test))
    
    # Confusion matrix for each fold
    pred = pl.predict(X_test)
    print(confusion_matrix(y_test, pred))
    
print(scores)

print(np.mean(scores))

# save the model to disk
filename = 'rf_finalized_model.sav'
pickle.dump(pl, open("model/"+filename, 'wb'))
