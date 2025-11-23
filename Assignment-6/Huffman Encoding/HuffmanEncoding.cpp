/**********************************************************
 * File: HuffmanEncoding.cpp
 *
 * Implementation of the functions from HuffmanEncoding.h.
 * Most (if not all) of the code that you write for this
 * assignment will go into this file.
 */
#include "pqueue.h"
#include "HuffmanEncoding.h"

/* Function: getFrequencyTable
 * Usage: Map<ext_char, int> freq = getFrequencyTable(file);
 * --------------------------------------------------------
 * Given an input stream containing text, calculates the
 * frequencies of each character within that text and stores
 * the result as a Map from ext_chars to the number of times
 * that the character appears.
 *
 * This function will also set the frequency of the PSEUDO_EOF
 * character to be 1, which ensures that any future encoding
 * tree built from these frequencies will have an encoding for
 * the PSEUDO_EOF character.
 */

void createNodes(Map<ext_char, int>& frequencies, PriorityQueue<Node*>& ourNode);
void assembleTree(Map<ext_char, int>& frequencies, PriorityQueue<Node*>& ourNode);
void generate(Map<ext_char, string>&  codes,string, Node* encodingTree);
void writeBits(obstream&, string);
bool moveThroughTree(Node*& activeNode, int bit, ostream& outputStream);

Map<ext_char, int> getFrequencyTable(istream& file) {
	
	Map<ext_char, int> freqTable;
	if (file.fail()) {
		error("Cannot open input file!");
	}
	char ch;
	while (file.get(ch)) {
		freqTable[ext_char(ch)]+=1;
	}
	freqTable[PSEUDO_EOF] = 1;
	return freqTable;
}

void createNodes(Map<ext_char, int>& frequencies, PriorityQueue<Node*>& ourNode) {
	for (ext_char currSymbol : frequencies) {
		Node* leaf = new Node;
		leaf->character = currSymbol;
		leaf->weight = frequencies[currSymbol];
		leaf->zero = NULL;
		leaf->one = NULL;
		ourNode.enqueue(leaf, frequencies[currSymbol]);
	}
}

void assembleTree(Map<ext_char, int>& frequencies, PriorityQueue<Node*>& ourNode) {
	while (ourNode.size() >= 2) {
		//left child
		Node* leftOne = ourNode.dequeue();
		//right child
		Node* rightOne = ourNode.dequeue();

		Node* parentNode = new Node();

		int firstCnt = leftOne->weight;
		int secondCnt = rightOne->weight;
		//sum of counts of children
		int sumOfCount = firstCnt + secondCnt;

		parentNode->weight = sumOfCount;
		parentNode->character = NOT_A_CHAR;
		parentNode->zero = leftOne;
		parentNode->one = rightOne;
		
		ourNode.enqueue(parentNode, sumOfCount);
	 }
}

/* Function: buildEncodingTree
 * Usage: Node* tree = buildEncodingTree(frequency);
 * --------------------------------------------------------
 * Given a map from extended characters to frequencies,
 * constructs a Huffman encoding tree from those frequencies
 * and returns a pointer to the root.
 *
 * This function can assume that there is always at least one
 * entry in the map, since the PSEUDO_EOF character will always
 * be present.
 */

Node* buildEncodingTree(Map<ext_char, int>& frequencies) {

	PriorityQueue<Node*> ourNode;
	// creates nodes
	createNodes(frequencies, ourNode);
	// assemble tree
	assembleTree(frequencies,ourNode);

	if (ourNode.isEmpty()) return NULL;
	return ourNode.dequeue();

}

/* Function: freeTree
 * Usage: freeTree(encodingTree);
 * --------------------------------------------------------
 * Deallocates all memory allocated for a given encoding
 * tree.
 */

// recursive method for deleting root
void freeTree(Node* root) {
	if (!root) return;

	freeTree(root->zero);
	freeTree(root->one);

	delete root;
}

// generates codes
void generate(Map<ext_char, string>& codes, string curr, Node* encodingTree) {
	if (!encodingTree) {
		return;
	}

	if (encodingTree->character != NOT_A_CHAR) {
		codes[encodingTree->character] = curr;
		return;
	}

	if(encodingTree->zero)(generate(codes, curr + '0', encodingTree->zero));
	if (encodingTree->one)(generate(codes, curr + '1', encodingTree->one));

}
/* Function: encodeFile
 * Usage: encodeFile(source, encodingTree, output);
 * --------------------------------------------------------
 * Encodes the given file using the encoding specified by the
 * given encoding tree, then writes the result one bit at a
 * time to the specified output file.
 *
 * This function can assume the following:
 *
 *   - The encoding tree was constructed from the given file,
 *     so every character appears somewhere in the encoding
 *     tree.
 *
 *   - The output file already has the encoding table written
 *     to it, and the file cursor is at the end of the file.
 *     This means that you should just start writing the bits
 *     without seeking the file anywhere.
 */
void encodeFile(istream& infile, Node* encodingTree, obstream& outfile) {
	if (!encodingTree) {
		error("Encoding tree is empty!");
	}
	Map<ext_char, string> huffCode;
	generate(huffCode,"",encodingTree);

	int ch = infile.get();
	while (ch != -1) {
		ext_char symbol = ext_char(ch);
		writeBits(outfile, huffCode[symbol]);
	    ch = infile.get();
	}

	// for pseudo eof
	for (char bit : huffCode[PSEUDO_EOF]) {
		outfile.writeBit(bit == '1' ? 1 : 0);
	}
}

