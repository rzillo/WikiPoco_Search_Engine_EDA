
#!bin/bash
CONVERT=" iconv -f ISO-8859-15 -t UTF-8"
for file in englishText*;
do
    $CONVERT "$file" -o "$file(conv)"
done
exit 0
