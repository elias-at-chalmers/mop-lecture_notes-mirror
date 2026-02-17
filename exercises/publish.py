#!/usr/bin/env python3

import os
import sys
import re
from pathlib import Path

def is_c_style(filename):
    """Check if file is C-style (uses # directives)."""
    return filename.endswith(('.c', '.h', '.cpp', '.cc', '.cxx'))

def is_asm_style(filename):
    """Check if file is assembly-style (uses . directives)."""
    return filename.endswith(('.s', '.S', '.asm'))

def remove_ifdef_blocks(content, ifdef_pattern, else_pattern, endif_pattern, define_pattern):
    """
    Remove #ifdef SOLUTION blocks from content.
    
    For each #ifdef SOLUTION:
    - If there's an #else at the same nesting level, keep content after #else until #endif
    - Otherwise, remove all content until #endif
    - Remove the #ifdef, #endif, #define, and #else lines themselves
    """
    lines = content.split('\n')
    result = []
    i = 0
    
    while i < len(lines):
        line = lines[i]
        
        # Remove #define SOLUTION or .set SOLUTION lines
        if re.match(define_pattern, line):
            i += 1
            # Skip the blank line after #define if present
            if i < len(lines) and lines[i].strip() == '':
                i += 1
            continue
        
        # Check for #ifdef SOLUTION or .if SOLUTION
        if re.match(ifdef_pattern, line):
            # Skip the #ifdef/#if line
            i += 1
            
            # Find the matching #endif, tracking nesting and looking for #else
            depth = 1
            has_else = False
            else_line = -1
            endif_line = -1
            block_start = i
            
            while i < len(lines) and depth > 0:
                curr_line = lines[i]
                
                if re.match(ifdef_pattern, curr_line):
                    depth += 1
                elif re.match(endif_pattern, curr_line):
                    depth -= 1
                    if depth == 0:
                        endif_line = i
                elif re.match(else_pattern, curr_line) and depth == 1:
                    has_else = True
                    else_line = i
                
                i += 1
            
            # Now handle the block
            if has_else:
                # Keep content from after #else until #endif
                for j in range(else_line + 1, endif_line):
                    result.append(lines[j])
            # else: remove everything until #endif (already skipped)
            
            continue
        
        result.append(line)
        i += 1
    
    # Clean up the result: remove leading/trailing blank lines
    output = '\n'.join(result)
    return output.strip() + '\n' if output.strip() else ''

def process_c_file(content):
    """Remove #ifdef SOLUTION blocks from C code."""
    ifdef_pattern = r'\s*#ifdef\s+SOLUTION\b'
    else_pattern = r'\s*#else\b'
    endif_pattern = r'\s*#endif\b'
    define_pattern = r'\s*#define\s+SOLUTION\b'
    
    return remove_ifdef_blocks(content, ifdef_pattern, else_pattern, endif_pattern, define_pattern)

def process_asm_file(content):
    """Remove .if SOLUTION blocks from assembly code."""
    ifdef_pattern = r'\s*\.if\s+SOLUTION\b'
    else_pattern = r'\s*\.else\b'
    endif_pattern = r'\s*\.endif\b'
    define_pattern = r'\s*\.set\s+SOLUTION\b'
    
    return remove_ifdef_blocks(content, ifdef_pattern, else_pattern, endif_pattern, define_pattern)

def process_file(input_path):
    """Process a file and return the modified content."""
    with open(input_path, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    
    filename = input_path.name
    
    # Determine file type and process accordingly
    if is_c_style(filename):
        return process_c_file(content)
    elif is_asm_style(filename):
        return process_asm_file(content)
    else:
        # Default to C-style processing
        return process_c_file(content)

def main():
    if len(sys.argv) < 2:
        print("Usage: publish.py <file1> [file2] ...")
        sys.exit(1)
    
    # Get the exercises directory (parent of the script)
    script_dir = Path(__file__).parent.absolute()
    exercises_dir = script_dir  # The script is in exercises/
    
    current_dir = Path.cwd()
    
    # Make sure we're in a subdirectory of exercises
    try:
        rel_path = current_dir.relative_to(exercises_dir)
    except ValueError:
        print(f"Error: Current directory {current_dir} is not under {exercises_dir}")
        sys.exit(1)
    
    # Determine the target directory
    # mdx07-templates-library is at the parent level of the workspace root
    workspace_root = exercises_dir.parent  # One level up from exercises
    parent_dir = workspace_root.parent     # Parent of workspace
    
    templates_base = parent_dir / 'mdx07-templates-library' / 'templates' / 'Assignment Templates'
    templates_dir = templates_base / str(rel_path)
    
    # Ensure the target directory exists
    templates_dir.mkdir(parents=True, exist_ok=True)
    
    # Process each file
    for file_arg in sys.argv[1:]:
        input_path = Path(file_arg).resolve()
        
        if not input_path.exists():
            print(f"Warning: File not found: {input_path}")
            continue
        
        # Process the file
        modified_content = process_file(input_path)
        
        # Determine output path, preserving relative directory structure from current dir
        try:
            rel_file_path = input_path.relative_to(current_dir)
        except ValueError:
            # If file is not relative to current dir, just use the filename
            rel_file_path = input_path.name
        
        output_path = templates_dir / rel_file_path
        
        # Ensure the output directory exists
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        # Write the processed content
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(modified_content)
        
        print(f"Processed: {rel_file_path} -> {output_path}")

if __name__ == '__main__':
    main()
