#!/usr/bin/env
import re

def select_race(character):
    

	regex=re.compile("[edmh]")
	datamapper={
		"e": {"strength":  6, "intelligence": 8, "dexterity": 10, "name": "Elf"},
		"d": {"strength": 10, "intelligence": 8, "dexterity":  6, "name": "Dwarf"},
		"m": {"strength":  8, "intelligence": 8, "dexterity":  8, "name": "Man"},
		"h": {"strength":  4, "intelligence": 8, "dexterity":  6, "name": "Hobbit"}
		}

	print("All right, bold one.")

	choice=""
	race=""

	while(race.lower()==""):
		print("You may be an Elf, Dwarf, Man, or Hobbit")

		try:
			print("\nYour Choice? ")
			choice=input()[0].lower()
		except:
			pass
		if  re.match(regex, choice):
			race=choice
		else:
			print("** That was incorrect. Please type E, D, M, or H.")

	character["race"]=datamapper.get(race).get("name")
	character["strength"]=datamapper.get(race).get("strength")
	character["dexterity"]=datamapper.get(race).get("dexterity")
	character["dexterity"]=datamapper.get(race).get("intelligence")

	return character


def select_sex(character):
    
	sex=""

	datamapper={"m": "Male", "f": "Female"}
	regex=re.compile("[mf]")

	choice=""
	while(sex.lower()==""):
		try:
			print("\nWhich sex do you prefer? ")
			choice=input()[0].lower()
		except:
			pass
		if re.match(regex, choice):
			sex=datamapper.get(choice)
		else:
			print("** Cute", character["race"], "real cute.  Try M or F.")

	character["sex"]=sex
	return sex

def print_intro():
	# Print Intro

	buffer=""
	for x in range(0,64):
		buffer=buffer+"*"

	buffer=buffer+("\n\n                * * * THE WIZARD'S CASTLE * * *\n\n")
	for x in range(0,64):
		buffer=buffer+"*"
	buffer=buffer+"\n\n"
	buffer=buffer+"""Many cycles ago, in the kindom of N'Dic, the gnomic
wizard zot forged his great *Orb of Power*. He soon
vanished, leaving behind his vast subterranean castle
filled with esurrient monsters, fabulous treasures, and
the incredible *Orb of Zot*. From that time hence, many
a bold youth has ventured into the wizard's castle. as
of now, *NONE* has ever emerged victoriously! Beware!!\n"""

	print(buffer)


def main():

	character={}	
	
	print_intro()
	character=select_race(character)
	character=select_sex(character)	


main()

