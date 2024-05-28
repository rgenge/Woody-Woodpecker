#!/bin/sh

error_exit() {
	echo "$1" && exit 1 
}

make || error_exit "[ KO ] Failed to make"

./woody_woodpacker sample && \
	echo '<- [ OK? ] Output must be empty or some successful message.' || error_exit '[ KO ] Program failed to execute.'

./woody && (
	echo "<- [ OK? ] \`woody\` is run, must have the same behavior of \`sample\`, but with \'....WOODY.....\' on top."
	echo "This is the output from \`sample\`:"
	./sample
	./woody > x.tmp
	tail -n +2 x.tmp > a.tmp
	./sample > b.tmp
	diff a.tmp b.tmp && echo "[ OK ] Auto-check, the outputs are identical but for the 1st line." || echo "[ OK ] Incorrect output."
	rm a.tmp b.tmp x.tmp
	) || error_exit "[ KO ] Something failed when executing \`woody\`."
