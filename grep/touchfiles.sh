filename1="test_file_1.txt"
content1="Little Red Riding 
Hood was walking through the forest.
The fox was cunning, and the dog was loyal.
The lazy cat slept on the windowsill.

The leaves fell softly from the trees.
Little Red and her dog played in the park."

filename2="test_file_2.txt"
content2="    LITTLE RED HOOD
The fox watched from afar, hiding behind the bushes.
Lazy days are the best days.
Little by little, the leaves turned red.
The dog barked at the fox, but the fox was too quick.
Red leaves danced in the wind."

filename3="test_file_3.txt"
content3="Little"

filename4="test_file_4.txt"
content4="Little
Red
fox
dog
lazy
leaves"

echo "$content1" > "$filename1"
echo "$content2" > "$filename2"
echo "$content3" > "$filename3"
echo "$content4" > "$filename4"

