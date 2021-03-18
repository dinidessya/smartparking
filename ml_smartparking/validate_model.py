# -*- coding: utf-8 -*-
"""
Created on Wed Mar 10 15:05:51 2021

@author: Who
"""
import numpy as np
import pickle
import pandas as pd

filename = 'finalized_model.sav'
model = pickle.load(open(filename, 'rb'))

val_set = pd.read_csv("dataset/testkuy_labelled.csv")
print(val_set.head())
