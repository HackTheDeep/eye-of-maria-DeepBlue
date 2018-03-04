import datetime
import math
import json
import netCDF4
import numpy
import sys
import argparse

# Define the parser
parser = argparse.ArgumentParser(description='NC to JSON converter')
parser.add_argument('input', action='store', help='NC file to read from')
parser.add_argument('output', action='store', help='JSON file to write to (defaults to <input.json>)')
parser.add_argument('--clamp', action='store', dest='clamp', type=int, default=-1, help='Max number of samples to parse. Defaults to -1 to parse all samples.')
parser.add_argument('--interval', action='store', dest='interval', type=int, default=-1, help='The interval used to sample every n-th element. Defaults to 1.\n')
parser.add_argument('--fields', action='store', dest='fields', nargs='*', default=-1, help='The fields to parse from the NC file. You can use `ncdump -h` to get a list of fields.')
args = parser.parse_args()

print 'Reading NC from {}...\n'.format(args.input)

ncfile = netCDF4.Dataset(args.input, 'r')

results = {}

for field in args.fields:
    values = ncfile.variables[field][:]

    # sample every N-th events
    if args.interval != 1:
        values = values[::,::args.interval]

    # clamp to N events
    if args.clamp != -1:
        values = values[:args.clamp]

    # remove nan and inf values
    values[numpy.where(numpy.isnan(values))] = 0
    values[numpy.where(numpy.isinf(values))] = 0

    results[field] = values.tolist()

with open(args.output, 'w') as outfile:
    print '\nWriting JSON to {}...'.format(args.output)
    json.dump(results, outfile)
