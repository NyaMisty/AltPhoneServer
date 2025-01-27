#!/usr/bin/python3

import re
import sys

F = sys.argv[1]

with open(F, 'rb') as f:
    content = f.read()

content = content.replace(br'\\\\', br'/') # c escape + regex escape = 2*2
content = content.replace(br'\\', br'/')
content = re.sub(br'/(\.[A-Za-z])', br'\\\\\1', content)

content = content.replace(br'int main(', br'int __main(')

sys.stdout.buffer.write(content)