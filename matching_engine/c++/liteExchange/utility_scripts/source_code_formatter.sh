#/bin/bash
#
#   What it does :
#
#   1. Uses dos2unix to convert Windows EOLs to Linux/Unix EOLs
#   2. Converts TABS to 4 spaces
#   3. Removes trailing whitespaces
#
#   Other considerations :
#
#   1. File filters : Currently acts on cpp , h and hpp files , modify FIND_FILE_FILTER to change it
#   2. Keeping original files before conversion : set SED_ARGUMENT to "-i.orig"
#   3. We ignore source files in dependencies directories
#
#   Dependencies :
#
#       dos2unix
#       sed
#
SED_ARGUMENT='-i' # Replace with -i.orig in order to save original files
FIND_SEARCH_DIRECTORY='..'
FIND_TYPE='-type f' # Files only
FIND_FILE_FILTER=' -name \*.cpp -o -name \*.hpp -o -name \*.h -o -name \*.sh -o -name \*.py;' # File extensions we want
GREP_IGNORE_FILTER='egrep -v '^*dependencies*''

IS_COMMAND_VALID_RESULT=0
function is_command_valid()
{
    local command_name=$1
    local which_command=`which ${command_name}`
    local valid_command=${#which_command}
    if [ $valid_command -eq 0 ]; then
        IS_COMMAND_VALID_RESULT=0
        return
    fi
    IS_COMMAND_VALID_RESULT=1
}

function check_dos2unix()
{
    is_command_valid "dos2unix"
    if [ $IS_COMMAND_VALID_RESULT -eq 0 ]; then
        echo  "dos2unix is not available"
        exit -1
    else
        echo  "dos2unix is available"
    fi
}

function convert_end_of_lines()
{
    eval find ${FIND_SEARCH_DIRECTORY} ${FIND_TYPE} ${FIND_FILE_FILTER} | ${GREP_IGNORE_FILTER} | xargs dos2unix
}

function convert_tabs_to_spaces()
{
    eval find ${FIND_SEARCH_DIRECTORY} ${FIND_TYPE} ${FIND_FILE_FILTER} | ${GREP_IGNORE_FILTER} | xargs sed ${SED_ARGUMENT} $'s/\t/    /g'
}

function remove_trailing_whitespace()
{
    eval find ${FIND_SEARCH_DIRECTORY} ${FIND_TYPE} ${FIND_FILE_FILTER} | ${GREP_IGNORE_FILTER} | xargs sed  ${SED_ARGUMENT} '' -e's/[ \t]*$//'
}

check_dos2unix
echo "Formatting started"
convert_end_of_lines
convert_tabs_to_spaces
remove_trailing_whitespace
echo "Formatting completed"