#!/bin/bash

error_exit() {
	echo "$1" && exit 1 
}

make || error_exit "[ KO ] Failed to make. Execute this script from the project root."

./woody_woodpacker sample && \
	echo "<- [ OK? ] Output must be empty or some successful message." || error_exit "[ KO ] \`woody_woodpacker\` crashed or returned non-zero status."

	[[ -f woody ]] && echo "[ OK ] File \`woody\` exists." || error_exit "[ KO ] File \`woody\` was not generated."

	[[ -x woody ]] && echo "[ OK ] \`woody\` is executable." || error_exit "[ KO ] \`woody\` is not executable."

	echo "This is the output from \`sample\`:"
	./sample
	echo "This is the output from \`woody\`:"
	./woody sample
	echo "<- [ OK? ] \`woody\` is run, must have the same behavior of \`sample\`, but with \'....WOODY.....\' on top."

	./woody > x.tmp
	tail -n +2 x.tmp > a.tmp
	./sample > b.tmp
	check=$(diff a.tmp b.tmp)
	rm a.tmp b.tmp x.tmp
	check && echo "[ OK ] Auto-check, the outputs are identical but for the 1st line." || error_exit "[ OK ] Incorrect output (not identical)."
