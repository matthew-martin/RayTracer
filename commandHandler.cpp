#include "commandHandler.h"

#include <iostream>
#include <string>
#include <vector>

#include "implicitShape.h"
#include "phongLightSource.h"
#include "shape.h"
#include "shapeCollection.h"
#include "main.h"
#include "viewport.h"


/*** Public Member Functions ***/

CommandHandler::CommandHandler()
{
	prompt = ">";
	input = "";
	loadedFileName = "";
	savedFileName = "";
}

void CommandHandler::getUserInput()
{
	std::cout << prompt;
	std::getline(std::cin, input);
	
	parseInput();
}

Command CommandHandler::execute(ShapeCollection* sc, Viewport* viewport, bool &redraw)
{
	int args = parsed.size();
	if (args == 0)
	{
		redraw = false;
		return cError;
	}
	
	// Translate the command from a string to enum value.
	Command command;
	try
	{
		command = commandMap.at(getArgString(0));
	}
	catch (const std::out_of_range& ex)
	{
		std::cout << "Command not recognized." << std::endl;
		redraw = false;
		return cError;
	}
	
	// Execute the correct command on the shapeCollection/viewport.
	redraw = true;
	bool notEnoughArgs = false;
	switch (command)
	{
		case cAddCube:
		{
			if (args <= 11)
			{
				notEnoughArgs = true;
			}
			else
			{
				sc->add(
					Shape::createCube(
						FCoord3D(getArgFloat(1), getArgFloat(2), getArgFloat(3)),
						getArgFloat(4),
						RGB(getArgFloat(5), getArgFloat(6), getArgFloat(7)),
						getArgFloat(8),
						getArgFloat(9),
						getArgFloat(10),
						getArgInt(11)
					)
				);
			}
			break;
		}
		
		case cAddImplicit:
		{
			if (args <= 17)
			{
				notEnoughArgs = true;
			}
			else
			{
				Shape* shape =
					new ImplicitShape(
						getArgFloat(1),
						getArgFloat(2),
						getArgFloat(3),
						getArgFloat(4),
						getArgFloat(5),
						getArgFloat(6),
						getArgFloat(7),
						getArgFloat(8),
						getArgFloat(9),
						getArgFloat(10)
					);
				shape->setColor(RGB(getArgFloat(11), getArgFloat(12), getArgFloat(13)));
				shape->setRefl(getArgFloat(14));
				shape->setRefr(getArgFloat(15));
				shape->setRefractiveIndex(getArgFloat(16));
				shape->setPhongExponent(getArgInt(17));
				sc->add(shape);
			}
			break;
		}
		
		case cAddSphere:
		{
			if (args <= 11)
			{
				notEnoughArgs = true;
			}
			else
			{
				sc->add(
					Shape::createSphere(
						FCoord3D(getArgFloat(1), getArgFloat(2), getArgFloat(3)),
						getArgFloat(4),
						RGB(getArgFloat(5), getArgFloat(6), getArgFloat(7)),
						getArgFloat(8),
						getArgFloat(9),
						getArgFloat(10),
						getArgInt(11)
					)
				);
			}
			break;
		}
		
		case cAtPointMove:
		{
			if (args <= 2)
			{
				notEnoughArgs = true;
			}
			else
			{
				viewport->moveAtPoint(directionStringToEnum(getArgString(1)), getArgFloat(2));
			}
			break;
		}
		
		case cCameraMove:
		{
			if (args <= 2)
			{
				notEnoughArgs = true;
			}
			else
			{
				viewport->moveCamera(directionStringToEnum(getArgString(1)), getArgFloat(2));
			}
			break;
		}
		
		case cDelete:
		{
			if (args <= 1)
			{
				notEnoughArgs = true;
			}
			else
			{
				sc->remove(getArgInt(1));
			}
			break;
		}
		
		case cDeleteLight:
		{
			if (args <= 1)
			{
				notEnoughArgs = true;
			}
			else
			{
				viewport->deleteLight(getArgInt(1));
			}
			break;
		}
		
		case cFromPointMove:
		{
			if (args <= 2)
			{
				notEnoughArgs = true;
			}
			else
			{
				viewport->moveFromPoint(directionStringToEnum(getArgString(1)), getArgFloat(2));
			}
			break;
		}
		
		case cLight:
		{
			if (args <= 7)
			{
				notEnoughArgs = true;
			}
			else
			{
				viewport->addLight(
					new PhongLightSource(
						RGB(getArgFloat(1), getArgFloat(2), getArgFloat(3)),
						getArgFloat(4),
						FCoord3D(getArgFloat(5), getArgFloat(6), getArgFloat(7))
					)
				);
			}
			
			break;
		}
		
		case cLoad:
		{
			if (args <= 1) notEnoughArgs = true;
			else
			{
				bool success = sc->loadFromFile(getArgString(1));
				if (success)
				{
					std::cout << "Loaded from \"" << getArgString(1) << "\" successfully." << std::endl;
					loadedFileName = getArgString(1);
				}
				else
				{
					std::cout << "Failed to load from file \"" << getArgString(1) << "\"." << std::endl;
				}
			}
			break;
		}
		
		case cQuit:
		{
			std::string fileName = savedFileName == "" ? loadedFileName : savedFileName;
			if (fileName != "")
			{
				std::cout << "Would you like to save to file \"" << fileName << "\"? (y/n): ";
				std::string input;
				std::cin >> input;
				
				if (input == "yes" || input == "y")
				{
					bool success = sc->saveToFile(fileName);
					if (success)
					{
						std::cout << "Saved successfully." << std::endl;
					}
					else
					{
						std::cout << "Could not save to file \"" << fileName << "\"." << std::endl;
					}
				}
			}
			exit(EXIT_SUCCESS);
			break;
		}
		
		case cSave:
		{
			std::string fileName = (args == 1) ?
				(savedFileName == "" ? loadedFileName : savedFileName) :
				getArgString(1);
			
			bool success = sc->saveToFile(fileName);
			if (success)
			{
				std::cout << "Saved to file \"" << fileName << "\" successfully." << std::endl;
				savedFileName = fileName;
			}
			else
			{
				if (fileName == "")
				{
					std::cout
						<< "Saving failed. No file has been loaded or saved, and no file name specified."
						<< std::endl;
				}
				else
				{
					std::cout << "Failed to save to file \"" << fileName << "\"." << std::endl;
				}
			}
			redraw = false;
			break;
		}
		
		case cSetAtPoint:
		{
			if (args == 1)
			{
				FCoord3D aa = viewport->getAtPoint();
				std::cout << "(" << aa.x << ", " << aa.y << ", " << aa.z << ")" << std::endl;
				redraw = false;
			}
			else if (args <= 3)
			{
				notEnoughArgs = true;
			}
			else
			{
				viewport->setAtPoint(FCoord3D(getArgFloat(1), getArgFloat(2), getArgFloat(3)));
			}
			break;
		}
		
		case cSetFromPoint:
		{
			if (args == 1)
			{
				FCoord3D ff = viewport->getFromPoint();
				std::cout << "(" << ff.x << ", " << ff.y << ", " << ff.z << ")" << std::endl;
				redraw = false;
			}
			else if (args <= 3)
			{
				notEnoughArgs = true;
			}
			else
			{
				viewport->setFromPoint(FCoord3D(getArgFloat(1), getArgFloat(2), getArgFloat(3)));
			}
			break;
		}
		
		case cSetViewingAngle:
		{
			if (args <= 1)
			{
				notEnoughArgs = true;
			}
			else
			{
				viewport->setViewingAngle(getArgFloat(1));
			}
			break;
		}
		
		default:
		{
			std::cout << "Command not recognized." << std::endl;
			redraw = false;
			return cError;
		}
	}
	
	if (notEnoughArgs)
	{
		std::cout << "Not enough command arguments." << std::endl;
		redraw = false;
		return cError;
	}
	return command;
}

