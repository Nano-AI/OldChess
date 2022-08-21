# This just converts all the SVG's to PNG's.
find -name "*.svg" -exec sh -c 'echo "converting ${1%.svg}";inkscape.com $1 -o ${1%.svg}.png' _ {} \;
