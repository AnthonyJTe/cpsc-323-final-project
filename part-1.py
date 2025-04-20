import re

def clean_file(input_path, output_path):
    with open(input_path, 'r', encoding='utf-8') as data:
        lines = data.readlines()

    cleaned_lines = []
    in_comment_block = False

    for line in lines:
        stripped = line.strip()

        if not stripped:
            continue

        if stripped.startswith('//') and stripped.endswith('//'):
            continue

        if stripped.startswith('//'):
            in_comment_block = True
            continue

        if stripped.endswith('//'):
            in_comment_block = False
            continue

        if in_comment_block:
            continue

        code = re.split(r'//', stripped)[0].strip()
        if not code:
            continue

        tokens = re.findall(r'\w+|“[^”]*”|\"[^\"]*\"|[^\s\w]', code)
        cleaned_line = ' '.join(tokens)
        cleaned_lines.append(cleaned_line)
    with open(output_path, 'w', encoding='utf-8') as outfile:
        for line in cleaned_lines:
            outfile.write(line + '\n')

clean_file('final.txt', 'final25.txt')
