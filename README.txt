Information about the files:

	  In the wordle.cpp file, there is a string array which contains over 5000 words. If something looks funny about the file, that's what it is. I was unable to find a way to make the list of words into a separate file.

	  I have fully tested this file on the @cs3307.gaul.csd.uwo.ca server to ensure that it works.

To create a binary executable, upload this file on gaul and then run:

   	  g++ -std=c++14 -o wordle wordle.cpp -lwthttp -lwt

then

	  ./wordle --docroot . --http-listen 0.0.0.0:XYZ
	  (where XYZ is your port #)

When I was testing this program on my cs3307 gaul directory, i ran the following commands in the respective orders:

     g++ -std=c++14 -o wordle wordle.cpp -lwthttp -lwt
     ./wordle --docroot . --http-listen 0.0.0.0:16210
