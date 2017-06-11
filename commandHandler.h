#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__

/* commandHandler.h
 * 
 * Handles user input from the command line.
 * It parses the input, and executes the correct command.
 *
 */

#include <map>
#include <string>
#include <vector>

#include "misc.h"

class ShapeCollection;
class Viewport;

enum Command {
	cAddCube,
	cAddImplicit,
	cAddSphere,
	cAtPointMove,
	cCameraMove,
	cDelete,
	cDeleteLight,
	cFromPointMove,
	cLight,
	cLoad,
	cQuit,
	cSave,
	cSetAtPoint,
	cSetFromPoint,
	cSetViewingAngle,
	
	cError
};

class CommandHandler
{
	public:
		/*** Public Member Functions ***/
		// Default constructor.
		CommandHandler();
		
		// Asks the user to input a command, then parses the input.
		void getUserInput();
		// Execute the command on the passed shape collection/ viewports (depending on command type).
		Command execute(ShapeCollection* sc, Viewport* viewport, bool &redraw);
		
		// Prints the parsed command (debugging).
		void debug_dumpParsed();
		
		
	private:
		/*** Private Member Functions ***/
		// Parses the user input string.
		void parseInput();
		// Returns the argument at the specified index as an int.
		int getArgInt(int index);
		// Returns the argument at the specified index as a float.
		float getArgFloat(int index);
		// Returns the argument at the specified index as a string.
		std::string getArgString(int index);
		
		// Returns true if the character should be ignored by the parser.
		static bool ignoreChar(char c);
		// Returns the string with all uppercase characters made lowercase.
		static std::string toLower(std::string);
	
		/*** Private Member Variables ***/
		// The string that is shown as a prompt to the user.
		std::string prompt;
		// The raw user input (after it is gotten).
		std::string input;
		// The parsed user input.
		std::vector<std::string> parsed;
		// The name of the last loaded file.
		std::string loadedFileName;
		// The name of the last saved file.
		std::string savedFileName;
		
		// The string-to-command mapping.
		// Used to convert user input strings into a command.
		std::map<std::string, Command> commandMap =
		{
			{"ac", cAddCube},
			{"cube", cAddCube},
			{"addcube", cAddCube},
			
			{"ai", cAddImplicit},
			{"implicit", cAddImplicit},
			{"addimplicit", cAddImplicit},
			
			{"as", cAddSphere},
			{"sphere", cAddSphere},
			{"addsphere", cAddSphere},
			
			{"ma", cAtPointMove},
			{"mat", cAtPointMove},
			{"mvat", cAtPointMove},
			{"moveat", cAtPointMove},
			
			{"mc", cCameraMove},
			{"mv", cCameraMove},
			{"move", cCameraMove},
			{"movecamera", cCameraMove},
			
			{"d", cDelete},
			{"dt", cDelete},
			{"del", cDelete},
			{"delete", cDelete},
			{"erase", cDelete},
			{"rem", cDelete},
			{"remove", cDelete},
			
			{"dl", cDeleteLight},
			{"rl", cDeleteLight},
			{"dell", cDeleteLight},
			{"dlight", cDeleteLight},
			{"dellight", cDeleteLight},
			{"removelight", cDeleteLight},
			
			{"mf", cFromPointMove},
			{"mfr", cFromPointMove},
			{"mvfrom", cFromPointMove},
			{"movefrom", cFromPointMove},
			
			{"li", cLight},
			{"lt", cLight},
			{"light", cLight},
			{"addlight", cLight},
			
			{"ld", cLoad},
			{"load", cLoad},
			{"lf", cLoad},
			{"lfile", cLoad},
			{"loadf", cLoad},
			{"loadfile", cLoad},
			
			{"q", cQuit},
			{"qt", cQuit},
			{"quit", cQuit},
			
			{"sv", cSave},
			{"save", cSave},
			{"sf", cSave},
			{"sfile", cSave},
			{"savef", cSave},
			{"savefile", cSave},
			
			{"at", cSetAtPoint},
			{"aa", cSetAtPoint},
			{"setat", cSetAtPoint},
			{"atpoint", cSetAtPoint},
			{"setatpoint", cSetAtPoint},
			
			{"ff", cSetFromPoint},
			{"setff", cSetFromPoint},
			{"from", cSetFromPoint},
			{"frompoint", cSetFromPoint},
			{"setfrompoint", cSetFromPoint},
			
			{"alpha", cSetViewingAngle},
			{"va", cSetViewingAngle},
			{"sva", cSetViewingAngle},
			{"viewangle", cSetViewingAngle},
			{"viewingangle", cSetViewingAngle},
			{"setviewingangle", cSetViewingAngle},
		};
};

#endif