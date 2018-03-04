import numpy as np
import math
from datetime import timedelta
from operator import attrgetter
from netCDF4 import Dataset
from netCDF4 import MFDataset
import xarray as xr
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sb
import os

"""
This python file is designed to extract data from a folder of .nc files.

written in python 3.6, and should be executed with python 3.6

This section is extracting the data from the folder of .nc files. In this
deployment, we assume that the files are called 'Cyclonic' and 'Anticyclonic'.

You will have to customize the source folder, which is flagged as such.
"""

print('extracting data from nc files')

#This section builds a list of files to extract from
anticyclonic = []
cyclonic = []
for item in os.listdir('followup'): #change this to the name of the folder with your .nc files
    if 'Anticyclonic' in item:
        anticyclonic.append(item)

for item in os.listdir('followup'):
    if 'Cyclonic' in item:
        cyclonic.append(item)

anticyclonic.sort()
cyclonic.sort()


#This section extracts the data
anti_size = []
anti_date = []

for entry in anticyclonic:
    dataset = Dataset('./followup/' + entry)
    anti_size.append(dataset.dimensions['Nobs'].size)
    anti_date.append(dataset.variables['j1'][4])

cyc_size = []
cyc_date = []

for entry in cyclonic:
    dataset = Dataset('./followup/' + entry)
    cyc_size.append(dataset.dimensions['Nobs'].size)
    cyc_date.append(dataset.variables['j1'][4])

#This section turns the extracted lists into dataframes
print('building dataframes')

anti_df = pd.DataFrame([anti_size, anti_date]).T
cyc_df = pd.DataFrame([cyc_size, cyc_date]).T

anti_df.columns = ['size', 'date']
cyc_df.columns = ['size', 'date']

anti_df['date'] = pd.to_datetime(anti_df['date'], origin='julian', unit='D')
# cyc_df['date'] = pd.to_datetime(anti_df['date'], origin='julian', unit='D')
anti_df = anti_df.set_index('date')
