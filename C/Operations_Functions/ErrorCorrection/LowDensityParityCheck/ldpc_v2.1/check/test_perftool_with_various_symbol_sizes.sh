#!/bin/sh
#
# $Id: test_perftool_with_various_symbol_sizes.sh,v 1.2 2006/09/06 10:22:57 roca Exp $
#
#  Copyright (c) 2002-2006 INRIA - All rights reserved
#  (main author: Vincent Roca - vincent.roca@inrialpes.fr)
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
#  USA.


# test the LDPC codec with an application that opens/closes encoding sessions

host_name=`uname -s`


case ${host_name} in

	Linux)
	bin_path="../bin/linux/perf_tool"
	;;
	SunOS)
	bin_path="../bin/solaris/perf_tool"
	;;
	FreeBSD)
	bin_path="../bin/freebsd/perf_tool"
	;;
	# other OS???? todo
esac

# for debug...
verbosity=''
#verbosity='-v1'
#verbosity='-v2'

# k param
block_len='16 56 128 512 1000 10000'

# symbol size
symbol_sz='1 2 3 4 128 129 130 131 132 133'

echo ""
echo "Running perf_tool for various k/n/symbol size values"
echo ""
for k_arg in ${block_len}
do
	for s_arg in ${symbol_sz}
	do
		n_arg=`expr ${k_arg} \* 2`
		echo ""
		echo "${bin_path} ${verbosity} -k$k_arg -r${n_arg} -s${s_arg}"
		echo ""
		${bin_path} ${verbosity} -k$k_arg -r${n_arg} -s${s_arg}
		val=$?
		echo "returns ${val}"
		if [ ${val} -ne 0 ] ; then
			exit 1
		fi
	done
done


# ok, no problem
exit 0
