# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
"""Convert a native gameplay trace into the headless verifier's input script."""
import argparse
import csv
from pathlib import Path

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('trace', type=Path)
parser.add_argument('output', type=Path)
args = parser.parse_args()
keys = {'up': 38, 'down': 40, 'left': 37, 'right': 39,
        'space': 32, 'enter': 13, 'escape': 27}
previous = dict.fromkeys(keys, 0)
events = []
with args.trace.open(newline='', encoding='utf-8') as source:
    reader = csv.DictReader(source)
    if not set(keys).issubset(reader.fieldnames or []):
        parser.error('Trace has no input columns; record with the current executable.')
    for row in reader:
        for key, vk in keys.items():
            value = int(row[key])
            if value not in (0, 1):
                raise ValueError(f'Invalid input value at tick {row["tick"]}')
            if value != previous[key]:
                events.append(f'{int(row["tick"])} {vk} {value}')
                previous[key] = value
args.output.write_text('\n'.join(events) + '\n', encoding='utf-8')
print(f'{len(events)} input transitions written')
