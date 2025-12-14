import re
from argparse import ArgumentParser
import pathlib

parser = ArgumentParser(prog='extract_rounds')
parser.add_argument('input', type=pathlib.Path)

args = parser.parse_args()

ROUNDS_REGEX = r'^Took ([0-9]+) rounds to find the cut$'

data = ''
with open(args.input, 'r') as f:
    data = f.read()

matches = re.findall(ROUNDS_REGEX, data, flags=re.MULTILINE)

numbers = [int(num) for num in matches]

avg = sum(numbers) // len(numbers)

print(f"Average Rounds: {avg} - Succesful Runs: {len(numbers)}")