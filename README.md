# Executable input argument parsing and storage utilities

This is a set of libraries to parse argument inputs and store the parsing value results into 
vararg variables.  The intent of the library is to provide a quick and easy initialization 
input parsing utility so developers can focus on actual executable functionality.<br>
## libvarg
Is a library containing a set of variable argument value storage templates classes.  The varg 
template classes implement interger, unsigned integer, floating point, text strings, boolean flags,
incrementing and enumeration values.  In addition listvarg templates implement a list version of 
the varg templates for storage of multiple array like value inputs.
## libcmd_line_parse
Is a library to parse the input command line for an executable.  It uses vargs and listvargs classes
to store the results of the command line parsing.  The cmd_line_parse class uses an add argument 
interface similar to the Python randaleike/argparse class to define the association between the command line 
input and the varg/listvarg value storage location.
## libenvparse
Is a library to parse the input environment for an executable.  It uses vargs and listvargs classes
to store the results of the environment parsing.  The envparse uses an add argument interface to 
define the association between the environment argument input and the varg/listvarg value storage
location.
## libjson_arg_parser - Not implemented yet
Is a library to parse an input JSON file for an executable.  It uses vargs and listvargs classes
to store the results of the JSON parsing.  The config_json_parse uses an add argument interface to 
define the association between the JSON argument input and the varg/listvarg value storage
location.
## libxml_arg_parser - Not implemented yet
Is a library to parse an input XML file for an executable.  It uses vargs and listvargs classes
to store the results of the XML parsing.  The config_xml_parse uses an add argument interface to 
define the association between the XML argument input and the varg/listvarg value storage
location.
## libargparse
Single library containing libvarg, libcmd_line_parse and libenvparse

[Homepage for this project](https://github.com/randaleike/argparse)<br>
[Repository for this project](https://github.com/randaleike/randaleike/argparse)<br>
[Documentation for this project](https://github.com/randaleike/argparse/doc)<br>
[To report issues](https://github.com/randaleike/argparse/issues)<br>
[Changelog for this project](https://github.com/randaleike/argparse/CHANGELOG.md)<br>
[Contribution guidelines for this project](https://github.com/randaleike/argparse/CONTRIBUTING.md)<br>