#!/usr/bin/env python3
# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

import argparse
import os
import sys

import hjson
from mako.template import Template

'''
Read in a JSON test vector file, convert the test vector to C constants, and
generate a header file with these test vectors.
'''

# Number of 32-bit words in a coordinate or scalar
P256_NUMWORDS = int(256 / 32)

# Default template file name
DEFAULT_TEMPLATE = 'ecdsa_p256_verify_testvectors.h.tpl'

# Default output file name
DEFAULT_OUTFILE = 'ecdsa_p256_verify_testvectors.h'


def ecdsa_p256_int_to_hexwords(x):
    '''Convert a 256-bit integer to a list of 32-bit integers (little-endian).'''
    out = []
    for _ in range(P256_NUMWORDS):
        out.append(x & ((1 << 32) - 1))
        x >>= 32
    if x != 0:
        raise ValueError('Integer out of range!')
    return [f'{w:#010x}' for w in out]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('hjsonfile',
                        metavar='FILE',
                        type=argparse.FileType('r'),
                        help='Read test vectors from this HJSON file.')
    tpl_default = open(
        os.path.join(os.path.dirname(__file__), DEFAULT_TEMPLATE), 'r')
    parser.add_argument('--template',
                        metavar='FILE',
                        required=False,
                        default=tpl_default,
                        type=argparse.FileType('r'),
                        help='Read header template from this file.')
    out_default = open(
        os.path.join(os.path.dirname(__file__), DEFAULT_OUTFILE), 'w')
    parser.add_argument('headerfile',
                        metavar='FILE',
                        nargs='?',
                        default=out_default,
                        type=argparse.FileType('w'),
                        help='Write output to this file.')

    args = parser.parse_args()

    # Read test vectors and stringify them
    testvecs = hjson.load(args.hjsonfile)
    args.hjsonfile.close()

    # Convert the 256-bit numbers into words expressed in hex
    for t in testvecs:
        for param in ['x', 'y', 'r', 's']:
            t[param + '_hexwords'] = ecdsa_p256_int_to_hexwords(t[param])

    # Convert the message into an array of bytes
    for t in testvecs:
        t['msg_bytes'] = t['msg'].to_bytes(t['msg_len'], byteorder='big')

    args.headerfile.write(Template(args.template.read()).render(tests=testvecs))
    args.headerfile.close()
    out_default.close()
    args.template.close()
    tpl_default.close()

    return 0


if __name__ == '__main__':
    sys.exit(main())
