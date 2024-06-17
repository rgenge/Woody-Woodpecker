#!/bin/bash

error_exit() {
	rm -f woody_error;
	rm -f a.tmp b.tmp
	red
	echo "$1"
	normal
	exit 1
}

red() { tput setaf 1; }
green() { tput setaf 2; }
yellow() { tput setaf 3; }
normal() { tput sgr0; }

./woody_woodpacker "$1" && \
	(yellow && \
	echo "[ OK? ] Output OK?") || error_exit "[ KO ] \`woody_woodpacker\` crashed or returned non-zero status."

	[[ -f woody ]] && (green && echo "[ OK ] File \`woody\` exists.") || error_exit "[ KO ] File \`woody\` was not generated."

	[[ -x woody ]] && (green && echo "[ OK ] \`woody\` is executable.") || error_exit "[ KO ] \`woody\` is not executable."

if [[ $(readelf -h ./woody 1>/dev/null 2>woody_error) ]]; then
	red
	echo "[ KO ] \`woody\` is not a valid ELF."
	error_exit "$(cat woody_error)"
else
	green && echo "[ OK ] Valid ELF."
fi;

	yellow
	echo "This is the output from \`$1\`:"
	normal
	"./$1"
	echo "Return value: $?";

	yellow
	echo "This is the output from \`./woody\`:"
	normal
	./woody
	echo "Return value: $?";

	yellow
	echo "[ OK? ] \`woody\` is run, must have the same behavior of \`$1\`, but with \'....WOODY.....\' on top."

	./woody > out.tmp
	cat out.tmp | tail -n +2 > a.tmp
	"./$1" > b.tmp;
	check=$(diff a.tmp b.tmp)
	[[ -n "$(cat a.tmp)" ]] && \
		(green && echo "[ OK ] There is some response.") \
		|| red && error_exit "[ KO ] Empty output." 
	[[ -z "$check" ]] && (green && echo "[ OK ] Auto-check, the outputs are identical but for the 1st line.") || error_exit "[ OK ] Incorrect output (not identical)."

	rm -f a.tmp b.tmp
	rm -f woody_error

	normal
