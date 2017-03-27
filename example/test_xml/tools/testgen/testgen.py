# -*- coding: utf-8 -*-
'''
Created on 2017

Auxiliary script to generate tests for emunit display

Author: Radoslaw Koppel <r.koppel\@k-el.com>
'''
import os
import sys
import argparse
import json
import jinja2 as j2

def get_script_path():
    return os.path.dirname(os.path.realpath(sys.argv[0]))

def main(argv):
    u"""Main function
    
    The program that generates test files using patterns and settings from JSON
    files.
    """
    # Change default encoding to utf-8
    reload(sys)
    sys.setdefaultencoding('utf-8')
    
    # Start parsing
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--config', type=argparse.FileType('r'), required=True, 
        help='the test json configuration file')
    parser.add_argument('-o', '--output', type=argparse.FileType('wb'), required=True,
        help='the file where the output would be saved')

    args = parser.parse_args(argv)
    
    # Load the configuration file
    test_config = json.load(args.config)

    # Prepare Jinja2 template engine and render now
    j2env = j2.Environment(
        loader=j2.FileSystemLoader(get_script_path() + '/templates'),
        undefined=j2.StrictUndefined)
    ctemplate = j2env.get_template(test_config['_template']);
    
    args.output.write(ctemplate.render(test_config))

# Call main function
if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
