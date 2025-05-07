import re

def clean_file(input_path, output_path):
    with open(input_path, 'r', encoding='utf-8') as infile:
        lines = infile.readlines()

    cleaned_lines = []
    in_comment_block = False
    for line in lines:
        line = line.strip()
        if not line:
            continue
        if line.startswith('//') and line.endswith('//'):
            continue
        if in_comment_block:
            if line.endswith('//'):
                in_comment_block = False
            continue
        if line.startswith('//'):
            in_comment_block = True
            continue
        if '//' in line:
            line = line.split('//')[0].strip()
        if not line:
            continue
        tokens = re.findall(r'\w+|“[^”]*”|\"[^\"]*\"|[^\s\w]', line)
        cleaned_line = ' '.join(tokens)

        cleaned_lines.append(cleaned_line)

    with open(output_path, 'w', encoding='utf-8') as outfile:
        for line in cleaned_lines:
            outfile.write(line + '\n')

# Example use
clean_file('final.txt', 'final25.txt')
