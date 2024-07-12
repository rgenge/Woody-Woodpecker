#!/bin/bash

error_exit() {
	rm -f woody_error;
	rm -f a.tmp b.tmp out.tmp
	red
	echo "$1"
	normal
	exit 1
}

red() { tput setaf 1; }
green() { tput setaf 2; }
yellow() { tput setaf 3; }
normal() { tput sgr0; }

head -c 5042 /dev/urandom > noise;
./woody_woodpacker noise && error_exit "[ KO ] Invalid file was not invalidated." || (green && echo "[ OK ] Invalid file was correctly rejected." && normal)
rm noise

./woody_woodpacker "$1" && \
	(yellow && \
	echo "[ OK? ] Output OK?" && normal) || error_exit "[ KO ] \`woody_woodpacker\` crashed or returned non-zero status."

	[[ -f woody ]] && (green && echo "[ OK ] File \`woody\` exists." && normal) || error_exit "[ KO ] File \`woody\` was not generated."

	[[ -x woody ]] && (green && echo "[ OK ] \`woody\` is executable." && normal) || error_exit "[ KO ] \`woody\` is not executable."

if [[ $(readelf -h ./woody 1>/dev/null 2>woody_error) ]]; then
	red
	echo "[ KO ] \`woody\` is not a valid ELF."
	error_exit "$(cat woody_error)"
else
	green && echo "[ OK ] Valid ELF." && normal
fi;


	yellow
	echo "This is the output from \`$1\`:"
	normal
	"$1" > b.tmp;
	ret_out_a="$?"
	cat b.tmp
	echo "Return value: $ret_out_a";

	yellow
	echo "This is the output from \`./woody\`:"
	normal
	./woody > out.tmp
	ret_out_w="$?"
	cat out.tmp
	echo "Return value: $ret_out_w";

	cat out.tmp | tail -n +2 > a.tmp

	yellow
	echo "[ OK? ] \`woody\` is run, must have the same behavior of \`$1\`, but with \'....WOODY.....\' on top."
	normal
	[[ "$ret_out_a" -eq "$ret_out_w" ]] && (green && echo "[ OK ] Same return value." && normal) || error_exit "[ KO ] Return values differ."

	[[ -n "$(cat out.tmp)" ]] && \
		(green && echo "[ OK ] There was some ouput." && normal) \
		|| error_exit "[ KO ] Output must exist and be ...WOODY..."

	check=$(diff a.tmp b.tmp)
	[[ -z "$check" ]] && (green && echo "[ OK ] Auto-check, the outputs are identical but for the 1st line." && normal) || (yellow && echo "[ OK? ] Unmatched output, behavior must be the same." && normal)

	rm -f a.tmp b.tmp out.tmp
	rm -f woody_error
