#!/bin/sh

echo '#include <nono.h>' > test.c
gcc -v test.c 2>&1 | grep '/'| \
   grep -v 'ignoring nonexistent directory' | grep -v '^GNU C ' | \
   grep -v "^COLLECT_LTO_WRAPPER" |grep -v '^gcc version' | \
   grep -v "^Configured with" |\
   grep -v 'test.c' | grep -v 'InstalledDir' > dirs.txt

cat dirs.txt|grep -v '(framework directory)' | sed "s/^ /-I/" >> .factor
cat dirs.txt|grep '(framework directory)' | sed "s/(framework directory)//"| sed "s/^ /-I/" >> .factor
gcc -dM -E - < /dev/null | sed "s/#define /-D/"|sed "s/ /=/" >>.factor

rm dirs.txt
rm test.c
