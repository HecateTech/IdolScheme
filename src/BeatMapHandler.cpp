#include "../include/BeatMapHandler.h"

///////////////////////////////////////////////////////////////
   /////////////////      Constructor      ///////////////
///////////////////////////////////////////////////////////////

BeatMapHandler::BeatMapHandler() {
#ifdef TARGET_WIN32
	vector<string> directories = getBeatMapDirectories("bin\\beatmaps\\");
#else // UNIX SYSTEMS
	vector<string> directories = getBeatMapDirectories("beatmaps/");
#endif //!TARGET_WIN32

	// for debugging purposes the loop goes through every
	// beatmap available in the directory and loads them all.
	for (string &path : directories) {
		beatMaps.push_back(setNoteParameters(path));
	}
	/// debug beatmap printing
	for (beatMap &bm : beatMaps) {
		cout << "BeatMap: " + bm.name << endl << endl;
		for (unsigned int j = 0; j < bm.noteParams.size(); j++) {
			cout << "Note: " + ofToString(j + 1) + " : line " + ofToString(bm.noteParams.at(j).lineNum) << endl;
			cout << "bpm:  " + ofToString(bm.noteParams.at(j).bpm)    << endl;
			cout << "off:  " + ofToString(bm.noteParams.at(j).offset) << endl;
			cout << "len:  " + ofToString(bm.noteParams.at(j).length) << endl;
			cout << "type: " + ofToString(bm.noteParams.at(j).type)   << endl;
			cout << "btn:  " + ofToString(bm.noteParams.at(j).button) << endl << endl;
		}
	}
	cout << "BeatMaps Loaded!" << endl << endl;
}

/////////////////////////////////////////////////////////////////
   /////////////////      Getters/Setters      ///////////////
/////////////////////////////////////////////////////////////////

vector<string> BeatMapHandler::getBeatMapDirectories(const string &path) {
	vector<string> fileList;

	/// if the path given isn't empty
	/// -----------------------------
	if (!path.empty()) {
		filesystem::path apkPath(path);				  // path to folder
		filesystem::recursive_directory_iterator end; // number of files in path

		for (filesystem::recursive_directory_iterator i(apkPath); i != end; ++i) {
			const filesystem::path cp = (*i); // saves file's directory
			fileList.push_back(cp.string());  // stores it into a vector
		}
	}

	return fileList;
}

//-------------------------------------------------------
beatMap BeatMapHandler::setNoteParameters(string path) {
	int lineNum = 1;		// the current line number of `rawText`.
	int defbpm = 30;		// default bpm set to all notes.
	bool nameSet = false;   // check for if you're setting the isbm name.
	bool bpmSet  = false;   // check for if you're setting the BPM.
	ifstream infile;		// the file that's being read.
	size_t pos = 0;			// which section of text you're on.
	string rawText;			// the raw text in each line.
	beatMap result;			// the resultant returned by the method.

	/// opens text file at path
	/// -----------------------
	infile.open(path);
	
	/// lambda to push strings into note objects.
	/// -----------------------------------------
	auto pb_Element = [&](string str) {
		if (!result.noteParams.empty()) {			// if there are notes available
			for (noteInfo &a : result.noteParams) { // for every note available
				if (a.lineNum == lineNum) {			// if current line is equal to note's line number.
					a.args.push_back(str);
					if (a.args.size() > 7) {		// if enough arguments to make a note.
						a.bpm = defbpm;
						a.convert();
					}
				}
			}
		}
	};

	/// runs through every line of text in the txt file.
	/// ------------------------------------------------
	while (!infile.eof()) {
		/// stores the selected line into a string
		/// --------------------------------------
		getline(infile, rawText);

		/// check if it's not whitespace or a comment.
		/// ------------------------------------------
		if (rawText.find_first_not_of(' ') != string::npos && rawText.compare(0, CMTPREFIX.size(), CMTPREFIX)) {
			
			/// remove all whitespace in the line selected.
			/// -------------------------------------------
			for (unsigned int i = 0; i < rawText.length(); i++)
				if (rawText[i] == ' ')
					rawText.erase(remove(rawText.begin(), rawText.end(), ' '), rawText.end());

			/// while it can find more delimiters
			/// ---------------------------------
			while ((pos = rawText.find(DELIMITER)) != string::npos) {
				if (!rawText.substr(0, pos).compare(0, NOTEKEY.size(), NOTEKEY)) { // if there's "note"
					result.noteParams.push_back(noteInfo(lineNum)); // adds new note
				}
				else if (!rawText.substr(0, pos).compare(0, NAMEKEY.size(), NAMEKEY)) { // if there's "beatname"
					nameSet = true; // enable the name check
				}
				else if (!rawText.substr(0, pos).compare(0, BPMKEY.size(), BPMKEY)) {
					bpmSet = true;    // enable the bpm check
				}

				pb_Element(rawText.substr(0, pos));			// adds string to 'args'
				rawText.erase(0, pos + DELIMITER.length()); // erases what it just saved
			}

			/// pushes remaining string from `rawText`
			/// --------------------------------------
			pb_Element(rawText);

			/// if changing the beatmap name
			/// ----------------------------
			if (nameSet) {
				char c;
				for (unsigned int i = 0; i < rawText.length(); i++) {
					c = rawText[i];
					if (isupper(c) && i != 0) {
						rawText.insert(i, " ");
						i++;
					}
				}
				result.name = rawText;
				nameSet = false;
			}
			/// if changing the bpm
			/// -------------------
			if(bpmSet) {
				defbpm = ofToInt(rawText);
				bpmSet = false;
			}
		}
		/// next line
		/// ---------
		lineNum++;
	}
	/// returns result after looping every line
	/// ---------------------------------------
	return result;
}

//-------------------------------------------------------
beatMap BeatMapHandler::beatMapMenu() {
	int i;
begin:

#ifdef TARGET_WIN32
	system("cls");
#else // UNIX_SYSTEMS
	system("clear");
#endif //!TARGET_WIN32

	for (i = 0; i < beatMaps.size(); i++) {
		cout << ofToString(i + 1) << ": " << beatMaps.at(i).name << endl;
	}
	cout << "\nEnter index of beatmap:" << endl << endl << "> ";
	cin  >> i;
	if (i > beatMaps.size() || i < 1)
		goto begin; // retry
	for (int j = 0; j < beatMaps.size(); j++) {
		if (i == (j+1)) {
			cout << "You chose: " << beatMaps.at(j).name << endl;
			return beatMaps.at(j);
		}
	}
	return beatMaps.at(0);
}