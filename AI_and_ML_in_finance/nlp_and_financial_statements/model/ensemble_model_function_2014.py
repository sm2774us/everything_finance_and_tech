# -*- coding: utf-8 -*-
"""
Created on Tue Aug 25 15:07:17 2020

@author: gslicht
"""


import pandas as pd
import numpy as np


def ensemble_model_function_2014(doc):
    
    vectorizer = pd.read_pickle('tfidf_vectorizer_2014_dict.pickle')
    dict_models = pd.read_pickle('interim_model_dict.pickle')
    
    model_over = dict_models['model_over']
    model_under = dict_models['model_under']
    
    X = vectorizer.transform(doc)

    y_over_log_proba = model_over.predict_log_proba(X)[:,1]
    y_under_log_proba = model_under.predict_log_proba(X)[:,1]
        
    y_log_proba = 0.25*y_over_log_proba + 0.75*y_under_log_proba
    y_pred = (y_log_proba > np.log(0.5))*1
    
    return y_pred
    