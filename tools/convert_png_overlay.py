"""Convert a manually edited RGBA PNG into the portable runtime overlay format.

Requires 8-bit RGBA, non-interlaced PNG. Pixels and alpha are preserved exactly.
Outputs are local game assets, not source files for public distribution.
"""
import argparse
import binascii
from pathlib import Path
import struct
import zlib


def decode(path):
    data = path.read_bytes()
    if data[:8] != b'\x89PNG\r\n\x1a\n':
        raise ValueError('Not a PNG')
    pos, compressed, header = 8, bytearray(), None
    while pos < len(data):
        size, = struct.unpack_from('>I', data, pos)
        kind = data[pos+4:pos+8]
        payload = data[pos+8:pos+8+size]
        crc, = struct.unpack_from('>I', data, pos+8+size)
        if binascii.crc32(kind+payload) & 0xffffffff != crc:
            raise ValueError('PNG CRC mismatch')
        if kind == b'IHDR':
            header = struct.unpack('>IIBBBBB', payload)
        elif kind == b'IDAT':
            compressed.extend(payload)
        pos += size+12
        if kind == b'IEND':
            break
    if not header or header[2:] != (8, 6, 0, 0, 0):
        raise ValueError('Expected 8-bit non-interlaced RGBA PNG')
    width, height = header[:2]
    if not width or not height or width*height > 16777216:
        raise ValueError('Invalid image dimensions')
    stride = width*4
    scan = zlib.decompress(compressed)
    if len(scan) != (stride+1)*height:
        raise ValueError('Unexpected decoded PNG size')
    output, previous = bytearray(), bytearray(stride)
    for y in range(height):
        start = y*(stride+1)
        method = scan[start]
        row = bytearray(scan[start+1:start+1+stride])
        if method > 4:
            raise ValueError('Invalid PNG filter')
        for i in range(stride):
            left = row[i-4] if i >= 4 else 0
            up = previous[i]
            corner = previous[i-4] if i >= 4 else 0
            predictor = 0
            if method == 1:
                predictor = left
            elif method == 2:
                predictor = up
            elif method == 3:
                predictor = (left+up)//2
            elif method == 4:
                p = left+up-corner
                distances = [abs(p-left), abs(p-up), abs(p-corner)]
                predictor = [left, up, corner][distances.index(min(distances))]
            row[i] = (row[i]+predictor) & 255
        output.extend(row)
        previous = row
    return width, height, output


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('png', type=Path)
    parser.add_argument('output', type=Path)
    args = parser.parse_args()
    w, h, rgba = decode(args.png)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(b'SHRGBA1\0'+struct.pack('<II', w, h)+rgba)
    print(f'Converted {w}x{h}; RGBA pixels preserved without palette quantization')
