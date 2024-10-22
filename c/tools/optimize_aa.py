import re

def quote_ascii_art(input_text):
    """Convert ASCII art literals to have each line individually quoted with newlines."""
    # Find all ASCII art constant definitions
    pattern = r'const QString (\w+)_AA = R"\(([\s\S]*?)\)";'
    matches = re.finditer(pattern, input_text)
    
    output = []
    current_pos = 0
    
    for match in matches:
        # Add text before the current match
        output.append(input_text[current_pos:match.start()])
        
        name = match.group(1)
        art = match.group(2)
        
        # Split into lines and process each line
        lines = art.strip().split('\n')
        quoted_lines = []
        for line in lines:
            if line.strip():  # Only process non-empty lines
                # Escape any existing quotes and backslashes
                line = line.replace('\\', '\\\\').replace('"', '\\"')
                quoted_lines.append(f'"{line}\\n"')
        
        # Join the quoted lines with newlines
        art_string = '\n'.join(quoted_lines)
        
        # Create the new constant definition
        output.append(f'const QString {name}_AA = \n{art_string};')
        
        current_pos = match.end()
    
    # Add remaining text
    output.append(input_text[current_pos:])
    
    return ''.join(output)

def process_header_file(input_path, output_path):
    """Process the header file and write the modified version."""
    with open(input_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    modified_content = quote_ascii_art(content)
    
    with open(output_path, 'w', encoding='utf-8', newline='\n') as f:
        f.write(modified_content)

if __name__ == '__main__':
    input_file = 'asciiart.h'
    output_file = 'asciiart_optimized.h'
    process_header_file(input_file, output_file)
    print(f"Processed {input_file} and wrote modified version to {output_file}")
