def translate_line(line):
    line = line.strip().rstrip(';')
    if line.startswith(('program', 'var', 'begin', 'end')):
        return ''
    if line.startswith('show'):
        content = line[5:-1]
        return f'print{content}'
    if '=' in line:
        return line
    return ''

with open("final25.txt", "r") as file:
    pseudocode_lines = file.readlines()

translated_lines = [translate_line(line) for line in pseudocode_lines]
python_code = '\n'.join(filter(None, translated_lines))

with open("translated.py", "w") as py_file:
    py_file.write(python_code)