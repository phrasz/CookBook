#!/bin/sh

# ============================================================================
# A barebones Script Starter for BASH
	version=0.0
	progname=$(basename $0)
# ============================================================================

# Orignal Copyright:
# Copyright (C) 2007 by Bob Proulx <[hidden email]>.
# Found on: http://gnu-bash.2382.n7.nabble.com/Bash-getopts-option-td3251.html
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.


# ==============================================
print_help(){
    cat <<'EOF'
Usage: exampleprog [options] SOURCE...
	or:  exampleprog [options] DIRECTORY

Write your man page description here.

Options:
      --help          print this help message
      --version       print program version
  -a, --alpha         do alpa
  -b, --bravo=STRING  do bravo
  -d, --debug         debug program
  -q, --quiet         quiet output
  -v, --verbose       verbose output

Examples:

The most common use is to run it like this.

  $ exampleprog

But sometimes like this.

  $ exampleprog -q -a --bravo=foo

Report bugs to <mailing-address>
EOF
}
# ==============================================

# ==============================================
print_version() {
	cat <<EOF
	$progname (@GNU_PACKAGE@) $version
	Copyright (C) @RELEASE_YEAR@ Free Software Foundation, Inc.
	This is free software.  You may redistribute copies of it under the terms of
	the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.
	There is NO WARRANTY, to the extent permitted by law.

	Originally Written by Bob Proulx.

	Barebone Script Created bt Phrasz
EOF
}
# ==============================================



# Start_Time=`date | awk '{print $4}'`
Start_Time=`date +%s%N`

SHORTOPTS="ab:dqv"
LONGOPTS="help,version,alpha,bravo:,debug,quiet,verbose"

if $(getopt -T >/dev/null 2>&1) ; [ $? = 4 ] ; then # New longopts getopt.
    OPTS=$(getopt -o $SHORTOPTS --long $LONGOPTS -n "$progname" -- "$@")
else # Old classic getopt.
    # Special handling for --help and --version on old getopt.
    case $1 in --help) print_help ; exit 0 ;; esac
    case $1 in --version) print_version ; exit 0 ;; esac
    OPTS=$(getopt $SHORTOPTS "$@")
fi

if [ $? -ne 0 ]; then
    echo "'$progname --help' for more information" 1>&2
    exit 1
fi

eval set -- "$OPTS"

# INTIALIZATIONS:
alpha=false
bravo=""
debug=false
quiet=false
verbose=false


while [ $# -gt 0 ]; do
    : debug: $1
    case $1 in
        --help)
            print_help
            exit 0
            ;;
        --version)
            print_version
            exit 0
            ;;
        -a|--alpha)
            alpha=true
            shift
            ;;
        -b|--bravo)
            bravo=$2
            shift 2
            ;;
	-d | --debug)
            debug=true
            shift
            ;;
        -q | --quiet)
            quiet=true
            shift
            ;;
        -v | --verbose)
            verbose=true
            shift
            ;;
        --)
            shift
            break
            ;;
        *)
            echo "Internal Error: option processing error: $1" 1>&2
            exit 1
            ;;
    esac
done
if $verbose; then
	echo "Program Started: $Start_Time"
	echo "PLACE Verbose output here."
#	End_Time=`date | awk '{print $4}'`
fi

if $quiet; then
#if ! $quiet; then
    echo PLACE PROGRAM HERE THAT IS THE QUIET VERSION
fi

$debug && echo No bugs found here.

End_Time=`date +%s%N`

if $verbose; then
echo "Program Ended: $End_Time"
fi

echo "Program Runtime: " $(( (End_Time - Start_Time) / 1000 ))" microseconds"

exit 0
