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
    print '\tnc_to_json.py <input.nc> <output.json> <max-count>\n'
    print '\t\toutputs the first <max-count> entries to output.json\n'
    sys.exit()

inputFileName = str(sys.argv[1])
outputFileName = str(sys.argv[2]) if len(sys.argv) >= 3 else inputFileName.replace('.nc', '.json')
maxCount = int(sys.argv[3]) if len(sys.argv) >= 4 else -1

print 'Reading NC from {}...\n'.format(inputFileName)

ncfile = netCDF4.Dataset(inputFileName, 'r')

times = ncfile.variables['time'][:]
lats = ncfile.variables['lat'][:]
lons = ncfile.variables['lon'][:]
depths = ncfile.variables['z'][:]

if maxCount != -1:
    times = times[:maxCount]
    lats = lats[:maxCount]
    lons = lons[:maxCount]
    depths = depths[:maxCount]

# remove nan and inf values
for values in [times, lats, lons, depths]:
    values[numpy.where(numpy.isnan(values))] = 0
    values[numpy.where(numpy.isinf(values))] = 0

# drifters = []
#
# for i in range(len(times)):
#     drifters.append([times[i], lats[i], lons[i], depths[i]])

with open(outputFileName, 'w') as outfile:
    print '\nWriting JSON to {}...'.format(outputFileName)
    # json.dump({'type': 'DrifterCollection', 'drifters': drifters}, outfile)
    json.dump({'type': 'DrifterCollection', 'drifters': {
        'times': times.tolist(),
        'lats': lats.tolist(),
        'lons': lons.tolist(),
        'depths': depths.tolist()
    }}, outfile)
