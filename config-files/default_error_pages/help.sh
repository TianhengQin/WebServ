#!/bin/bash

# Loop through all .html files in the current directory
for file in *.html; do
    # Extract the number from the file name (e.g., "404" from "404.html")
    number=$(echo "$file" | grep -o '[0-9]*')

    # Write the HTML content into the file, substituting the number where necessary
    cat > "$file" <<EOF
<!DOCTYPE html>
<html>
	<title> $number </title>
	<body>
		<div>
			<H1> $number </H1>
			<p> TODO: </p>
		</div>
	</body>
</html>

EOF

done
