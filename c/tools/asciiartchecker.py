#!/usr/bin/env python3
import re
import sys

MSVC_LIMIT = 16380

def check_file(filename):
    """Check string literal lengths in a C++ header file."""
    with open(filename, 'r', encoding='utf-8') as file:
        content = file.read()
        
    # Dictionary to store the longest version of each constant
    constants = {}
    
    # Find all constant definitions
    const_matches = re.finditer(r'const QString\s+(\w+)\s*=\s*((?:["\n][^;]*)*);', content, re.MULTILINE)
    
    for match in const_matches:
        name = match.group(1)
        literal_text = match.group(2)
        # Remove line continuations and quotes
        literal_text = re.sub(r'"\s*"', '', literal_text)
        literal_text = re.sub(r'^"|"$', '', literal_text)
        
        # Store only the longer version if we've seen this constant before
        if name not in constants or len(literal_text) > len(constants[name]['text']):
            line_num = content.count('\n', 0, match.start()) + 1
            length = len(literal_text)
            constants[name] = {
                'length': length,
                'line': line_num,
                'text': literal_text
            }

    # Print individual results sorted by name
    total_size = sum(info['length'] for info in constants.values())
    
    for name in sorted(constants.keys()):
        info = constants[name]
        print(f"{name}: {info['length']} characters (line {info['line']})")

    print(f"\nTotal size of string literals: {total_size} characters")
    if total_size > MSVC_LIMIT:
        print(f"*** EXCEEDS MSVC LIMIT by {total_size - MSVC_LIMIT} characters ***")
        print(f"Need to reduce by {((total_size - MSVC_LIMIT) / total_size * 100):.1f}%")

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <header_file>")
        sys.exit(1)
    
    check_file(sys.argv[1])

if __name__ == '__main__':
    main()
