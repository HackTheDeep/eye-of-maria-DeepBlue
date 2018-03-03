import datetime
import math
import json
import netCDF4
import numpy
import sys

if len(sys.argv) < 2:
    print '\nUsage:\n'
    print '\tnc_to_json.py <myfile.nc>\n'
    print '\t\toutputs to myfile.json\n'
    print '\tnc_to_json.py <input.nc> <output.json>\n'
    print '\t\toutputs to output.json\n'
    sys.exit()

inputFileName = str(sys.argv[1])
outputFileName = str(sys.argv[2]) if len(sys.argv) >= 3 else inputFileName.replace('.nc', '.json')

print 'Reading NC from {}...\n'.format(inputFileName)

ncfile = netCDF4.Dataset(inputFileName, 'r')

times = ncfile.variables['time'][:].tolist()
lats = ncfile.variables['lat'][:].tolist()
lons = ncfile.variables['lon'][:].tolist()
depths = ncfile.variables['z'][:].tolist()

# drifters = []
#
# for i in range(len(times)):
#     drifters.append([times[i], lats[i], lons[i], depths[i]])

with open(outputFileName, 'w') as outfile:
    print '\nWriting JSON to {}...'.format(outputFileName)
    # json.dump({'type': 'DrifterCollection', 'drifters': drifters}, outfile)
    json.dump({'type': 'DrifterCollection', 'drifters': {
        'times': times,
        'lats': lats,
        'lons': lons,
        'depths': depths
    }}, outfile)
