import datetime
import time
import math
import json
import netCDF4 # parse nc files
import numpy
import sys
import argparse
import jdcal # convert julian dates

# Define the parser
parser = argparse.ArgumentParser(description='NC to JSON converter')
parser.add_argument('input', action='store', help='NC file to read from')
parser.add_argument('output', action='store', help='JSON file to write to (defaults to <input.json>)')
parser.add_argument('--clamp', action='store', dest='clamp', type=int, default=-1, help='Max number of samples to parse. Defaults to -1 to parse all samples.')
parser.add_argument('--interval', action='store', dest='interval', type=int, default=1, help='The interval used to sample every n-th element in a 1D list. Defaults to 1.\n')
parser.add_argument('--subinterval', action='store', dest='subinterval', type=int, default=1, help='The sub sample interval used to sample every n-th element in a 2D list. Defaults to 1.\n')
parser.add_argument('--fields', action='store', dest='fields', nargs='*', default=-1, help='The fields to parse from the NC file. You can use `ncdump -h` to get a list of fields.')
args = parser.parse_args()

print 'Reading NC from {}...\n'.format(args.input)

ncfile = netCDF4.Dataset(args.input, 'r')

results = {}

def jul2Unix(julian):
    greg = jdcal.jd2gcal(julian, 0)
    dt = datetime.datetime(greg[0], greg[1], greg[2], int(greg[3] * 24), int((greg[3] * 24) % 60))
    return time.mktime(dt.timetuple())

vJul2Unix = numpy.vectorize(jul2Unix)

for field in args.fields:
    values = ncfile.variables[field][:]

    # convert julian dates to unix times
    if (field == 'j1'):
        values = vJul2Unix(values)
        field = 'time'

    # sample every N-th events
    if args.interval != 1:
        values = values[::args.interval]

    # sample every N-th events
    if args.subinterval != 1:
        values = values[::,::args.subinterval]

    # clamp to N events
    if args.clamp != -1:
        values = values[:args.clamp]

    # remove nan and inf values
    values[numpy.where(numpy.isnan(values))] = -1
    values[numpy.where(numpy.isinf(values))] = -1

    results[field] = values.tolist()

with open(args.output, 'w') as outfile:
    print '\nWriting JSON to {}...'.format(args.output)
    json.dump(results, outfile)
