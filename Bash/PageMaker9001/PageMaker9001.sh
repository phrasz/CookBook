clear

echo "   __                                                     __   __   __   _  "
echo "  /  |                     /|/|      /                   /  | /  | /  | /|  "
echo " (___| ___  ___  ___      ( / | ___ (     ___  ___      (___|(   |(   |( |  "
echo " |    |   )|   )|___)     |   )|   )|___)|___)|   )         )|   )|   )  | )"
echo " |    |__/||__/ |__       |  / |__/|| \  |__  |          __/ |__/ |__/  _|/ "
echo "           __/   "
echo " ==========================================================================="
echo " The following tool will create static html pages including header, content,"
echo " and footer portions. "
echo ""
#########################################################################################
#
# Variables
# ---------
#
header_default=Header.pm1
content_default=Content.pm1
footer_default=Footer.pm1
FileName=a.out
N=4;
#########################################################################################


#########################################################################################
#
# FileName
# --------
#
echo -n "(1/$N)Please enter your file's Output Name: "
read FileName
#########################################################################################


#########################################################################################
#
# Header
# ------
#
header=$header_default
echo -n "(2/$N) Would you like a new header(Y/N)? [$header_default]: "
read choice
if [[ $choice == "Y" ||  $choice == "Yes" ||  $choice == "y" ||  $choice == "yes" ]];
        then
                echo -n "(2/$N) Please enter your header file: "
		read header
		if [ -e "$header" ]; then
			echo "(2/$N) Saving $header to $FileName..."
		else
			echo "(2/$N)[ERROR] $header doesn't exist!"
			header=$header_default
			echo "(2/$N) Saving default ($header) to file..."
		fi
        #else
                #echo "There are NOT Ten Lines!"
                #echo "There are " $LinesReturned " Lines."
fi

#Print Header to File
cat $header > $FileName
#########################################################################################


#########################################################################################
#
# Content
# -------
#
content=$content_default
echo -n "(3/$N) Would you like to add NEW Content(Y/N)? [$content_default]: "
read choice
if [[ $choice == "Y" ||  $choice == "Yes" ||  $choice == "y" ||  $choice == "yes" ]];
        then
                echo -n "(3/$N) Please enter your content file: "
		read content
		if [ -e "$content" ]; then
			echo "(3/$N) Saving $content to $FileName..."
		else
			echo "(3/$N)[ERROR] $content doesn't exist!"
			content=$content_default
			echo "(3/$N) Saving default ($content) to file..."
		fi
        #else
                #echo "There are NOT Ten Lines!"
                #echo "There are " $LinesReturned " Lines."
fi

#Print Header to File
cat $content >> $FileName
#########################################################################################


#########################################################################################
#
# Footer
# ------
#
footer=$footer_default
echo -n "(4/$N) Would you like a new footer(Y/N)? [$footer_default]: "
read choice
if [[ $choice == "Y" ||  $choice == "Yes" ||  $choice == "y" ||  $choice == "yes" ]];
        then
                echo -n "(4/$N) Please enter your footer file: "
		read footer
		if [ -e "$footer" ]; then
			echo "(4/$N) Saving $footer to $FileName..."
		else
			echo "(4/$N)[ERROR] $header doesn't exist!"
			footerr=$footer_default
			echo "(4/$N) Saving default ($header) to file..."
		fi
        #else
                #echo "There are NOT Ten Lines!"
                #echo "There are " $LinesReturned " Lines."
fi

#Print Header to File
cat $footer >> $FileName
#########################################################################################


#########################################################################################
#
# Debugging Output
#
echo "================================="
echo "Result: $FileName"
echo ""
cat $FileName
#########################################################################################





#[TEMPLATE]
#check for file
#DirLs="Directory.log"
#if [ -e "$DirLs" ]; then
#echo "It exists!"
#else
#echo "It doesn't exist!"
#fi

#[TEMPLATE]
# Save to file
#Time=$(./time.sh)
#echo "Saving time to CurrentTime.txt..."
#echo $Time>>CurrentTime.txt