// method which is writing bits according to their value
void writeBits(obstream& outfile, string cur) {
	for (char ch : cur) {
		if (ch == '1') {
			outfile.writeBit(1);
		}
		else {
			outfile.writeBit(0);
		}
	}
}

/* Function: decodeFile
 * Usage: decodeFile(encodedFile, encodingTree, resultFile);
 * --------------------------------------------------------
 * Decodes a file that has previously been encoded using the
 * encodeFile function.  You can assume the following:
 *
 *   - The encoding table has already been read from the input
 *     file, and the encoding tree parameter was constructed from
 *     this encoding table.
 *
 *   - The output file is open and ready for writing.
 */
bool moveThroughTree(Node*& node, int tmp, ostream& file) {
	// go to the next node 
	if (tmp == 0) {
		node = node->zero;
	}
	// go to the next node 
	else {
		node = node->one;
	}
	// if is not char
	if (node->character == NOT_A_CHAR) {
		return true;
	}
	// if last one is pseudo eof
	if (node->character == PSEUDO_EOF) {
		return false;
	}

	file.put(node->character);
	return true;
	
}

void decodeFile(ibstream& infile, Node* encodingTree, ostream& file) {
	// if theres problem in reading file.
	if (infile.fail()) {
		error("Cannot read compressed file!");
	}
	
	Node* node = encodingTree;
	while (true) {
		// reading next bit
		int tmp = infile.readBit();
		
		bool isRight = moveThroughTree(node, tmp, file);
		if (!isRight) break;
		
		if (node->character != NOT_A_CHAR) {
			node = encodingTree;
		}
	}
}

/* Function: writeFileHeader
 * Usage: writeFileHeader(output, frequencies);
 * --------------------------------------------------------
 * Writes a table to the front of the specified output file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to decompress input files once they've been
 * compressed.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * readFileHeader function defined below this one so that it
 * can properly read the data back.
 */
void writeFileHeader(obstream& outfile, Map<ext_char, int>& frequencies) {
	/* The format we will use is the following:
	 *
	 * First number: Total number of characters whose frequency is being
	 *               encoded.
	 * An appropriate number of pairs of the form [char][frequency][space],
	 * encoding the number of occurrences.
	 *
	 * No information about PSEUDO_EOF is written, since the frequency is
	 * always 1.
	 */
	 
	/* Verify that we have PSEUDO_EOF somewhere in this mapping. */
	if (!frequencies.containsKey(PSEUDO_EOF)) {
		error("No PSEUDO_EOF defined.");
	}
	
	/* Write how many encodings we're going to have.  Note the space after
	 * this number to ensure that we can read it back correctly.
	 */
	outfile << frequencies.size() - 1 << ' ';
	
	/* Now, write the letter/frequency pairs. */
	foreach (ext_char ch in frequencies) {
		/* Skip PSEUDO_EOF if we see it. */
		if (ch == PSEUDO_EOF) continue;
		
		/* Write out the letter and its frequency. */
		outfile << char(ch) << frequencies[ch] << ' ';
	}
}

/* Function: readFileHeader
 * Usage: Map<ext_char, int> freq = writeFileHeader(input);
 * --------------------------------------------------------
 * Reads a table to the front of the specified input file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to reconstruct the encoding tree for that file.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * writeFileHeader function defined before this one so that it
 * can properly write the data.
 */
Map<ext_char, int> readFileHeader(ibstream& infile) {
	/* This function inverts the mapping we wrote out in the
	 * writeFileHeader function before.  If you make any
	 * changes to that function, be sure to change this one
	 * too!
	 */
	Map<ext_char, int> result;
	
	/* Read how many values we're going to read in. */
	int numValues;
	infile >> numValues;
	
	/* Skip trailing whitespace. */
	infile.get();
	
	/* Read those values in. */
	for (int i = 0; i < numValues; i++) {
		/* Get the character we're going to read. */
		ext_char ch = infile.get();
		
		/* Get the frequency. */
		int frequency;
		infile >> frequency;
		
		/* Skip the space character. */
		infile.get();
		
		/* Add this to the encoding table. */
		result[ch] = frequency;
	}
	
	/* Add in 1 for PSEUDO_EOF. */
	result[PSEUDO_EOF] = 1;
	return result;
}

/* Function: compress
 * Usage: compress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman compressor.  Compresses
 * the file whose contents are specified by the input
 * ibstream, then writes the result to outfile.  Your final
 * task in this assignment will be to combine all of the
 * previous functions together to implement this function,
 * which should not require much logic of its own and should
 * primarily be glue code.
 */
void compress(ibstream& infile, obstream& outfile) {
	Map<ext_char, int> freq = getFrequencyTable(infile);
	writeFileHeader(outfile, freq);

	infile.rewind();

	Node* encTree = buildEncodingTree(freq);
	encodeFile(infile, encTree, outfile);

	freeTree(encTree);
}
/* Function: decompress
 * Usage: decompress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman decompressor.
 * Decompresses the file whose contents are specified by the
 * input ibstream, then writes the decompressed version of
 * the file to the stream specified by outfile.  Your final
 * task in this assignment will be to combine all of the
 * previous functions together to implement this function,
 * which should not require much logic of its own and should
 * primarily be glue code.
 */
void decompress(ibstream& infile, ostream& outfile) {
	Map<ext_char, int> freq = readFileHeader(infile);

	Node* encTree = buildEncodingTree(freq);
	decodeFile(infile, encTree, outfile);

	freeTree(encTree);
}

