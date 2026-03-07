#!/usr/bin/env python3
content = open('aurelius_rpc_simplified.rpc', 'r', encoding='utf-8').read()

# Get Python section (between === PYTHON === and === GO ===)
python_section = content.split('=== PYTHON ===')[1].split('=== GO ===')[0]

# Split by the handlers marker (without triple quotes, they're on separate line)
handlers_marker = 'Aurelius RPC Protocol - Auto-generated handlers'
if handlers_marker in python_section:
    types_part = python_section.split(handlers_marker)[0]
    # Add back the triple quotes and marker
    handlers_part = '"""' + handlers_marker + python_section.split(handlers_marker)[1]
else:
    types_part = python_section
    handlers_part = ''

open('types_gen.py', 'w', encoding='utf-8').write(types_part.strip())
open('handlers_gen.py', 'w', encoding='utf-8').write(handlers_part.strip())
print('Types length:', len(types_part))
print('Handlers length:', len(handlers_part))

# Extract Go section
go_section = content.split('=== GO ===')[1]
open('rpc_types.go', 'w', encoding='utf-8').write(go_section.strip())
print('Go length:', len(go_section))