void CommandHandler::debug_dumpParsed()
{
	for (int i = 0; i < (int)parsed.size(); i++)
	{
		std::cout << i << ": \"" << parsed.at(i)
			<< "\"   int:" << getArgInt(i)
			<< "   float:" << getArgFloat(i) << std::endl;
	}
}


/*** Private Member Functions ***/

void CommandHandler::parseInput()
{
	std::vector<std::string> result;
	char c;
	std::string curr = "";
	
	for (int i = 0; i < (int)input.length(); i++)
	{
		c = input.at(i);
		if (ignoreChar(c))
		{
			if (curr != "")
			{
				result.push_back(toLower(curr));
				curr = "";
			}
		}
		else
		{
			curr += c;
		}
	}
	if (curr != "")
	{
		result.push_back(toLower(curr));
	}
	parsed = result;
}

int CommandHandler::getArgInt(int index)
{
	try
	{
		return std::stoi(parsed.at(index));
	}
	catch (...)
	{
		return 0;
	}
}

float CommandHandler::getArgFloat(int index)
{
	try
	{
		return std::stof(parsed.at(index));
	}
	catch (...)
	{
		return 0.0;
	}
}

std::string CommandHandler::getArgString(int index)
{
	return parsed.at(index);
}

bool CommandHandler::ignoreChar(char c)
{
	if (isalnum(c) || c == '.' || c == '-' || c == '+' || c == '_') return false;
	else return true;
}

std::string CommandHandler::toLower(std::string s)
{
	for (int i = 0; i < (int)s.length(); i++)
	{
		if (s.at(i) >= 65 && s.at(i) <= 90)
		{
			s.at(i) = s.at(i) + 32;
		}
	}
	return s;
}
