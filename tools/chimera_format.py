# **********************************************************************************
#   FileName:
#       chimera_format.py
#
#   Description:
#       Runs the clang-format options on the entire Chimera source file directory.
#
#   Usage Examples:
#       N/A
#
#   2019 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************

import os
import sys

# Make the CommonTools directory visible to python as a package
this_dir = os.path.dirname(__file__)
common_tool_dir = os.path.realpath(os.path.join(this_dir, 'CommonTools'))
sys.path.insert(0, common_tool_dir)

from CommonTools import run_clangformat as cf

if __name__ == "__main__":
    abs_format_file = os.path.abspath(os.path.join(os.path.dirname(__file__), 'chimera_clangformat.json'))
    sys.argv.extend(['-f', abs_format_file])
    sys.exit(cf.run_clang_format())
